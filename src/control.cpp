#include "tracking.h"
#include "chassis.h"
#include "globals.h"
#include "control.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "okapi/api.hpp"

#define TURN_TOLERANCE 1
#define DISTANCE_TOLERANCE 1

using namespace okapi;
const float driveP = 1;
const float driveI = 0;
const float driveD = 0;

const float turnP = 1;
const float turnI = 0;
const float turnD = 0;

pros::Motor frontLeft(FL_PORT);
pros::Motor frontRight(FR_PORT, true);
pros::Motor backRight(BR_PORT, true);
pros::Motor backLeft(BL_PORT);

PIDInfo turnConstants(1, 1, 0);
PIDInfo driveConstants(driveP, driveI, driveD);

float getDistance(float tx, float ty, float sx, float sy) {
	float xDiff = tx - sx;
	float yDiff = ty - sy;
	return sqrt((xDiff*xDiff) + (yDiff*yDiff));
}

// void strafeAndTurnToPoint(Vector2 target, int tangle) {
// 	tangle = tangle / 180 * M_PI;
// 	dController.reset();
// 	aController.reset();

// 	dController.setTarget(0);
// 	aController.setTarget(tangle);

// 	pros::lcd::print(5, "%i", !(dController.isSettled() && aController.isSettled()));

// 	int count = 0;

// 	do
// 	{
// 		count++;
// 		float newX = trackingData.getX();
// 		float newY = trackingData.getY();
// 		float newAngle = -trackingData.getHeading() + (M_PI / 2);

// 		float xVel = dController.step(getDistance(tx, ty, newX, newY));
// 		float aVel = aController.step(newAngle);

// 		float localX = tx * cos(newAngle) - (ty * sin(newAngle));
// 		float localY = ty * cos(newAngle) + (tx * sin(newAngle));

// 		float localAngle = atan2(localX, localY) + (M_PI / 4);

// 		frontLeft.move((xVel * sin(localAngle) - aVel) * 127);
// 		frontRight.move((xVel * cos(localAngle) + aVel) * 127);
// 		backLeft.move((xVel * cos(localAngle) - aVel) * 127);
// 		backRight.move((xVel * sin(localAngle) + aVel) * 127);

// 		pros::lcd::print(4, "%i", count);
// 		pros::delay(20);
// 	}
// 	while (!(dController.isSettled() && aController.isSettled()));
// }

void strafe(Vector2 dir, double turn) {
	dir = toLocalCoordinates(dir);
	dir.normalize();
	double xVel = dir.getX();
	double yVel = dir.getY();

	double scalar = 1;
	if(abs(xVel) + abs(yVel) + abs(turn) > 1) {
		scalar = abs(xVel) + abs(yVel) + abs(turn);
	}

	frontLeft.move((xVel + yVel - turn) / scalar * 127);
	frontRight.move((-xVel + yVel + turn) / scalar * 127);
	backLeft.move((-xVel + yVel - turn) / scalar * 127);
	backRight.move((xVel + yVel + turn) / scalar * 127);
}

void strafeToOrientation(Vector2 target, double angle) {
	PIDController distanceController(target.getMagnitude(), driveConstants, DISTANCE_TOLERANCE);
	PIDController turnController(angle, turnConstants, TURN_TOLERANCE);

	do {
		Vector2 delta = target - trackingData.getPos();
		float strVel = distanceController.step(delta.getMagnitude());
		Vector2 driveVec = rotateVector(Vector2(strVel, 0), delta.getAngle());
		float tVel = turnController.step(trackingData.getHeading());

		strafe(driveVec, tVel);

		pros::delay(20);
	} while(!distanceController.isSettled() && !turnController.isSettled());
}

void strafeToPoint(Vector2 target) {
	PIDController distanceController(target.getMagnitude(), driveConstants, DISTANCE_TOLERANCE);

	do {
		Vector2 delta = target - trackingData.getPos();
		float vel = distanceController.step(delta.getMagnitude());
		Vector2 driveVec = rotateVector(Vector2(vel, 0), delta.getAngle());
		strafe(driveVec, 0);

		pros::delay(20);
	} while(!distanceController.isSettled());
}

void turnToAngle(float target) {
	PIDController turnController(target, turnConstants, TURN_TOLERANCE);

	do {
		float vel = turnController.step(trackingData.getHeading());
		strafe(Vector2(0, 0), vel);

		pros::delay(20);
	} while(!turnController.isSettled());
}

PIDInfo::PIDInfo(double _p, double _i, double _d) {
    this->p = _p;
    this->i = _i;
    this->d = _d;
}

PIDController::PIDController(int _target, PIDInfo _constants, double _tolerance) {
	this->target = _target;
    this->lastError = target;
    this->constants = _constants;
    this->tolerance = _tolerance;
}

double PIDController::step(double newSense) {
    // calculate error terms
    sense = newSense;
    error = target - sense;
    integral += error;
    derivative = error - lastError;
    lastError = error;
    // Disable the integral until it enters a usable range of error
    if(error == 0 || abs(error) > 127 / 2) {
        integral = 0;
    }
    speed = (constants.p * error) + (constants.i * integral) + (constants.d * derivative);
    if(abs(error) <= tolerance) {
		if(!settling) {
			settleStart = pros::millis();
		}
		settling = true;
        speed = 0;
		if(pros::millis() - settleStart > SETTLE_DELAY) {
			settled = true;
		}
    }
	else {
		settling = false;
		settled = false;
	}
    return speed;
}

void PIDController::reset() {
    target = 0;
    sense = 0;
    lastError = 0;
    integral = 0;
    derivative = 0;
    speed = 0;
    error = 0;
}

double PIDController::getError() {
    return error;
}
bool PIDController::isSettled() {
	return settled;
}