#pragma once

#include "main.h"
#include "systemManager.h"
#include "chassis.h"

#define INTAKE_HOLD_SPEED 80

class Intake : public SystemManager {
	public:
		static const uint8_t IDLE_STATE = 0x10;
		static const uint8_t IN_STATE = 0x11;
		static const uint8_t OUT_STATE = 0x12;
		static const uint8_t HOLD_STATE = 0x14;

		void intake(double _power);
		void out(double _power);
		void stop();
		void control();

		void update() override;
		void fullReset() override;

		Intake(uint8_t _defaultState, okapi::Controller _controller);
		int *getX();
		int x;

	private:

    void setPower(double _power);
    bool changeState(uint8_t newState) override;
	pros::Motor leftIntakeMotor = pros::Motor(LINTAKE_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor rightIntakeMotor = pros::Motor(RINTAKE_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
	okapi::Controller controller;
};
