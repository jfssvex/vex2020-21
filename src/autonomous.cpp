#include "main.h"
#include "autonSelector.h"
#include "globals.h"
#include "macros.h"
#include "tracking.h"

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

using namespace pros;
using namespace okapi;
extern int auton_id;

/*const int { +FL, +BL } = 1;
const int { -FR, -BR } = 2;
const auto WHEEL_DIAMETER = 4_in;
const auto CHASSIS_WIDTH = 11.5_in;
*/

void updateSysMan(void* param) {
	while(1) {
		intake.update();
		tray.update();
		lift.update();
		pros::delay(5);
	}
}

void myAutonomous() {
	//autonSelector();
	//deleteSelector();
	tray.fullReset();
	//setAutonId(AUTON_BLUE_PROTECTED);
	if(getAutonId() == FLIPOUT) {
		flipout();
		drive->setMaxVelocity(75);
		drive->moveDistance(-0.3_m);
		drive->moveDistance(0.3_m);
		drive->setMaxVelocity(600);
		pros::delay(1000);
	}
	else {
		//flipout();
	}
	update = pros::Task(updateSysMan, (void *)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Update system manager");
	drive->setMaxVelocity(120);
	if (getAutonId() == FLIPOUT)
		;
	else if (getAutonId() == AUTON_BLUE_UNPROTECTED) {
		flipout();
		drive->setMaxVelocity(80);
		drive->moveDistanceAsync(0.80_m);
		intake.intake(100);
		drive->waitUntilSettled();
		pros::delay(400);
		intake.stop();
		drive->setMaxVelocity(60);
		drive->turnAngle(-107_deg);
		drive->setMaxVelocity(120);
		drive->moveDistance(0.87_m);
		intake.out(-50);
		pros::delay(400);
		intake.reset();
		stackCubes();
		while(tray.getTrayState() == tray.LIFT_STATE) {
			if (pros::millis() <= 14800)
			{
				pros::lcd::print(1, "Hello");
				pros::delay(2);
			}
			else
				break;
		}
		intake.stop();
		pros::delay(500);
		intake.out(-60);
		drive->setMaxVelocity(70);
		drive->moveDistance(-0.3_m);
		intake.reset();
	} else if(getAutonId() == AUTON_RED_UNPROTECTED) {
		flipout();
		drive->setMaxVelocity(80);
		drive->moveDistanceAsync(0.80_m);
		intake.intake(100);
		drive->waitUntilSettled();
		pros::delay(400);
		intake.stop();
		drive->setMaxVelocity(60);
		drive->turnAngle(107_deg);
		drive->setMaxVelocity(120);
		drive->moveDistance(0.87_m);
		intake.out(-50);
		pros::delay(400);
		intake.reset();
		stackCubes();
		while(tray.getTrayState() == tray.LIFT_STATE) {
			if (pros::millis() <= 14700)
			{
				pros::lcd::print(1, "Hello");
				pros::delay(2);
			}
			else
				break;
		}
		intake.stop();
		pros::delay(500);
		// intake.out(-60);
		// drive->setMaxVelocity(70);
		// drive->moveDistance(-0.3_m);
		disengageStack();
		//intake.reset();
	} else if (getAutonId() == SIMPLE_ONE_CUBE) {
		drive->setMaxVelocity(140);
		drive->moveDistance(0.08_m);
		intake.out(-127);
		drive->moveDistance(-0.12_m);
		drive->waitUntilSettled();
		intake.stop();
	} else if (getAutonId() == AUTON_BLUE_PROTECTED) {
		drive->setMaxVelocity(130);
		drive->moveDistance(0.1_m);
		drive->moveDistance(-0.1_m);
		flipout();
		drive->setMaxVelocity(130);
		intake.intake(120);
		drive->moveDistance(0.4_m);
		pros::delay(200);
		drive->setMaxVelocity(80);
		drive->turnAngle(70_deg);
		drive->setMaxVelocity(130);
		drive->moveDistance(0.5_m);
		pros::delay(300);
		intake.stop();
		drive->setMaxVelocity(80);
		drive->turnAngle(25_deg);
		drive->moveDistance(0.2_m);
		intake.out(-50);
		pros::delay(400);
		intake.reset();
		stackCubes();
		while (tray.getTrayState() == 0x11)
		{
			if (pros::millis() <= 14500) {
				pros::lcd::print(1, "Hello");
				pros::delay(2);
			} else {
				drive->moveDistance(-0.3_m);
			}
		}
		pros::lcd::print(1, "Hello again");
		disengageStack();
	} else if (getAutonId() == AUTON_RED_PROTECTED) {
		drive->setMaxVelocity(130);
		drive->moveDistance(0.3_m);
		drive->moveDistance(-0.3_m);
		flipout();
		drive->setMaxVelocity(80);
		intake.intake(120);
		drive->moveDistance(0.4_m);
		pros::delay(200);
		drive->setMaxVelocity(60);
		drive->turnAngle(-90_deg);
		drive->setMaxVelocity(80);
		drive->moveDistanceAsync(0.5_m);
		pros::delay(300);
		drive->setMaxVelocity(60);
		drive->turnAngle(90_deg);
		drive->moveDistance(0.3_m);
		stackCubes();
		while (tray.getTrayState() == 0x11)
		{
			if (pros::millis() <= 14500) {
				pros::lcd::print(1, "Hello");
				pros::delay(2);
			} else {
				drive->moveDistance(-0.3_m);
			}
		}
		pros::lcd::print(1, "Hello again");
		disengageStack();
	}
	intake.reset();
	update.remove();
	///
		/*
	*Possible auton path number 1
	chassis.moveDistance(2_ft);

	chassis.turnAngle(135_deg);

	chassis.moveDistance(2_ft);

	chassis.turnAngle(-135_deg);

	chassis.moveDistance(2_ft);

	chassis.turnAngle(-135_deg);

	chassis.moveDistance(3_ft);
	*/

	/*
	*Possible auton path number 2
	chassis.moveDistance(2_ft);

	chassis.moveDistance(-2_ft);

	chassis.turnAngle(-90_deg);

	chassis.moveDistance(1_ft);
	*/
}
