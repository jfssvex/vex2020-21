#include "main.h"
#include "lift.h"

#define LIFT_SPEED 107
#define MAX_LIFT_HEIGHT 3200
#define LIFT_TIMEOUT 10000

// Constructor
Lift::Lift(uint8_t _defaultState) : SystemManager(_defaultState) {}

// Sub-class specific functions

void Lift::stop()
{
    this->power = 0;
    this->target = this->liftMotor.get_position();
    this->liftMotor.modify_profiled_velocity(0);
    this->liftMotor.move(0);
}

void Lift::moveTo(double _target, void(*_callback)()) {
    this->callback = _callback;
    bool success;
    if(_target > this->position) {
        success = this->changeState(LIFT_STATE);
    } else {
        success = this->changeState(LOWER_STATE);
    }
    if (!success) {
        return;
    }
    this->target = _target;
}

void Lift::setTarget(double _target)
{
    this->target = _target;
    this->liftMotor.move_absolute(this->target, this->power);
}

// Overrides
bool Lift::changeState(uint8_t newState)
{
    if(lockState && (newState != RESET_STATE || newState != HOLD_STATE)) {
        return false;
    }
    bool processed = SystemManager::changeState(newState);
    if (!processed)
    {
        return false;
    }

    switch (newState)
    {
    case DISABLED_STATE:
        this->stop();
        break;
    case RESET_STATE:
        this->changeState(IDLE_STATE);
        break;
    case IDLE_STATE:
        this->stop();
        liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        break;
    case LIFT_STATE:
        // this->tray.setTargetPowerControl(1000, 100);
        break;
    case LOWER_STATE:
        this->liftMotor.set_brake_mode(MOTOR_BRAKE_COAST);
        break;
    case HOLD_STATE:
        this->target = this->liftMotor.get_position();
        this->stop();
        this->liftMotor.set_brake_mode(MOTOR_BRAKE_HOLD);
        break;
    }

    return true;
}

uint8_t Lift::getState()
{
    return this->state;
}

void Lift::setCallback(void(*_callback)()) {
    this->callback = _callback;
}

void Lift::update()
{
    this->position = this->liftMotor.get_position();
    //pros::lcd::print(5, "%f, %u, %f, %f", this->position, this->state, this->liftMotor.get_target_position(), this->liftMotor.get_target_velocity());
    this->error = this->target - this->position;

    switch (this->state)
    {
    case DISABLED_STATE:
        break;
    case RESET_STATE:
        break;
    case IDLE_STATE:
        break;
    case LIFT_STATE:
        if(timedOut(LIFT_TIMEOUT)) {
            this->reset();
            this->callback();
        }

        this->liftMotor.move(127);
        if(this->position > target-10) {
            this->callback();
            this->lock();
        }
        break;
    case LOWER_STATE:
        if(timedOut(LIFT_TIMEOUT)) {
            this->reset();
            this->callback();
        }
        this->liftMotor.move(-100);
        if(this->position < 250) {
            // this->tray.setTargetPowerControl(0, 127);
        }
        if(this->position < target+10) {
            this->callback();
            this->reset();
        }
        break;
    case HOLD_STATE:
        if(abs(this->position - this->target) > 40) {
            this->liftMotor.move_absolute(this->target, 100);
        }
        break;
    }
}

void Lift::move(bool up) {
    bool success;
    if(up) {
        success = this->changeState(LIFT_STATE);
    } else {
        success = this->changeState(LOWER_STATE);
    }
    if (!success) {
        return;
    }
    this->target = up ? MAX_LIFT_HEIGHT : 0.0f;
}

void Lift::lock() {
    if(this->state == HOLD_STATE) {
        return;
    }
    this->changeState(HOLD_STATE);
}

void Lift::fullReset() {
    this->liftMotor.tare_position();
    SystemManager::fullReset();
}
