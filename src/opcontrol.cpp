#include "main.h"
#include "tracking.h"
#include "globals.h"
#include "systems/toggle.h"
#include "systems/intake.h"
#include "globals.h"
#include "macros.h"
#include "motionControl.h"
#include <initializer_list>
#include <string>
#include <sstream>
LV_IMG_DECLARE(royals);

#define DEAD_ZONE_TIGHTNESS 100

double const accel = 0.045;
/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

using namespace pros;
using namespace okapi;

std::vector<double> encoder;
extern float x;
extern float y;
extern float theta;

std::string IntToStr(double i)
{
	std::ostringstream out;
	out << i;
	return out.str();
}

void processDrive(double straight, double strafe, double turn) {
	// Absolute dead zones
	if(abs(straight) <= 0.05)
		straight = 0;
	if (abs(strafe) <= 0.05)
		strafe = 0;
	if(abs(turn) <= 0.05)
		turn = 0;
	
	xDrive->xArcade(strafe, straight, turn);
}

void myOPControl() {
	// if(display.getMode() == SELECTOR)
	// 	display.startMatchMode();
	// else
	// 	display.startMatchMode();

	update.remove();
	intake.fullReset();
	rollers.fullReset();

	// Acceleration curving
	double lSpeed =0;
	double rSpeed = 0;
	double tSpeed = 0;
	double reqRSpeed = 0;
	double reqLSpeed = 0;
	double reqTSpeed = 0;

	// Toggle switches
	Toggle fullIntake = Toggle({ControllerDigital::R1}, master);
	Toggle fullOuttake = Toggle({ControllerDigital::L2}, master);

	bool L1Pressed = false;

	double shotTime = 0;

	while (1) {
	intake.update();
	rollers.update();

	// INTAKE
	// OUTTAKE
	if(master.getDigital(ControllerDigital::R1)) {
		if (rollers.getState() != Rollers::IN_STATE) {
			rollers.intake();
			intake.intake(127);
		}
	}
	else if(rollers.getState() == Rollers::IN_STATE) {
		rollers.stop();
		intake.stop();
	}

	// OUTTAKE
	if(master.getDigital(ControllerDigital::L1)) {
		if (rollers.getState() != Rollers::OUT_STATE) {
			rollers.out();
			intake.out(127);
		}
	}
	else if(rollers.getState() == Rollers::OUT_STATE) {
		rollers.revertState();
		intake.stop();
	}

	// EJECTING
	if(master.getDigital(ControllerDigital::L2)) {
		if (rollers.getState() != Rollers::EJECT_STATE)
			rollers.eject();
	}
	else if(rollers.getState() == Rollers::EJECT_STATE) {
		rollers.revertState();
	}

	// SHOOTING
	if(master.getDigital(ControllerDigital::R2)) {
		if(rollers.getState() != Rollers::SHOOT_STATE) {
			rollers.shoot();
			shotTime = pros::millis();
		}
		if(pros::millis() - shotTime > 150) {
			intake.intake(127);
		}
	}
	else if (master.getDigital(ControllerDigital::X)) {
		if (rollers.getState() != Rollers::SHOOT_STATE) {
			rollers.shoot();
		}
		intake.stop();
	}
	else if (rollers.getState() == Rollers::SHOOT_STATE) {
		rollers.revertState();
		intake.stop();
	}

	bool strafing = false;
	if(master.getDigital(ControllerDigital::up)) {
		// strafe(Vector2(1, 1), 0);
		strafeToPoint(Vector2(0, 0));
		strafing = true;
	}
	else {
		strafing = false;
	}

	if (master.getDigital(ControllerDigital::right))
	{
		// strafe(Vector2(1, 1), 0);
		turnToAngle(90);
	}
	if(master.getDigital(ControllerDigital::down)) {
		strafeToOrientation(Vector2(0, 0), 0);
	}
	if(master.getDigital(ControllerDigital::left)) {
		rollers.flipout();
	}

	// Acceleration curve
	reqLSpeed = joystickSlew(master.getAnalog(ControllerAnalog::leftY));
	reqRSpeed = joystickSlew(master.getAnalog(ControllerAnalog::leftX));
	reqTSpeed = joystickSlew(master.getAnalog(ControllerAnalog::rightX));

	if(!suspendDrive && !strafing)
		xDrive->xArcade(reqRSpeed, reqLSpeed, reqTSpeed);

	pros::delay(10);
	}
}

double joystickSlew(double input) {
	return pow(input, 3);
}
