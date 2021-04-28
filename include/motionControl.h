#pragma once

#include "tracking.h"
#include "main.h"
#include "chassis.h"
#include <math.h>

#define SETTLE_DELAY 200

typedef struct PIDInfo {
    double p, i, d, motor;

    PIDInfo(double _p, double _i, double _d);
    PIDInfo() {};
} PIDInfo;

#define TURN_TOLERANCE 0.04           // rad
#define DISTANCE_TOLERANCE 0.7        // inch
#define TURN_INTEGRAL_TOLERANCE 0.3   // rad
#define DISTANCE_INTEGRAL_TOLERANCE 3 // inch

#define MAX_ACCELERATION 10 // 127-scale

typedef struct PIDController {
private:
	double sense;
	double lastError;
	double integral;
	double error, derivative, speed;
    double target;
    double settleStart;
    bool settling, settled = false;

    // constants
    double tolerance;
    double integralTolerance;
    PIDInfo constants;

    bool first = true;

public:
	PIDController(double _target, PIDInfo _constants, double _tolerance, double _integralTolerance);

    double step(double newSense);
    void reset();

    double getError();
    bool isSettled();

} PIDController;

void strafe(Vector2 dir, double turn);
void strafeToOrientation(Vector2 target, double angle, double distanceTolerance = DISTANCE_TOLERANCE, double angleTolerance = TURN_TOLERANCE);
void strafeToPoint(Vector2 target, double distanceTolerance = DISTANCE_TOLERANCE);
void turnToAngle(double target, double tolerance = TURN_TOLERANCE);

void strafeRelative(Vector2 offset, double aOffset = 0);
void alignAndShoot(Vector2 goal, double angle, uint8_t balls = 1, bool intake = false, double distanceTolerance = DISTANCE_TOLERANCE, double angleTolerance = TURN_TOLERANCE);