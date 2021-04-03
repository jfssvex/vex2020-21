#ifndef _TRACKING_H_
#define _TRACKING_H_

#include "tracking.h"
#include "main.h"
#include "chassis.h"
#include <math.h> 
#define _USE_MATH_DEFINES

double radToDeg(double r);
double degToRad(double d);

typedef struct Vector2 {
private:
	double x, y;

public:
	Vector2(double _x, double _y);
	Vector2();

	double getX();
	double getY();

	double getMagnitude();
	double getAngle();

	Vector2 normalize();

	// friend keyword allows access to private members
	friend Vector2 operator + (const Vector2 &v1, const Vector2 &v2);
	friend Vector2 operator - (const Vector2 &v1, const Vector2 &v2);
	friend Vector2 operator * (const Vector2 &v1, double scalar);
} Vector2;

typedef struct TrackingData {
private:
	double heading;
	Vector2 pos;

public:
	TrackingData(double _x, double _y, double _h);

	double getX();
	double getY();
	double getHeading();
	Vector2 getPos();

	void update(double _x, double _y, double _h);

} TrackingData;

/*
	Tracks encoder positions; No parameters
*/
void tracking(void* param);
Vector2 rotateVector(Vector2 vec, double angle);
Vector2 toLocalCoordinates(Vector2 vec);

#endif
