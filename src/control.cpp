#include "tracking.h"
#include "chassis.h"
#include "globals.h"
#include "control.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "okapi/api.hpp"

#define TURN_TOLERANCE 1

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
    if(abs(error) <= tolerance && abs(lastError) <= tolerance) {
        speed = 0;
        target = sense;
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

double PIDController::getError()
{
    return error;
}

using namespace okapi;
const float driveP = 1;
const float driveI = 0;
const float driveD = 0;

const float turnP = 1;
const float turnI = 0;
const float turnD = 0;

auto dController = IterativeControllerFactory::posPID(driveP, driveI, driveD);
auto aController = IterativeControllerFactory::posPID(turnP, turnI, turnD);

pros::Motor frontLeft(FL_PORT);
pros::Motor frontRight(FR_PORT, true);
pros::Motor backRight(BR_PORT, true);
pros::Motor backLeft(BL_PORT);

PIDInfo turnConstants(1, 1, 0);

float getDistance(float tx, float ty, float sx, float sy) {
	float xDiff = tx - sx;
	float yDiff = ty - sy;
	return sqrt((xDiff*xDiff) + (yDiff*yDiff));
}

void strafeToPoint(int tx, int ty, int tangle) {
	tangle = tangle / 180 * M_PI;
	dController.reset();
	aController.reset();

	dController.setTarget(0);
	aController.setTarget(tangle);

	pros::lcd::print(5, "%i", !(dController.isSettled() && aController.isSettled()));

	int count = 0;

	do
	{
		count++;
		float newX = trackingData.getX();
		float newY = trackingData.getY();
		float newAngle = -trackingData.getHeading() + (M_PI / 2);

		float xVel = dController.step(getDistance(tx, ty, newX, newY));
		float aVel = aController.step(newAngle);

		float localX = tx * cos(newAngle) - (ty * sin(newAngle));
		float localY = ty * cos(newAngle) + (tx * sin(newAngle));

		float localAngle = atan2(localX, localY) + (M_PI / 4);

		frontLeft.move((xVel * sin(localAngle) - aVel) * 127);
		frontRight.move((xVel * cos(localAngle) + aVel) * 127);
		backLeft.move((xVel * cos(localAngle) - aVel) * 127);
		backRight.move((xVel * sin(localAngle) + aVel) * 127);

		pros::lcd::print(4, "%i", count);
		pros::delay(20);
	}
	while (!(dController.isSettled() && aController.isSettled()));
}

void turnToAngle(float target) {
	PIDController turnPid(target, turnConstants, TURN_TOLERANCE);
	int count = 0;

	do {
        float speed = turnPid.step(trackingData.getHeading() * 180 / M_PI);

		if (abs(speed) > 80) {
			speed = speed < 0 ? -80 : 80;
		}
		frontLeft.move(speed);
		backLeft.move(speed);
		frontRight.move(-speed);
		backRight.move(-speed);
		pros::lcd::print(6, "%f, %f", turnPid.getError(), target);
		pros::delay(5);
		if (abs(turnPid.getError()) <= TURN_TOLERANCE) {
			count++;
		}
		else {
			count = 0;
		}
	} while (count < 5);
}
