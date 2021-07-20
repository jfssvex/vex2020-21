/**
 * @file include/systems/intake.h
 * 
 * Definition for the Intake state machine for Change Up.
 * Inherits from SystemManager for basic state machine functionality. 
 */

#pragma once

#include "main.h"
#include "systemManager.h"
#include "chassis.h"

#define INTAKE_HOLD_SPEED 80 // Used in TT to keep cubes from sliding out when motors went limp

class Intake : public SystemManager {
	public:
		// Intake-specific state codes
		static const uint8_t IDLE_STATE = 0x10; // Do nothing, motor power = 0
		static const uint8_t IN_STATE = 0x11; 	// Intake balls
		static const uint8_t OUT_STATE = 0x12;	// Eject balls
		static const uint8_t HOLD_STATE = 0x14;	// Hold intakes in place (idle but don't allow freespin)

		/**
		 * @brief Set the system's state to start intaking balls
		 * 
		 * @param _power the power (127-scale) to apply to intake motors
		 */
		void intake(double _power);

		/**
		 * @brief Set the system's state to start ejecting balls
		 * 
		 * @param _power the power (127-scale) to apply to intake motors (set to negative to outtake)
		 */
		void out(double _power);

		/**
		 * @brief Stop the intakes
		 */
		void stop();

		/**
		 * @brief Set the intakes to take input from controller.
		 * Used in tower takeover season where intakes were controlled
		 * by the right stick y-axis
		 */
		void control();

		/**
		 * @brief Update system based on current state.
		 * Should be called regularly
		 */
		void update() override;

		/**
		 * @brief Reset the intakes to default state (idle) and
		 * tare all encoders.
		 */
		void fullReset() override;

		/**
		 * @brief Construct a new Intake object
		 * 
		 * @param _defaultState the default state for the state machine
		 * @param _controller the V5 controller to use for driver input
		 */
		Intake(uint8_t _defaultState, okapi::Controller _controller);

		/**
		 * @brief Function pointer used to test display messages
		 * 
		 * @return int* a reference to the property x
		 */
		int *getX();
		int x;

	private:

	/**
	 * @brief Apply power to the intake motors
	 * 
	 * @param _power power to apply (127-scale)
	 */
    void setPower(double _power);

	/**
	 * @brief Change the current state of the intake state machine
	 * 
	 * @param newState code of the state to change to
	 * @return true if the state change went through
	 * @return false if the state change was blocked
	 */
    bool changeState(uint8_t newState) override;

	// Assosciated motors
	pros::Motor leftIntakeMotor = pros::Motor(LINTAKE_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor rightIntakeMotor = pros::Motor(RINTAKE_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

	// Controller for control state
	okapi::Controller controller;
};
