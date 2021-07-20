/**
 * @file include/systems/systemManager.h
 * 
 * Contains definition for SystemManager class that acts as the
 * parent for all state machines. Includes basic stat
 */

#pragma once

#include "main.h"

class SystemManager {
public:
    /**
     * @brief Construct a new System Manager object
     * 
     * @param _defaultState the default state for the state machine
     */
    SystemManager(uint8_t _defaultState);

    // Get Functions
    /**
     * @brief Get the current target.
     * For state machines centered around PID-controller mechanisms (i.e. lifts)
     * 
     * @return double - the current target
     */
    double getTarget();

    /**
     * @brief Get the current error (target - position).
     * For state machines centered around PID-controller mechanisms (i.e. lifts)
     * 
     * @return double - the current error
     */
    double getError();

    /**
     * @brief Get the current position.
     * For state machines centered around PID-controller mechanisms (i.e. lifts)
     * 
     * @return double - the current position
     */
    double getPosition();

    /**
     * @brief Get the current power applied to the main motors
     * 
     * @return double - the current power
     */
    double getPower();

    /**
     * @brief Get the system's current state
     * 
     * @return uint8_t - the state code
     */
    uint8_t getState();

    /**
     * @brief Sets state to reset. The reset state's actions are
     * implemented by child state machines.
     */
    void reset();

    /**
     * @brief Check if the state machine is enabled
     * 
     * @return true if the system is enabled
     * @return false if the system is disabled
     */
    bool enabled();

    /**
     * @brief Disable the state machine. This disallows state changes
     * until enable() is called.
     */
    void disable();

    /**
     * @brief Change state to the most recent one
     */
    void revertState();
    
    /**
     * @brief Enable the state machine. Reallows state changes
     * after the machine has been disabled.
     */
    void enable();
    
    /**
     * @brief Updates the system based on its current state. Should
     * be called regularly. Functionality for specific states is implemented
     * by child state machines.
     */
    virtual void update() {};

    /**
     * @brief Reset all PID and power related variables. Other reset
     * routines can be implemented by child state machines.
     */
    virtual void fullReset();

    // Universal operator override state to allow driver control. Every state machine should implement this.
    static const uint8_t OPERATOR_OVERRIDE = 0x20;

protected:
    // PID-related properties
    double target;
    double error;
    double position;

    double power;                   // Current power applied to main motors
    uint32_t timeOfLastChange = 0;  // pros::millis() time when last state change occurred

    static const uint8_t DISABLED_STATE = 0x00; // Disables machine and blocks state changes
    static const uint8_t RESET_STATE = 0x01;    // Implemented by children to perform reset routines
    uint8_t lastState = DISABLED_STATE;         // Code of the most recent state (defaults to disabled)
    uint8_t state = DISABLED_STATE;             // Code of current state
    uint8_t defaultState;                       // Default state set in constructor

    /**
	 * @brief Change the current state of the state machine.
     * Children should call the base definition then implement a switch
     * statement if the base returns true.
	 * 
	 * @param newState code of the state to change to
	 * @return true if the state change went through
	 * @return false if the state change was blocked
	 */
    virtual bool changeState(uint8_t newState);

    /**
     * @brief Checks if time in the current state has exceeded a given timeout.
     * Can be used to detect failure or abort inconsistent routines.
     * 
     * @param timeout the time limit for the current state
     * @return true if elapsed time has exceeded the timeout
     * @return false if elapsed time is within the timout 
     */
    bool timedOut(uint32_t timeout);
};
