#include "tracking.h"
#include "chassis.h"
#include "globals.h"
#include "motionControl.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "okapi/api.hpp"
#include "macros.h"

#define TURN_TOLERANCE 0.04           // rad
#define DISTANCE_TOLERANCE 0.7        // inch
#define TURN_INTEGRAL_TOLERANCE 0.3   // rad
#define DISTANCE_INTEGRAL_TOLERANCE 3 // inch

#define PID_TIMEOUT 4000              // ms

using namespace okapi;
const float driveP = 0.2;
const float driveI = 0.000001;
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

void strafeRelative(Vector2 offset, double aOffset) {
	if(offset.getMagnitude() == 0) {
		turnToAngle(trackingData.getHeading() + aOffset);
	}
	else if(aOffset == 0) {
		strafeToPoint(trackingData.getPos() + offset);
	}
	else {
		strafeToOrientation(trackingData.getPos() + offset, trackingData.getHeading() + aOffset);
	}
	return;
}

void alignAndShoot(Vector2 goal, double angle, uint8_t balls, bool intake) {

	trackingData.suspendAngleModulus();
	// Set up controllers
	Vector2 extDir = (goal - Vector2(70, 70)).normalize();
	// target somewhere beyond the goal to force the button to trigger
	Vector2 goalOvershoot = goal + extDir * 6;
	double time = pros::millis();
	angle = angle * M_PI / 180;
	if (abs(angle - trackingData.getHeading()) > degToRad(180)) {
		angle = flipAngle(angle);
	}
	PIDController distanceController(0, driveConstants, DISTANCE_TOLERANCE, DISTANCE_INTEGRAL_TOLERANCE);
	PIDController turnController(angle, turnConstants, TURN_TOLERANCE, TURN_INTEGRAL_TOLERANCE);
	bool sensorInterrupt = false;
	auto interruptTime = pros::millis();

	if(intake) {
		in();
	}

	do {
		if(alignerSwitch.get_new_press()) {
			sensorInterrupt = true;
			strafe(Vector2(0, 0), 0);
			break;
		}

		// Angle controller
		float tVel = turnController.step(trackingData.getHeading());

		Vector2 delta = goalOvershoot - trackingData.getPos();
		Vector2 dNorm = delta.normalize();

		// Get the "forward" vector and calculate the dot product
		Vector2 alignment = rotateVector(dNorm, turnController.getError());
		float dotScalar = dot(alignment, dNorm);

		// Step drive PID if dot is not negative
		Vector2 driveVec(0, 0);
		if(dotScalar > 0) {
			float strVel = -distanceController.step(delta.getMagnitude() * dotScalar);
			driveVec = rotateVector(Vector2(strVel, 0), delta.getAngle());
		}

		strafe(driveVec, tVel);

		if(pros::millis() - time > 4000) {
			break;
		}

		pros::delay(20);
	} while ((!distanceController.isSettled() || !turnController.isSettled()) && sensorInterrupt);

	trackingData.resumeAngleModulus();
	
	if(intake) {
		if(balls == 1) { shootCleanIntake(balls); }
		else { shootStaggeredIntake(balls); }
	}
	else {
		if (balls == 1) { shootClean(balls); }
		else { shootStaggered(balls); }
	}

	stopRollers();
}

void strafeToOrientation(Vector2 target, double angle) {
	trackingData.suspendAngleModulus();
	turnToAngle(angle);
	strafeToPoint(target);
	trackingData.resumeAngleModulus();

	return;

	double time = pros::millis();
    angle = angle * M_PI / 180;
	if (abs(angle - trackingData.getHeading()) > degToRad(180)) {
		angle = flipAngle(angle);
	}
	PIDController distanceController(0, driveConstants, DISTANCE_TOLERANCE, DISTANCE_INTEGRAL_TOLERANCE);
	PIDController turnController(angle, turnConstants, TURN_TOLERANCE, TURN_INTEGRAL_TOLERANCE);

	do {
		// Angle controller
		float tVel = turnController.step(trackingData.getHeading());

		// Distance controller
		Vector2 delta = target - trackingData.getPos(); // distance vector
		Vector2 dNorm = delta.normalize();

		// Get the "forward" vector and calculate the dot product
		Vector2 alignment = rotateVector(dNorm, turnController.getError());
		float dotScalar = dot(alignment, dNorm);

		// Step drive PID if dot is not negative
		Vector2 driveVec(0, 0);
		if(dotScalar > 0) {
			float strVel = -distanceController.step(delta.getMagnitude() * dotScalar);
			driveVec = rotateVector(Vector2(strVel, 0), delta.getAngle());
		}

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

		if(pros::millis() - time > 3000) {
			break;
		}

		pros::delay(20);
	} while(!distanceController.isSettled());
}

void turnToAngle(double target) {
	trackingData.suspendAngleModulus();

    target = target * M_PI / 180;
	if(abs(target - trackingData.getHeading()) > degToRad(180)) {
		target = flipAngle(target);
	}

    double time = pros::millis();
	PIDController turnController(target, turnConstants, TURN_TOLERANCE, TURN_INTEGRAL_TOLERANCE);

	do {
		float vel = turnController.step(trackingData.getHeading());
		strafe(Vector2(0, 0), vel);

		if(pros::millis() - time > 3000) {
			break;
		}

		pros::delay(20);
	} while(!turnController.isSettled());

	trackingData.resumeAngleModulus();
}

PIDInfo::PIDInfo(double _p, double _i, double _d) {
    this->p = _p;
    this->i = _i;
    this->d = _d;
}

PIDController::PIDController(double _target, PIDInfo _constants, double _tolerance, double _integralTolerance) {
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
	if(first) {
		lastError = error;
		first = false;
	}
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