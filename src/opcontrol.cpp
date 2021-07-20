/**
 * @file src/opcontrol.cpp
 * 
 * Contains main operator control loop called from src/main.cpp.
 * Also has some helper functions to handle drive input.
 */

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

using namespace pros;
using namespace okapi;

std::vector<double> encoder;
extern float x;
extern float y;
extern float theta;

// Convert integer to string
std::string IntToStr(double i) {
	std::ostringstream out;
	out << i;
	return out.str();
}

// Drive the robot using the okapi controller and arcade control
// Not in use right now for some reason
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
	// Run display controller. Not working right now so its commented
	// if(display.getMode() == SELECTOR)
	// 	display.startMatchMode();
	// else
	// 	display.startMatchMode();

	// Set up state machines and remove update task from autonomous
	update.remove();
	intake.fullReset();
	rollers.fullReset();

	// Acceleration curving
	double lSpeed = 0;
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

	// Main loop
	while (1) {
	intake.update();
	rollers.update();

	// INTAKE
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

	// EJECTING (Clean shot)
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
		if(pros::millis() - shotTime > 30) {
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

	// The following few if/else statements were used to test tracking and motion algorithms
	// They don't have any actual application in operator control during matches/skills
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
		pros::delay(500);
	}

	// Joystick curve
	reqLSpeed = joystickSlew(master.getAnalog(ControllerAnalog::leftY));
	reqRSpeed = joystickSlew(master.getAnalog(ControllerAnalog::leftX));
	reqTSpeed = joystickSlew(master.getAnalog(ControllerAnalog::rightX));

	// Make sure drive isn't suspended by macros before driving
	if(!suspendDrive && !strafing)
		xDrive->xArcade(reqRSpeed, reqLSpeed, reqTSpeed);

	pros::delay(10);
	}
}

double joystickSlew(double input) {
	// Cube input for more natural control
	// Low end of the range becomes less sensitive, allowing more precise movements
	// High end still allows full speed
	return pow(input, 3);
}
