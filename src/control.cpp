#include "tracking.h"
#include "chassis.h"
#include "globals.h"
#include "motionControl.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "okapi/api.hpp"

#define TURN_TOLERANCE 0.04
#define DISTANCE_TOLERANCE 0.7
#define TURN_INTEGRAL_TOLERANCE 0.3
#define DISTANCE_INTEGRAL_TOLERANCE 3

using namespace okapi;
const float driveP = 0.2;
const float driveI = 0.0000000001;
const float driveD = 0.1;

const float turnP = 1.5;
const float turnI = 0.1;
const float turnD = 0.5;

pros::Motor frontLeft(FL_PORT);
pros::Motor frontRight(FR_PORT, true);
pros::Motor backRight(BR_PORT, true);
pros::Motor backLeft(BL_PORT);

PIDInfo turnConstants(turnP, turnI, turnD);
PIDInfo driveConstants(driveP, driveI, driveD);

double flipAngle(double angle) {
	if(angle > 0) {
		return -(2 * M_PI) + angle;
	}
	else {
		return (2 * M_PI) + angle;
	}
}

float getDistance(float tx, float ty, float sx, float sy) {
	float xDiff = tx - sx;
	float yDiff = ty - sy;
	return sqrt((xDiff*xDiff) + (yDiff*yDiff));
}

void strafe(Vector2 dir, double turn) {
	dir = toLocalCoordinates(dir);
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
	double time = pros::millis();
    angle = angle * M_PI / 180;
	if (abs(angle - trackingData.getHeading()) > degToRad(180)) {
		angle = flipAngle(angle);
	}
	PIDController distanceController(0, driveConstants, DISTANCE_TOLERANCE, DISTANCE_INTEGRAL_TOLERANCE);
	PIDController turnController(angle, turnConstants, TURN_TOLERANCE, TURN_INTEGRAL_TOLERANCE);

	do {
		Vector2 delta = target - trackingData.getPos();
		float strVel = -distanceController.step(delta.getMagnitude());
		Vector2 driveVec = rotateVector(Vector2(strVel, 0), delta.getAngle());
		float tVel = 2 * turnController.step(trackingData.getHeading());

		strafe(driveVec, tVel);

		if(pros::millis() - time > 4000) {
			break;
		}

		pros::delay(20);
	} while(!distanceController.isSettled() || !turnController.isSettled());
}

void strafeToPoint(Vector2 target) {
	double time = pros::millis();
	PIDController distanceController(0, driveConstants, DISTANCE_TOLERANCE, DISTANCE_INTEGRAL_TOLERANCE);

	do {
		Vector2 delta = target - trackingData.getPos();
		float vel = -distanceController.step(delta.getMagnitude());
		Vector2 driveVec = rotateVector(Vector2(vel, 0), delta.getAngle());
		strafe(driveVec, 0);

		if(pros::millis() - time > 4000) {
			break;
		}

		pros::delay(20);
	} while(!distanceController.isSettled());
}

void turnToAngle(double target) {
    target = target * M_PI / 180;
	if(abs(target - trackingData.getHeading()) > degToRad(180)) {
		target = flipAngle(target);
	}

    double time = pros::millis();
	PIDController turnController(target, turnConstants, TURN_TOLERANCE, TURN_INTEGRAL_TOLERANCE);

	do {
		float vel = turnController.step(trackingData.getHeading());
		strafe(Vector2(0, 0), vel);

		if(pros::millis() - time > 4000) {
			break;
		}

		pros::delay(20);
	} while(!turnController.isSettled());
}

PIDInfo::PIDInfo(double _p, double _i, double _d) {
    this->p = _p;
    this->i = _i;
    this->d = _d;
}

PIDController::PIDController(double _target, PIDInfo _constants, double _tolerance, double _integralTolerance = 1) {
	this->target = _target;
    this->lastError = target;
    this->constants = _constants;
    this->tolerance = _tolerance;
	this->integralTolerance = _integralTolerance;
}

double PIDController::step(double newSense) {

    // calculate error terms
    sense = newSense;
    error = target - sense;
    integral += error;
    derivative = error - lastError;
    lastError = error;
    // Disable the integral until it enters a usable range of error
    if(error == 0 || abs(error) > integralTolerance) {
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