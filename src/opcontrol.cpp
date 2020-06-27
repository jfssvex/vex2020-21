#include "main.h"
#include "tracking.h"
#include "globals.h"
#include "systems/toggle.h"
#include "systems/intake.h"
#include "autonSelector.h"
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

void layStack(Intake intake, Tray& tray, Toggle& t) {
	tray.layCubes();
	while(t.checkState() != 0) {
		tray.update();
		if(master.getDigital(ControllerDigital::L2)) {
			tray_motor.move_velocity(70);
		}
		else if(master.getDigital(ControllerDigital::R2)) {
			tray_motor.move_velocity(-70);
		}
		else if(tray.getState() != 0x12 && tray.getState() != 0x11) {
			tray_motor.move_velocity(0);
		}

		// float traySpeed = joystickSlew(master.getAnalog(ControllerAnalog::rightY));
		// move({TRAY}, traySpeed*127);

		float driveSpeed = joystickSlew(master.getAnalog(ControllerAnalog::leftY));
		drive.tank(driveSpeed*0.5f, driveSpeed*0.5f, 0.05f);

		if(driveSpeed < 0) {
			intake.out(driveSpeed * 127 /2);
		}
		else {
			intake.reset();
			move({LINTAKE, RINTAKE}, 0);
		}
		pros::delay(5);
	}
}

std::string IntToStr(double i)
{
	std::ostringstream out;
	out << i;
	return out.str();
}

void processDrive(double straight, double turn) {
	// Absolute dead zones
	if(abs(straight) <= 0.05)
		straight = 0;
	if(abs(turn) <= 0.05)
		turn = 0;

	// Straight and turn dead zones
	if(straight / turn > DEAD_ZONE_TIGHTNESS)
		turn = 0;
	else if(turn / straight > DEAD_ZONE_TIGHTNESS)
		straight = 0;

	double leftSpeed = 0;
	double rightSpeed = 0;
	if(turn == 0) {
		leftSpeed = straight;
		rightSpeed = straight;
	}
	else if(straight == 0) {
		rightSpeed = -turn;
		leftSpeed = turn;
	}
	else {
		double magnitude = sqrt((turn*turn)+(straight*straight));
		if(straight < 0) {
			magnitude = -magnitude;
		} 
		// double ratio = turn/straight;
		leftSpeed = magnitude;
		rightSpeed = magnitude;
		leftSpeed += turn;
		rightSpeed -= turn;
	}
	frontRightDrive.move(rightSpeed * 127);
	backRightDrive.move(rightSpeed * 127);
	frontLeftDrive.move(leftSpeed * 127);
	backLeftDrive.move(leftSpeed * 127);
}

void opcontrol() {
	//lv_theme_t *a = lv_theme_alien_init(10, NULL);
	//lv_theme_set_current(a);
	/*lv_obj_t * img = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img, &royals);
	lv_obj_align(img, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);*/
	update.remove();
	intake.control();

	// Acceleration curving
	double lSpeed =0;
	double rSpeed = 0;
	double reqRSpeed = 0;
	double reqLSpeed = 0;

	// Speed limit
	float speed = 1.0f;

	// Toggle switches
	Toggle fullIntake = Toggle({ControllerDigital::L2, ControllerDigital::R2}, master);
	Toggle controlIntake = Toggle({ControllerDigital::R1}, master, true);
	Toggle engageTray = Toggle({ControllerDigital::L1}, master);
	Toggle liftButton = Toggle({ControllerDigital::Y}, master);
	/*pros::Vision andyVision(VISION_PORT);
	pros::vision_signature_s_t PURPLE[3];
	PURPLE[0] = pros::Vision::signature_from_utility(PURPLE_SIG, 2931, 3793, 3362, 5041, 6631, 5836, 4.800, 1);
	PURPLE[1] = pros::Vision::signature_from_utility(PURPLE_SIG2, 2227, 3669, 2948, 2047, 3799, 2923, 3.6, 0);*/
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
	if(in == 1) {
		intake.intake(INTAKE_SPEED);
	}
	else if(in == 0) {
		intake.stop();
	}

	// CONTROL INTAKE
	int control = controlIntake.checkState();
	if(control == 1) {
		intake.control();
	}
	else if(control == 0) {
		intake.stop();
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

	// TRAY 
	// if(tray.getState() == Tray::HOLD_STATE) {
	// 	if(master.getDigital(ControllerDigital::L2) && !stacking) {
	// 		tray.setOperatorControl(1);
	// 	}
	// 	else if(master.getDigital(ControllerDigital::R2) && !stacking) {
	// 		tray.setOperatorControl(-1);
	// 	}
	// 	else if(!stacking) {
	// 		tray.setOperatorControl(0);
	// 	}
	// }
	// int stack = engageTray.checkState();
	// if(stack == 1) {
	// 	stackCubes();
	// }
	// if(stack == 0) {
	// 	disengageStack();
	// }

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
	reqLSpeed = joystickSlew(master.getAnalog(ControllerAnalog::leftY));
	reqRSpeed = joystickSlew(master.getAnalog(ControllerAnalog::leftX));
	if (reqRSpeed > (rSpeed+accel))
		rSpeed+=accel;
	else if (reqRSpeed < (rSpeed-(accel*2)))
		rSpeed -= accel*2;
	else
		rSpeed = reqRSpeed;
	if (reqLSpeed > (lSpeed+accel))
		lSpeed+=accel;
	else if (reqLSpeed < (lSpeed-(accel*2)))
		lSpeed -= accel*2;
	else
		lSpeed = reqLSpeed;
	if(!suspendDrive)
		processDrive(lSpeed, rSpeed);
	
	// Diagnostics
	pros::delay(10);
	//}
		//pros::vision_object_s_t testCube = andyVision.get_by_sig(0, PURPLE_SIG2);
		//pros::lcd::print(5, "location of purple cube: %f", testCube.left_coord);
		//pros::lcd::print(5, "loc x  %f; loc y %f", x, y);
	}
}

double joystickSlew(double input) {
	return pow(input, 3);
}
