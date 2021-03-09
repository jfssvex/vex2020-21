#ifndef _TRACKING_H_
#define _TRACKING_H_

#include "tracking.h"
#include "main.h"
#include "chassis.h"
#include <math.h> 
#define _USE_MATH_DEFINES

typedef struct Vector2 {
private:
	double x, y;

public:
	Vector2(double _x, double _y);

	double getX();
	double getY();

	double getMagnitude();
	double getAngle();

	void normalize();

	// friend keyword allows access to private members
	friend Vector2 operator + (const Vector2 &v1, const Vector2 &v2);

	friend Vector2 operator - (const Vector2 &v1, const Vector2 &v2);
} Vector2;

typedef struct TrackingData {
private:
	double x, y, heading;

public:
	TrackingData(double _x, double _y, double _h);

	double getX();
	double getY();
	double getHeading();

	void setX(double _x);
	void setY(double _y);
	void setHeading(double _h);

} TrackingData;

/*
	Tracks encoder positions; No parameters
*/
void tracking(void* param);

#endif
