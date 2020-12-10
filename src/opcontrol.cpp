#include "main.h"
#include "tracking.h"
#include "globals.h"
#include "systems/toggle.h"
#include "systems/intake.h"
#include "systems/tray.h"
#include "systems/lift.h"
#include "globals.h"
#include "macros.h"
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
	// 	display.startDebugMode();

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
	Toggle fullOuttake = Toggle({ControllerDigital::L1}, master);

	bool L1Pressed = false;
	//flipout();

	while (1) {
	intake.update();
	rollers.update();

	if(master.getDigital(ControllerDigital::right)) {
		turnToAngle(90);
	}

	// INTAKE
	bool inHeld = fullIntake.getHeld();
	int in = fullIntake.checkState();
	if((in == 1 || in == 0) && inHeld) {
		if(intake.getState() == Intake::IN_STATE) {
			intake.stop();
			rollers.stop();
		}
		else {
			intake.intake(INTAKE_SPEED);
			rollers.intake();
		}
	}

	// OUTTAKE
	bool outHeld = fullIntake.getHeld();
	int out = fullOuttake.checkState();
	if((out == 1 || out == 0) && outHeld) {
		if (intake.getState() == Intake::OUT_STATE) {
			intake.stop();
			rollers.stop();
		}
		else {
			intake.out(INTAKE_SPEED);
			rollers.out();
		}
	}

	// EJECTING
	if(master.getDigital(ControllerDigital::L1)) {
		rollers.eject();
	}
	else if(rollers.getState() == Rollers::EJECT_STATE) {
		rollers.revertState();
	}

	// SHOOTING
	if(master.getDigital(ControllerDigital::R2)) {
		rollers.shoot();
	}
	else if (master.getDigital(ControllerDigital::X)) {
		rollers.shoot();
		intake.stop();
	}
	else if (rollers.getState() == Rollers::SHOOT_STATE) {
		rollers.revertState();
	}

	// Acceleration curve
	reqLSpeed = joystickSlew(master.getAnalog(ControllerAnalog::leftY));
	reqRSpeed = joystickSlew(master.getAnalog(ControllerAnalog::leftX));
	reqTSpeed = joystickSlew(master.getAnalog(ControllerAnalog::rightX));
	// if (reqRSpeed > (rSpeed+accel))
	// 	rSpeed+=accel;
	// else if (reqRSpeed < (rSpeed-(accel*2)))
	// 	rSpeed -= accel*2;
	// else
	// 	rSpeed = reqRSpeed;
	// if (reqLSpeed > (lSpeed+accel))
	// 	lSpeed+=accel;
	// else if (reqLSpeed < (lSpeed-(accel*2)))
	// 	lSpeed -= accel*2;
	// else
	// 	lSpeed = reqLSpeed;
	// if (reqTSpeed > (tSpeed + accel))
	// 	tSpeed += accel;
	// else if (reqTSpeed < (tSpeed - (accel * 2)))
	// 	tSpeed -= accel * 2;
	// else
	// 	tSpeed = reqTSpeed;
	// if(!suspendDrive)
		// processDrive(lSpeed, rSpeed, tSpeed);

	if(!suspendDrive)
		xDrive->xArcade(reqRSpeed, reqLSpeed, reqTSpeed);

	// Diagnostics
	pros::delay(10);
	}
}

double joystickSlew(double input) {
	return pow(input, 3);
}
