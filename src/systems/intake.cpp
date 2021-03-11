#include "intake.h"
#include "main.h"

// Subclass-specific functions

int * Intake::getX() {
    return &x;
}

void Intake::setPower(double _power) {
    this->power = _power;
    this->leftIntakeMotor.move(-this->power);
    this->rightIntakeMotor.move(this->power);
}

void Intake::intake(double _power) {
    this->power = abs(_power);
    this->changeState(IN_STATE);
}

void Intake::out(double _power) {
    if(_power > 0) {
        _power = -_power;
    }
    this->power = _power;
    this->changeState(OUT_STATE);
}

void Intake::control() {
    this->changeState(OPERATOR_OVERRIDE);
}

void Intake::stop() {
    this->changeState(HOLD_STATE);
}

Intake::Intake(uint8_t _defaultState, okapi::Controller _controller) : SystemManager(_defaultState) {
    this->controller = _controller;
}

// Overrides
void Intake::update() {
    this->position = (this->leftIntakeMotor.get_position() + this->rightIntakeMotor.get_position()) / 2;
    this->error = this->position - this->target;

    switch(this->state) {
        case IDLE_STATE:
            break;
        case IN_STATE:
            break;
        case OUT_STATE:
            break;
        case OPERATOR_OVERRIDE: {
            float newIntakeSpeed = joystickSlew(this->controller.getAnalog(okapi::ControllerAnalog::rightY)) * 127;
            this->setPower(-newIntakeSpeed);
            break; }
        case HOLD_STATE:
            if(abs(this->error) >= 40) {
                this->leftIntakeMotor.move_absolute(0, INTAKE_HOLD_SPEED);
                this->rightIntakeMotor.move_absolute(0, INTAKE_HOLD_SPEED);
            }
            break;
        case RESET_STATE:
            break;
        case DISABLED_STATE:
            break;
    }
}

bool Intake::changeState(uint8_t newState) {
    bool processed = SystemManager::changeState(newState);
    if(!processed) {
        return false;
    }

    switch(newState) {
        case IDLE_STATE:
            this->leftIntakeMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            this->leftIntakeMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            this->setPower(0);
            break;
        case IN_STATE:
            this->leftIntakeMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            this->rightIntakeMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            this->setPower(power);
            break;
        case OUT_STATE:
            this->leftIntakeMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            this->rightIntakeMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            this->setPower(power);
            break;
        case OPERATOR_OVERRIDE:
            this->leftIntakeMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            this->rightIntakeMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            if(this->leftIntakeMotor.get_actual_velocity() > 20) {
                this->setPower(0);
            }
            break;
        case HOLD_STATE:
            this->leftIntakeMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
            this->rightIntakeMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
            this->leftIntakeMotor.tare_position();
            this->rightIntakeMotor.tare_position();
            this->target = 0;
            this->leftIntakeMotor.move_absolute(0, INTAKE_HOLD_SPEED);
            this->rightIntakeMotor.move_absolute(0, INTAKE_HOLD_SPEED);
            this->target = 0;
            break;
        case RESET_STATE:
            this->changeState(IDLE_STATE);
            break;
        case DISABLED_STATE:
            this->leftIntakeMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            this->rightIntakeMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            this->setPower(0);
            break;
    }

    return true;
}

void Intake::fullReset() {
    this->leftIntakeMotor.tare_position();
    this->rightIntakeMotor.tare_position();
    SystemManager::fullReset();
}
