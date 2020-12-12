#ifndef _TRACKING_H_
#define _TRACKING_H_

#include "tracking.h"
#include "main.h"
#include "chassis.h"
#include <math.h> 
#define _USE_MATH_DEFINES

/*
	Tracks encoder positions; No parameters
*/
void tracking(void* param);

void strafeToPoint(int tx, int ty, int tangle);

void turnToAngle(float target);

#endif
