#include "rollers.h"
#include "main.h"

// Subclass-specific functions

int * Rollers::getX() {
    return &x;
}

void Rollers::setPower(double _power) {
    this->power = _power;
    this->topRollerMotor.move(this->power);
    this->botRollerMotor.move(this->power);
}

void Rollers::intake(double _power) {
    this->power = abs(_power);
    this->changeState(IN_STATE);
}

void Rollers::out(double _power) {
    if(_power > 0) {
        _power = -_power;
    }
    this->power = _power;
    this->changeState(OUT_STATE);
}

void Rollers::stop() {
    this->changeState(IDLE_STATE);
}

void Rollers::setBrakeMode(enum pros::motor_brake_mode_e mode) {
    this->topRollerMotor.set_brake_mode(mode);
    this->botRollerMotor.set_brake_mode(mode);
}

Rollers::Rollers(uint8_t _defaultState, okapi::Controller _controller) : SystemManager(_defaultState) {
    this->controller = _controller;
}

// Overrides
void Rollers::update() {
    this->position = (this->topRollerMotor.get_position() + this->botRollerMotor.get_position()) / 2;
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
        case RESET_STATE:
            break;
        case DISABLED_STATE:
            break;
    }
}

bool Rollers::changeState(uint8_t newState) {
    bool processed = SystemManager::changeState(newState);
    if(!processed) {
        return false;
    }

    switch(newState) {
        case IDLE_STATE:
            this->setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
            this->setPower(0);
            break;
        case IN_STATE:
            this->setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
            this->topRollerMotor.move(0);
            this->botRollerMotor.move(DEFAULT_ROLLER_SPEED);
            break;
        case OUT_STATE:
            this->setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
            this->topRollerMotor.move(0);
            this->botRollerMotor.move(DEFAULT_ROLLER_SPEED);
            break;
        case EJECT_STATE:
            this->setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
            this->topRollerMotor.move(-DEFAULT_ROLLER_SPEED);
            this->botRollerMotor.move(DEFAULT_ROLLER_SPEED);
            break;
        case CYCLE_STATE:
            this->setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
            this->topRollerMotor.move(DEFAULT_ROLLER_SPEED);
            this->botRollerMotor.move(DEFAULT_ROLLER_SPEED);
        case SHOOT_STATE:
            this->setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
            this->topRollerMotor.move(DEFAULT_ROLLER_SPEED);
            this->botRollerMotor.move(0);
        case OPERATOR_OVERRIDE:
            this->setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
            if (this->topRollerMotor.get_actual_velocity() > 20) {
                this->setPower(0);
            }
            break;
        case HOLD_STATE:
            this->setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
            this->topRollerMotor.tare_position();
            this->botRollerMotor.tare_position();
            this->target = 0;
            this->topRollerMotor.move_absolute(0, ROLLER_HOLD_SPEED);
            this->botRollerMotor.move_absolute(0, ROLLER_HOLD_SPEED);
            this->target = 0;
            break;
        case RESET_STATE:
            this->changeState(IDLE_STATE);
            break;
        case DISABLED_STATE:
            this->topRollerMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            this->botRollerMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            this->setPower(0);
            break;
    }

    return true;
}

void Rollers::fullReset() {
    this->stop();
    this->topRollerMotor.tare_position();
    this->botRollerMotor.tare_position();
    SystemManager::fullReset();
}