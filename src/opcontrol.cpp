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
	if(display.getMode() == SELECTOR)
		display.startMatchMode();
	else
		display.startDebugMode();

	update.remove();
	intake.control();

	display.logMessage("Hey", LOG);
	display.logMessage("Bruh", WARNING);
	display.logMessage("Error", ERROR);

	// Acceleration curving
	double lSpeed =0;
	double rSpeed = 0;
	double tSpeed = 0;
	double reqRSpeed = 0;
	double reqLSpeed = 0;
	double reqTSpeed = 0;

	// Speed limit
	float speed = 1.0f;

	// Toggle switches
	Toggle fullIntake = Toggle({ControllerDigital::R1}, master);
	Toggle fullOuttake = Toggle({ControllerDigital::L1}, master);
	Toggle engageTray = Toggle({ControllerDigital::L1}, master);
	Toggle liftButton = Toggle({ControllerDigital::Y}, master);
	int lastEncoder = getEncoders({TRAY})[0];
	tray.reset();
	lift.reset();

	bool L1Pressed = false;
	//flipout();

	while (1) {
	pros::lcd::print(2, "DRIVE: %f", frontLeftDrive.get_actual_velocity());
	pros::lcd::print(3, "%d", backTrackingWheel.get_value());
	tray.update();
	intake.update();
	lift.update();
	encoder = getEncoders({LIFT, TRAY});

	if (abs(getEncoders({TRAY})[0] - lastEncoder) > 5)
	{
		master.setText(0, 0, IntToStr(getEncoders({TRAY})[0]));
		lastEncoder = getEncoders({TRAY})[0];
	}

	if(master.getDigital(ControllerDigital::right)) {
		turnToAngle(90);
	}

	// INTAKE
	int in = fullIntake.checkState();
	if(in == 1 || in == 0) {
		if(intake.getState() == Intake::IN_STATE) {
			intake.stop();
		}
		else {
			intake.intake(INTAKE_SPEED);
		}
	}

	// OUTTAKE
	int out = fullOuttake.checkState();
	if (out == 1 || out == 0) {
		if (intake.getState() == Intake::OUT_STATE) {
			intake.stop();
		}
		else {
			intake.out(INTAKE_SPEED);
		}
	}

	int liftIterate = 0;
	if(master.getDigital(ControllerDigital::X)) {
		//tray.moveTo(100);
		liftIterate = 1;
	}
	else if(master.getDigital(ControllerDigital::B)) {
		liftIterate = -1;
	}
	incrementLift(liftIterate);

	if(master.getDigital(ControllerDigital::Y)) {
		liftToLow();
	}
	if(master.getDigital(ControllerDigital::A)) {
		liftToMid();
	}
	if(master.getDigital(ControllerDigital::down)) {
		dropLift();
	}

	if(master.getDigital(ControllerDigital::L1) && !L1Pressed) {
		L1Pressed = true;
		if(trayUp) {
			disengageStack();
		}
		else {
			halfStack();
		}
	}
	else if(!master.getDigital(ControllerDigital::L1) && L1Pressed && tray.getState() != Tray::LOWER_STATE) {
		L1Pressed = false;
		if(trayUp) {
			stackCubes();
		}
	}

	if(master.getDigital(ControllerDigital::L2)) {
		dropTray();
	}

	// Acceleration curve
	// reqLSpeed = joystickSlew(master.getAnalog(ControllerAnalog::leftY));
	// reqRSpeed = joystickSlew(master.getAnalog(ControllerAnalog::leftX));
	// reqTSpeed = joystickSlew(master.getAnalog(ControllerAnalog::rightX));
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
