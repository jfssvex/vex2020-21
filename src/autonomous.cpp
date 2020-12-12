#include "main.h"
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

void updateSysMan(void* param) {
	while(1) {
		intake.update();
		tray.update();
		lift.update();
		pros::delay(5);
	}
}

void myAutonomous() {
	update = pros::Task(updateSysMan, (void *)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Update system manager");
	drive->setMaxVelocity(120);
	rollers.fullReset();
	intake.fullReset();

	//Auto routine
	rollers.shoot();
	pros::delay(1500);
		
	rollers.reset();
	intake.reset();
	update.remove();
}
