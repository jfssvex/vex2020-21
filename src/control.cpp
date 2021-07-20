/**
 * @file src/control.cpp
 * 
 * Contains definitions of motion control functions for controlling
 * the chassis. Currently designed for X-Drive control for the Change
 * Up seasons specifically.
 */

#include "tracking.h"
#include "chassis.h"
#include "globals.h"
#include "motionControl.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "okapi/api.hpp"
#include "macros.h"

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

/**
 * @brief Flips an angle from negative to positive.
 * Ex: -pi = +pi
 * 
 * @param angle Angle to flip in radians
 * @return double - Equivalent angle with flipped sign in radians
 */
double flipAngle(double angle) {
	if(angle > 0) {
		return -(2 * M_PI) + angle;
	}
	else {
		return (2 * M_PI) + angle;
	}
}

/* This doesn't work right now, didn't have time to figure out why
void stepMotor(pros::Motor motor, float targetSpeed) {
	float targetRPM = motor.get_target_velocity();
	auto gearset = motor.get_gearing();
	float RPMscale;

	switch(gearset) {
		case(pros::E_MOTOR_GEARSET_36): // red cartridge (torque)
			RPMscale = 100;
			break;
		case(pros::E_MOTOR_GEARSET_18): // green cartridge (high speed)
			RPMscale = 200;
			break;
		case(pros::E_MOTOR_GEARSET_06): // blue cartridge (turbo)
			RPMscale = 600;
			break;
	}

	// Convert to the 127-scale
	float pastTarget = (targetRPM / RPMscale) * 127;

	// Cap acceleration and drive
	float delta = targetSpeed - pastTarget;
	if(abs(delta) > MAX_ACCELERATION) {
		delta = (delta/abs(delta)) * MAX_ACCELERATION;
	}
	motor.move(delta);
} */

void strafe(Vector2 dir, double turn) {
	// Get the direction relative to the robot's orientation
	dir = toLocalOrientation(dir);
	double xVel = dir.getX();
	double yVel = dir.getY();

	// Scale everything so no motor is being driven at >100%
	double scalar = 1;
	if(abs(xVel) + abs(yVel) + abs(turn) > 1) {
		scalar = abs(xVel) + abs(yVel) + abs(turn);
	}

	// This doesn't work right now, didn't have time to figure out why
	// stepMotor(frontLeft, (xVel + yVel - turn) / scalar * 127);
	// stepMotor(frontRight, (-xVel + yVel + turn) / scalar * 127);
	// stepMotor(backLeft, (-xVel + yVel - turn) / scalar * 127);
	// stepMotor(backRight, (xVel + yVel + turn) / scalar * 127);

	// Drive motors according to their positions on the robot
	// The signs are determined by how the wheel would move/turn the robot if set to positive power
	frontLeft.move((xVel + yVel - turn) / scalar * 127);
	frontRight.move((-xVel + yVel + turn) / scalar * 127);
	backLeft.move((-xVel + yVel - turn) / scalar * 127);
	backRight.move((xVel + yVel + turn) / scalar * 127);
}

void strafeRelative(Vector2 offset, double aOffset) {
	// If no position offset, just turn
	if(offset.getMagnitude() == 0) {
		turnToAngle(trackingData.getHeading() + aOffset);
	}
	// If no angle offset, just strafe
	else if(aOffset == 0) {
		strafeToPoint(trackingData.getPos() + offset);
	}
	// If both, do both
	else {
		strafeToOrientation(trackingData.getPos() + offset, trackingData.getHeading() + aOffset);
	}
	return;
}

void alignAndShoot(Vector2 goal, double angle, uint8_t balls, bool intake, double distanceTolerance, double angleTolerance) {
	// Angle modulus is suspended to prevent PID errors. See relevant journal entry for details.
	trackingData.suspendAngleModulus();
	double time = pros::millis();
	// Convert angle to radians and then flip if it makes the turn easier
	angle = angle * M_PI / 180;
	if (abs(angle - trackingData.getHeading()) > degToRad(180)) {
		angle = flipAngle(angle);
	}

	// Target somewhere beyond the goal to force the button to trigger
	Vector2 extDir = (goal - trackingData.getPos()).normalize();
	Vector2 goalOvershoot = goal + extDir * 6;

	// Set up controllers
	PIDController distanceController(0, driveConstants, distanceTolerance, DISTANCE_INTEGRAL_TOLERANCE);
	PIDController turnController(angle, turnConstants, angleTolerance, TURN_INTEGRAL_TOLERANCE);

	// Front bumper variables
	bool sensorInterrupt = false;
	auto interruptTime = pros::millis();

	if(intake) {
		in();
	}

	do {
		// Break if the aligner button has been triggered
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

		// Step drive PID if dot is not negative. If it is negative (more than 90 deg off) only turn.
		Vector2 driveVec(0, 0);
		if(dotScalar > 0) {
			float strVel = -distanceController.step(delta.getMagnitude() * dotScalar);
			driveVec = rotateVector(Vector2(strVel, 0), delta.getAngle());
		}

		// Move chassis using PID outputs
		strafe(driveVec, tVel);

		// Timeout in case something goes wrong
		if(pros::millis() - time > 4000) {
			break;
		}

		pros::delay(20);
	} while ((!distanceController.isSettled() || !turnController.isSettled()) && !sensorInterrupt); // Break if both settled or sensor interrupts

	// After loop is complete angle can be reduced again
	trackingData.resumeAngleModulus();

	// If there are no balls to shoot, stop rollers and exit
	if(balls == 0) {
		stopRollers();
		return;
	}
	
	// Run intakes/uptake rollers based on parameters
	if(intake) {
		// shootClean works better when theres 1 ball
		if(balls == 1) { shootCleanIntake(balls); }
		else { shootStaggeredIntake(balls); }
	}
	else {
		if (balls == 1) { shootStaggered(balls); }
		else { shootStaggered(balls); }
	}

	// Stop rollers and exit
	stopRollers();
}

