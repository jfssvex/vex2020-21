/**
 * @file src/systems/rollers.cpp
 * 
 * Contains Rollers state machine definitions for Change Up
 */

#include "rollers.h"
#include "main.h"

// Subclass-specific functions

void Rollers::setPower(double _power) {
    // Uptake rollers' direction of motion are aligned
    this->power = _power;
    this->topRollerMotor.move(this->power);
    this->botRollerMotor.move(this->power);
}

void Rollers::intake() {
    this->changeState(IN_STATE);
}

void Rollers::flipout() {
    this->changeState(FLIPOUT_STATE);
}

void Rollers::out() {
    this->changeState(OUT_STATE);
}

void Rollers::eject() {
    this->changeState(EJECT_STATE);
}

void Rollers::shoot() {
    this->changeState(SHOOT_STATE);
}

void Rollers::stop() {
    this->changeState(IDLE_STATE);
}

void Rollers::setBrakeMode(enum pros::motor_brake_mode_e mode) {
    // Not sure why top is always set to brake but I guess it made sense at some point
    this->topRollerMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    this->botRollerMotor.set_brake_mode(mode);
}

Rollers::Rollers(uint8_t _defaultState, okapi::Controller _controller) : SystemManager(_defaultState) {
    // Set controller after calling base constructor
    this->controller = _controller;
}

// Overrides
void Rollers::update() {
    this->position = (this->topRollerMotor.get_position() + this->botRollerMotor.get_position()) / 2;
    this->error = this->position - this->target;

    switch(this->state) {
        case IDLE_STATE:
            break;
        case IN_STATE: {
            // Used to be used to stop top ball from getting pushed up into the top roller. No longer an issue in newer versions
            int newPos = this->topRollerMotor.get_position();
            // if(abs(newPos - this->topRollerPos) > 40)
            //     this->botRollerMotor.move(0);
            break; }
        case OUT_STATE:
            break;
        case SHOOT_STATE:
            // Delay running the bottom roller so the second ball doesn't collide with the first
            if(pros::millis() - this->timeOfLastChange > 400) {
                this->botRollerMotor.move(DEFAULT_ROLLER_SPEED);
            }
            // Run the bottom roller very shortly at the beginning to get the first ball unstuck if its in the pocket
            else if(pros::millis() - this->timeOfLastChange > 50) {
                this->botRollerMotor.move(0);
            }
            break;
        case FLIPOUT_STATE:
            // Flipout only lasts ~250 ms. Stop after that's complete
            if(pros::millis() - this->timeOfLastChange > 250) {
                this->stop();
            }
            break;
        case OPERATOR_OVERRIDE: {
            // Update speed with operator input if necessary
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
    // Run basic state change code in base function
    bool processed = SystemManager::changeState(newState);
    if(!processed) {
        return false;
    }

    switch(newState) {
        case IDLE_STATE:
            // Stop motors and leave limp
            this->setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
            this->setPower(0);
            break;
        case IN_STATE: {
            // Intake balls but don't shoot
            // Also contains code to keep top ball from moving excessively which is no longer necessary
            if(this->resetRollerPos)
                this->topRollerPos = this->topRollerMotor.get_position();
            this->resetRollerPos = false;
            this->setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
            this->topRollerMotor.move(0);
            this->botRollerMotor.move(DEFAULT_ROLLER_SPEED);
            int newPos = this->topRollerMotor.get_position();
            // if (abs(newPos - this->topRollerPos) > 40)
            //     this->botRollerMotor.move(0);

            break; }
        case OUT_STATE:
            // Eject balls out the front
            this->setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
            this->topRollerMotor.move(-DEFAULT_ROLLER_SPEED);
            this->botRollerMotor.move(-DEFAULT_ROLLER_SPEED);
            break;
        case EJECT_STATE:
            // Shoot balls without the delay from update()
            this->setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
            this->topRollerMotor.move(DEFAULT_ROLLER_SPEED);
            this->botRollerMotor.move(DEFAULT_ROLLER_SPEED);
            break;
        case SHOOT_STATE:
            // Shoot balls but there's a delay in update()
            this->resetRollerPos = true;
            this->setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
            this->topRollerMotor.move(DEFAULT_ROLLER_SPEED);
            this->botRollerMotor.move(DEFAULT_ROLLER_SPEED);
            break;
        case FLIPOUT_STATE:
            // Run the motors in the directions for the hood + intake flipouts
            this->topRollerMotor.move(DEFAULT_ROLLER_SPEED);
            this->botRollerMotor.move(-DEFAULT_ROLLER_SPEED);
        case OPERATOR_OVERRIDE:
            // Stop rollers temporarily if they're running when operator takes over
            this->setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
            if (this->topRollerMotor.get_actual_velocity() > 20) {
                this->setPower(0);
            }
            break;
        case HOLD_STATE:
            // Set brakes to hold and ensure using move_absolute and tared encoders
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
            this->setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
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
