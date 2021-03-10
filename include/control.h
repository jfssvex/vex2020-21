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
    PIDInfo constants;

public:
	PIDController(int _target, PIDInfo _constants, double _tolerance);

    double step(double newSense);
    void reset();

    double getError();
    bool isSettled();

} PIDController;

void strafeToPoint(int tx, int ty, int tangle);
void turnToAngle(float target);