void strafeToOrientation(Vector2 target, double angle, double distanceTolerance, double angleTolerance) {
	// Stop angle reduction for PID reasons. See journal entry for more.
	trackingData.suspendAngleModulus();

	// Save time for timeout
	double time = pros::millis();
	// Flip angle if it makes turning easier
    angle = angle * M_PI / 180;
	if (abs(angle - trackingData.getHeading()) > degToRad(180)) {
		angle = flipAngle(angle);
	}

	// Set up controllers
	PIDController distanceController(0, driveConstants, distanceTolerance, DISTANCE_INTEGRAL_TOLERANCE);
	PIDController turnController(angle, turnConstants, angleTolerance, TURN_INTEGRAL_TOLERANCE);

	do {
		// Angle controller
		float tVel = turnController.step(trackingData.getHeading());

		// Get the direction to the target
		Vector2 delta = target - trackingData.getPos(); // distance vector
		Vector2 dNorm = delta.normalize();

		// Get the "forward" vector and calculate the dot product
		// This will weight the motion more towards turning which is ideal
		Vector2 alignment = rotateVector(dNorm, turnController.getError());
		float dotScalar = dot(alignment, dNorm);

		// Step drive PID if dot is not negative. Othwerise just turn
		Vector2 driveVec(0, 0);
		if(dotScalar > 0) {
			float strVel = -distanceController.step(delta.getMagnitude() * dotScalar);
			driveVec = rotateVector(Vector2(strVel, 0), delta.getAngle());
		}

		// Move chassis with PID outputs
		strafe(driveVec, tVel);

		// Timeout if something goes wrong
		if(pros::millis() - time > 4000) {
			break;
		}

		pros::delay(20);
	} while(!distanceController.isSettled() || !turnController.isSettled()); // Break when both controllers settle
}

void strafeToPoint(Vector2 target, double distanceTolerance) {
	// Get time for timeout and set up controller
	double time = pros::millis();
	PIDController distanceController(0, driveConstants, distanceTolerance, DISTANCE_INTEGRAL_TOLERANCE);

	do {
		// Get the distance to the target to use as PID sense
		Vector2 delta = target - trackingData.getPos();
		float vel = -distanceController.step(delta.getMagnitude());
		// Multiply direction vector by speed to strafe by
		Vector2 driveVec = rotateVector(Vector2(vel, 0), delta.getAngle());
		strafe(driveVec, 0);

		// Timeout if something goes wrong
		if(pros::millis() - time > 3000) {
			break;
		}

		pros::delay(20);
	} while(!distanceController.isSettled()); // Break when controller settles
}

void turnToAngle(double target, double tolerance) {
	// Suspend angle reduction for PID purposes. See journal entry for more.
	trackingData.suspendAngleModulus();

	// Flip angle if the turning becomes more efficient
    target = target * M_PI / 180;
	if(abs(target - trackingData.getHeading()) > degToRad(180)) {
		target = flipAngle(target);
	}

	// Set up controller and timeout
    double time = pros::millis();
	PIDController turnController(target, turnConstants, tolerance, TURN_INTEGRAL_TOLERANCE);

	do {
		// Step controller and drive
		float vel = turnController.step(trackingData.getHeading());
		strafe(Vector2(0, 0), vel);

		// Timeout if something goes wrong
		if(pros::millis() - time > 3000) {
			break;
		}

		pros::delay(20);
	} while(!turnController.isSettled()); // Break when controller settles

	// Resume angle reduction after PID has completed
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
	// Calculate output
    speed = (constants.p * error) + (constants.i * integral) + (constants.d * derivative);
	
	// Check if loop should settle
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