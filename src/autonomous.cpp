#include "main.h"
#include "globals.h"
#include "macros.h"
#include "tracking.h"
#include "control.h"

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
		rollers.update();
		pros::delay(5);
	}
}

double radToDeg(double r) {
	return r * 180 / PI;
}

void myAutonomous() {
	update = pros::Task(updateSysMan, (void *)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Update system manager");
	rollers.fullReset();
	intake.fullReset();

	flipout();
	pros::delay(400);
	in();
	strafeToPoint(Vector2(24, 18));
	stopRollers();
	strafeToOrientation(Vector2(6.5, 5.5), 135);
	shootStaggered();
	pros::delay(800);
	stopRollers();
	strafeToPoint(Vector2(24, 18));
	Vector2 secondBall(36 + 24, 38);
	turnToAngle(radToDeg((secondBall-trackingData.getPos()).getAngle()) - 90);
	in();
	strafeToPoint(secondBall);
	pros::delay(400);
	stopRollers();
	turnToAngle(-185);
	strafeToPoint(Vector2(56, 10));
	shootStaggered();
	pros::delay(1200);
	stopRollers();
	strafeToPoint(secondBall);
	Vector2 thirdBall(secondBall.getX() + 36, secondBall.getY() - 22);
	turnToAngle(radToDeg((thirdBall - trackingData.getPos()).getAngle()) - 90);
	in();
	strafeToPoint(thirdBall);
	pros::delay(400);
	stopRollers();
	turnToAngle(-135);
	strafeToOrientation(Vector2(110, -2), -135);
	shootStaggeredIntake();
	pros::delay(1000);
	stopRollers();

    strafeToPoint(trackingData.getPos() + Vector2(-20, 20));
    in();
    strafeToOrientation(Vector2(102.5, 45), -10);
    strafeToPoint(Vector2(102.5, 56));
    turnToAngle(-90);
    stopRollers();
    strafeToPoint(Vector2(110, 56));
    shootStaggered();
    pros::delay(600);
    stopRollers();
    strafeToPoint(trackingData.getPos() + Vector2(-20, 0));

    Vector2 topRightBall = thirdBall + Vector2(0, 86);
    in();
    strafeToOrientation(topRightBall, 0);
    turnToAngle(-50);
    stopRollers();
    strafeToPoint(topRightBall + Vector2(20, 10));
    shootStaggered();
    pros::delay(600);
    stopRollers();


	//Auto routine
	// rollers.shoot();
	// pros::delay(1500);

	rollers.reset();
	intake.reset();
	update.remove();
}
