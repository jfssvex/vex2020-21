#include "macros.h"
#include "globals.h"
#include "main.h"
#include "control.h"

#define MID_HEIGHT 680
#define LOW_HEIGHT 450
#define LIFT_MULTIPLIER 0.5f
#define TRAY_LIFT_MAX 360
#define MANUAL_TRAY_VELOCITY 50

bool stacking = false;
bool lifting = false;
bool suspendDrive = false;
bool trayUp = false;

int callbackCount = 0;

// void finishedLiftCallback() {
//     lifting = false;
//     lift.setCallback(nullCallback);
//     pros::lcd::print(5, "LIFT CALLBACK: %i", callbackCount);
//     callbackCount++;
// }

// void droppedLiftCallback() {
//     lifting = false;
//     lift.setCallback(nullCallback);
//     tray.lower();
//     pros::lcd::print(5, "DROP CALLBACK: %i", callbackCount);
//     callbackCount++;
// }

// void outtakeStack(void *param) {
//     intake.out(-80);
//     pros::delay(1000);
//     intake.reset();
// }

// float driveCurve(float time) {
//     return 0.08*time + 20;
//     float speed = 20 * pow(2, 0.003 * time);
//     if(speed < 100) return speed;
//     else {return 100;}
// }

// void backOff(void *param) {
//     uint32_t start = pros::millis();
//     while(pros::millis() - start < 600) {
//         float power = -driveCurve(pros::millis()-start);
//         frontLeftDrive.move_velocity(power);
//         frontRightDrive.move_velocity(power);
//         backLeftDrive.move_velocity(power);
//         backRightDrive.move_velocity(power);
//         pros::delay(10);
//     }
//     frontLeftDrive.move(0);
//     frontRightDrive.move(0);
//     backLeftDrive.move(0);
//     backRightDrive.move(0);
//     suspendDrive = false;
// }

// void finishedTrayCallback() {
//     if(tray.getPosition() <  20) {
//         trayUp = false;
//     }
//     suspendDrive = false;
//     stacking = false;
//     tray.setCallback(nullCallback);
//     drive->setMaxVelocity(600);
//     intake.control();
//     pros::lcd::print(5, "TRAY CALLBACK: %i", callbackCount);
//     callbackCount++;
// }

// void stackCubes() {
//     trayUp = true;
//     stacking = true;
//     tray.layCubes();
//     tray.setCallback(finishedTrayCallback);
// }

// void halfStack() {
//     trayUp = true;
//     stacking = true;
//     tray.layHalf();
//     tray.setCallback(finishedTrayCallback);
// }

// void autonStack() {
//     stacking = true;
//     tray.layCubes();
//     tray.setCallback(finishedTrayCallback);
//     intake.reset();
// }

// void dropTray() {
//     stacking = true;
//     tray.lower();
//     tray.setCallback(finishedTrayCallback);
// }

// void disengageStack() {
//     suspendDrive = true;
//     stacking = true;
//     /*Something with the intake*/
//     drive->setMaxVelocity(30);
//     using namespace okapi::literals;
//     pros::Task trackingTask(outtakeStack, (void *)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Let go");
//     pros::Task backOffTas(backOff, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Back off");
//     //drive.moveDistanceAsync(-0.3_m);
//     tray.lower();
//     tray.setCallback(finishedTrayCallback);
// }

// void liftToMid() {
//     lifting = true;
//     tray.setTargetPowerControl(TRAY_LIFT_MAX, 127);
//     pros::delay(200);
//     lift.moveTo(MID_HEIGHT, finishedLiftCallback);
// }

// void liftToLow() {
//     if(stacking) {
//         return;
//     }
//     lifting = true;
//     tray.setTargetPowerControl(TRAY_LIFT_MAX, 127);
//     pros::delay(200);
//     lift.moveTo(LOW_HEIGHT, finishedLiftCallback);
// }

// void dropLift() {
//     if(stacking) return;
//     lifting = true;
//     lift.moveTo(0, droppedLiftCallback);
// }

// void incrementLift(int dir) {
//     if(lifting) {
//         return;
//     }
//     if(dir == 0) {
//         lift.lock();
//     }
//     else {
//         if(dir == 1) {
//             if (lift.getPosition()+150 < TRAY_LIFT_MAX) {
//                 tray.setTargetPowerControl(lift.getPosition()+150, 127);
//             }
//             else if (tray.getTarget() > TRAY_LIFT_MAX) {
//                 tray.setTargetPowerControl(TRAY_LIFT_MAX, 127);
//             }
//         }
//         else if(lift.getPosition() < 250) {
//             lift.reset();
//             tray.lower();
//         }
//         if(tray.getPosition() > 100) {
//             lift.move(dir == 1 ? true : false);
//         }
//     }
// }

void flipout()
{
    strafe(Vector2(0, 1), 0);
    pros::delay(300);
    strafe(Vector2(0, -1), 0);
    pros::delay(350);
    xDrive->stop();
}
