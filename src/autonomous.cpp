#include "main.h"
#include "globals.h"
#include "macros.h"
#include "tracking.h"
#include "motionControl.h"

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

// Goals
Vector2 goalBL(17.5, 5.5);
Vector2 goalCL(5, 65);
Vector2 goalTL(17, 125);
Vector2 goalTC(74, 130);
Vector2 goalTR(140-15.5, 120);
Vector2 goalCR(122, 60);
Vector2 goalBR(140-15.5, 8);
Vector2 goalBC(67, 16);

void fullAuto();
void homeRowAuto();

void myAutonomous() {
	update = pros::Task(updateSysMan, (void *)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Update system manager");
	rollers.fullReset();
	intake.fullReset();

	flipout();
	pros::delay(500);
	in();
	strafeRelative(Vector2(0, 18), 0);
	stopRollers();
    alignAndShoot(goalBL, 135, 1);
	stopRollers();
	strafeToPoint(Vector2(24, 18));
	Vector2 secondBall(36 + 36, 46);
	turnToAngle(radToDeg((secondBall-trackingData.getPos()).getAngle()) - 90);
	in();
	strafeToPoint(secondBall);
	turnToAngle(-185);
    alignAndShoot(goalBC, radToDeg(trackingData.getHeading()), 2);
	stopRollers();
	strafeToPoint(secondBall);
    outtake(1);
    Vector2 thirdBall(secondBall.getX() + 34, secondBall.getY() - 27);
	turnToAngle(radToDeg((thirdBall - trackingData.getPos()).getAngle()) - 90);
	in();
	strafeToPoint(thirdBall);
	pros::delay(400);
	stopRollers();
	turnToAngle(-125);
    alignAndShoot(goalBR, -128, 1);
	stopRollers();

    strafeRelative(Vector2(-20, 20));
    in();
    strafeToOrientation(Vector2(90.5, 67), 360 + 10);
    // strafeToPoint(Vector2(90.5, 67));
    turnToAngle(-95);
    strafeToPoint(Vector2(110, 60));
    stopRollers();
    alignAndShoot(goalCR, -90, 1);
    stopRollers();
    strafeRelative(Vector2(-20, 0));

    Vector2 topRightBall = thirdBall + Vector2(0, 90);
    in();
    strafeToOrientation(topRightBall, -1);
    turnToAngle(-50);
    stopRollers();
    alignAndShoot(goalTR, -45, 1);

    strafeRelative(Vector2(-20, -20));
    in();
    Vector2 topCenterBall = Vector2(72, 94);
    turnToAngle(120);
    strafeToPoint(topCenterBall);
    pros::delay(200);
    stopRollers();
    turnToAngle(1);
    alignAndShoot(goalTC, 1, 1);

    strafeRelative(Vector2(0, -20));
    Vector2 topLeftBall = topRightBall + Vector2(-66, 0);
    turnToAngle(radToDeg((topLeftBall - trackingData.getPos()).getAngle()) - 90);
    in();
    strafeToPoint(topLeftBall);
    stopRollers();
    alignAndShoot(goalTL, 45, 1);

    // outtake(1);
    strafeRelative(Vector2(35, -35));
    alignAndShoot(goalCL, 135, 5, true);

    // strafeRelative(Vector2(20, -20));
    // Vector2 lastBalls(42.5, 71);
    // in();
    // strafeToOrientation(lastBalls, 180);
    // // strafeToPoint(lastBalls);
    // turnToAngle(85);
    // strafeToPoint(lastBalls + Vector2(-20, 0));
    // alignAndShoot(goalCL, 90, 1);

    // strafeRelative(Vector2(20, 0));
    // turnToAngle(-90);
    // in();
    // alignAndShoot(goalCL + Vector2(48, 0), -90, 10, true);

    rollers.reset();
    intake.reset();
	update.remove();
}


void fullAuto() {
    flipout();

    // Flipout
    pros::delay(400);
    in();
    strafeToPoint(Vector2(36, 28));
    stopRollers();

    // First goal
    strafeToOrientation(goalBL, 135);
    shootStaggered(1);
    stopRollers();

    // Second ball
    strafeToPoint(Vector2(28, 24));
    in();
    strafeToOrientation(Vector2(13.5, 32), 60);

    // Third ball
    strafeToOrientation(Vector2(36, 56), 30);
    strafeToPoint(Vector2(30.5, 70));
    stopRollers();

    // Second goal
    strafeToOrientation(goalCL, 90);
    shootStaggered(2);
    stopRollers();

    // Fourth and fifth balls
    in();
    strafeToPoint(goalCL + Vector2(10, 0));
    strafeToOrientation(Vector2(14.5, 106), 72);
    Vector2 fifthBall(28.5, 117.5);
    turnToAngle(-45);
    strafeToPoint(fifthBall);

    // Third goal
    strafeToOrientation(goalTL, 45);
    shootStaggered(1);
    stopRollers();

    // Sixth ball
    strafeToPoint(trackingData.getPos() + Vector2(10, -10));
    in();
    strafeToOrientation(Vector2(62.5, 105), -115);

    // Fourth goal
    stopRollers();
    strafeToOrientation(goalTC, 0);
    shootStaggered(2);
    stopRollers();

    // Seventh ball
    strafeToPoint(trackingData.getPos() - Vector2(0, 10));
    Vector2 seventhBall(109, 123.5);
    in();
    strafeToOrientation(seventhBall, radToDeg((seventhBall - trackingData.getPos()).getAngle()));

    // Fifth goal
    stopRollers();
    strafeToOrientation(goalTR, -45);
    shootStaggered(1);
    stopRollers();

    // Eighth ball
    strafeToPoint(trackingData.getPos() + Vector2(-30, -30));
    in();
    turnToAngle(-135);
    strafeToPoint(Vector2(112, 85));

    // Sixth goal
    stopRollers();
    strafeToOrientation(goalCR, -90);
    shootStaggered(1);
    stopRollers();

    // Ninth ball
    strafeToPoint(trackingData.getPos() + Vector2(-10, 0));
    Vector2 ninthBall(108, 28);
    turnToAngle(radToDeg((ninthBall - trackingData.getPos()).getAngle()));
    in();
    strafeToPoint(ninthBall);
    
    // Seventh goal
    stopRollers();
    strafeToOrientation(goalBR, -135);
    shootStaggered(1);
    stopRollers();

    // Tenth ball
    strafeToPoint(trackingData.getPos() + Vector2(-30, 30));
    turnToAngle(90);
    in();
    strafeToPoint(Vector2(73, 55.5));
    stopRollers();

    // Eighth goal
    strafeToOrientation(goalBC, 180);
    shootStaggered(1);
    stopRollers();

    // Last goal
    strafeToPoint(trackingData.getPos() + Vector2(0, 10));
    turnToAngle(0);
    in();
    strafeToPoint(Vector2(72.5, 64.5));
    shootStaggeredIntake(3);
}

void homeRowAuto() {
    flipout();

    // Flipout
    pros::delay(400);
    in();
    strafeToPoint(Vector2(36, 28));
    stopRollers();

    // First goal
    strafeToOrientation(goalBL, 135);
    shootStaggered(1);
    stopRollers();

    // Second ball
    strafeToPoint(Vector2(28, 24));
    Vector2 secondBall(73.5, 50.5);
    turnToAngle(radToDeg((secondBall - trackingData.getPos()).getAngle()) - 90);
    in();
    strafeToPoint(secondBall);

    // Second goal
    strafeToOrientation(goalBC, 180);
    shootStaggered(2);
    stopRollers();

    // Third ball
    strafeToPoint(secondBall);
    Vector2 thirdBall(109, 25);
    turnToAngle(radToDeg((thirdBall - trackingData.getPos()).getAngle()) - 90);
    in();
    strafeToPoint(thirdBall);

    // Third goal
    turnToAngle(-135);
    strafeToOrientation(goalBR, -135);
    shootStaggered(1);
    stopRollers();

    // Fourth ball
    strafeToPoint(trackingData.getPos() + Vector2(-20, 20));
    turnToAngle(-90);
    in();
    strafeToPoint(Vector2(129, 38.5));

    // Fifth ball
    strafeToOrientation(Vector2(100, 38.5), -40);
    strafeToPoint(Vector2(113.5, 65.5));
    stopRollers();

    // Fourth goal
    strafeToOrientation(goalCR, -90);
    shootStaggered(2);
    stopRollers();

    // Sixth ball
    strafeToPoint(trackingData.getPos() + Vector2(-10, 0));
    in();
    strafeToOrientation(Vector2(129.5, 105), -60);
    turnToAngle(45);
    strafeToPoint(Vector2(117.5, 115));

    // Fifth goal
    strafeToOrientation(goalTR, -45);
    shootStaggered(1);
    stopRollers();
}