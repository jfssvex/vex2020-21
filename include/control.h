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
void strafeToOrientation(Vector2 target, double angle);
void strafeToPoint(Vector2 target);
void turnToAngle(double target);