/**
 * @file include/systems/rollers.h
 * 
 * Definition for the Rollers state machine for Change Up.
 * Inherits from SystemManager for basic state machine functionality. 
 */

#pragma once

#include "main.h"
#include "systemManager.h"
#include "chassis.h"

#define DEFAULT_ROLLER_SPEED 127
#define ROLLER_HOLD_SPEED 80

class Rollers : public SystemManager
{
public:
    // Roller-specific state codes
    static const uint8_t IDLE_STATE = 0x10;     // Do nothing, motor power = 0
    static const uint8_t IN_STATE = 0x11;       // Pull balls up from the intakes to shooting position
    static const uint8_t OUT_STATE = 0x12;      // Eject balls towards the intake
    static const uint8_t EJECT_STATE = 0x13;    // Originally used to eject balls out back flap, now used to shoot without second ball-delay
    static const uint8_t SHOOT_STATE = 0x15;    // Shoot with second ball-delay (avoids balls from interfering with each other)
    static const uint8_t HOLD_STATE = 0x16;     // Hold rollers in place
    static const uint8_t FLIPOUT_STATE = 0x17;  // Move rollers in flipout routine. Runs stop() after routine completes.

    /**
     * @brief Set state to intake balls
     */
    void intake();

    /**
     * @brief Set state to eject balls out front
     */
    void out();

    /**
     * @brief Set state to shoot balls cleanly. No second ball-delay.
     * More effective for shooting with only 1 ball in the robot.
     */
    void eject();

    /**
     * @brief Set state to shoot balls with second ball-delay.
     * More effective for shooting with 2+ balls in the robot.
     */
    void shoot();

    /**
     * @brief Stop the rollers
     */
    void stop();

    /**
     * @brief Run flipout routine and stop afterwards
     */
    void flipout();

    /**
     * @brief Update system based on current state.
     * Should be called regularly
     */
    void update() override;

    /**
     * @brief Reset the rollers to default state (idle) and
     * tare all encoders
     * 
     */
    void fullReset() override;

    /**
     * @brief Construct a new Rollers object
     * 
     * @param _defaultState the default state for the state machine
     * @param _controller the V5 controller to use for driver input
     */
    Rollers(uint8_t _defaultState, okapi::Controller _controller);

private:
    /**
     * @brief Drive roller motors at given power in same direction
     * 
     * @param _power power to apply (127-scale)
     */
    void setPower(double _power);

    /**
     * @brief Set the brake modes for both roller motors
     * 
     * @param mode new brake mode
     */
    void setBrakeMode(enum pros::motor_brake_mode_e mode);

    /**
	 * @brief Change the current state of the rollers state machine
	 * 
	 * @param newState code of the state to change to
	 * @return true if the state change went through
	 * @return false if the state change was blocked
	 */
    bool changeState(uint8_t newState) override;

    int topRollerPos = 0;       // Tracks top roller encoder position and prevent top ball from being pushed
    bool resetRollerPos = true; // Resets topRollerPos when true (i.e. when shooting)

    // Assosciated motors
    pros::Motor botRollerMotor = pros::Motor(BOTROLLER_PORT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
    pros::Motor topRollerMotor = pros::Motor(TOPROLLER_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

    // Controller for operator override state (deprecated)
    okapi::Controller controller;
};
