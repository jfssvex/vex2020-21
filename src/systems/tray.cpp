#include "main.h"
#include "tray.h"

#define MAX_TRAY 1020
#define HALF_STACK 300
#define OPERATOR_TRAY_VELOCITY 50

// Constructor
Tray::Tray(uint8_t _defaultState, okapi::Controller _controller) : SystemManager(_defaultState), controller(_controller) {}

// Sub-class specific functions

void Tray::stop() {
    this->power = 0;
    this->target = this->trayMotor.get_position();
    this->trayMotor.move(0);
}

void Tray::setPower(double _power) {
    this->power = _power;
}

void Tray::setTargetPowerControl(double _target, double _power) {
	this->trayMotor.move_absolute(_target, _power);
}

void Tray::setTarget(double _target) {
    this->target = _target;
    this->trayMotor.move_absolute(this->target, this->power);
}

void Tray::layCubes() {
    this->stacking = true;
    this->target = MAX_TRAY;
    this->changeState(LIFT_STATE);
}

void Tray::layHalf() {
    this->stacking = true;
    this->target = HALF_STACK;
    this->changeState(LIFT_STATE);
}

void Tray::layCubesAuton() {
    this->stacking = true;
	this->changeState(AUTON_LIFT);
}

void Tray::lower() {
    this->changeState(LOWER_STATE);
}

double Tray::getPowerFunction(double time) {
    return 60 * pow(2, -0.003*time) + 40;
}

double Tray::getReversePowerFunction(double time) {
    return 200;
    //return 70 * pow(2, 0.003*(time-1000)) + 60;
}

void Tray::moveTo(double _position) {
    this->stacking = false;
    this->target = _position;
    this->power = 127;
    this->changeState(LIFT_STATE);
}

void Tray::setCallback(void(*_callback)()) {
    this->callback = _callback;
}

void Tray::setOperatorControl(int op) {
    if(this->stacking) {
        return;
    }
    // if(this->state == LIFT_STATE || this->state == LOWER_STATE) { return; }
    if(this->state != OPERATOR_OVERRIDE) {
        this->changeState(OPERATOR_OVERRIDE);
    }
    this->opUp = op;
}

// Overrides
bool Tray::changeState(uint8_t newState) {
    bool processed = SystemManager::changeState(newState);
    if(!processed) {
        return false;
    }

    switch(newState) {
        case DISABLED_STATE:
            this->stop();
            break;
        case RESET_STATE:
            this->changeState(IDLE_STATE);
            break;
        case IDLE_STATE:
            this->stop();
            this->trayMotor.set_brake_mode(MOTOR_BRAKE_COAST);
            break;
        case AUTON_LIFT:
            this->target = MAX_TRAY;
            this->trayMotor.move_velocity(getPowerFunction(0));
            break;
        case LIFT_STATE:
            this->trayMotor.set_brake_mode(MOTOR_BRAKE_BRAKE);
            this->trayMotor.move_velocity(getPowerFunction(0));
            break;
        case LOWER_STATE:
            this->trayMotor.set_brake_mode(MOTOR_BRAKE_COAST);
            this->setTarget(0);
            break;
        case HOLD_STATE:
            this->trayMotor.set_brake_mode(MOTOR_BRAKE_HOLD);
            break;
        case OPERATOR_OVERRIDE:
            this->trayMotor.set_brake_mode(MOTOR_BRAKE_BRAKE);
    }

    return true;
}

uint8_t Tray::getTrayState() {
	return this->state;
}

void Tray::fullReset() {
	this->trayMotor.tare_position();
	SystemManager::fullReset();
}

void Tray::update() {
    this->position = this->trayMotor.get_position();
  //  pros::lcd::print(5, "%f, %u, %f, %f", this->position, this->state, this->trayMotor.get_target_position(), this->trayMotor.get_target_velocity());
    this->error = this->target - this->position;

    switch(this->state) {
    case DISABLED_STATE:
        break;
    case RESET_STATE:
        break;
    case IDLE_STATE:
        break;
	case AUTON_LIFT:
		if(abs(this->error) < 100) {
            // intake.intake(-60);
		}
        if(this->timedOut(STACK_TIMEOUT) || abs(this->error)<20) {
        	this->stop();
		    // intake.stop();
            this->callback();
           	this->changeState(HOLD_STATE);
           	break;
        }
        if(stacking) {
            this->setPower(getPowerFunction(this->position));
        }
        this->trayMotor.move_velocity(this->power);
        break;

    case LIFT_STATE:
        if(abs(this->error) < 100) {
            // intake.reset();
        }
        if(this->timedOut(STACK_TIMEOUT) || abs(this->error)<20) {
            this->stop();
            this->callback();
            this->stacking = false;
            this->changeState(HOLD_STATE);
            break;
        }
        this->setPower(getPowerFunction(this->position));
        this->trayMotor.move_velocity(this->power);
        break;
    case LOWER_STATE:
        if(this->timedOut(LOWER_TIMEOUT) || abs(this->error)<20) {
            this->trayMotor.tare_position();
            this->stop();
            this->callback();
            this->changeState(IDLE_STATE);
            break;
        }
        this->setPower(getReversePowerFunction(this->position));
        this->trayMotor.modify_profiled_velocity(this->power);
        break;
    case HOLD_STATE:
        break;
    case OPERATOR_OVERRIDE:
        if(opUp == 0) {
            this->stop();
        } else {
            this->trayMotor.move_velocity(opUp == 1 ? OPERATOR_TRAY_VELOCITY : -OPERATOR_TRAY_VELOCITY);
        }
        break;
    }
}
