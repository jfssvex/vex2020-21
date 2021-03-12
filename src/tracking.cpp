#include "tracking.h"
#include "chassis.h"
#include "globals.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "okapi/api.hpp"

// Sensors
float rDelta, lDelta, bDelta;
std::vector<double> encoders;
// Temp
float rDist, lDist, aDelta, rLast, lLast, halfA;
float bDist, bLast;
// Constants and macros
const float lrOffset = WHEELBASE/2.0f; // 9.5 inch wheel base, halved
const float bOffset = -BACK_WHEEL_OFFSET;

// inches per encoder tick (degrees)
#define DRIVE_DEGREE_TO_INCH (M_PI * DRIVE_WHEEL_DIAMETER / 360) 
#define TRACKING_WHEEL_DEGREE_TO_INCH (M_PI * TRACKING_WHEEL_DIAMETER / 360)

void tracking(void* param) {

	// Initialize variables
	resetChassis();
	lLast = 0;
	rLast = 0;
	bLast = 0;
	float x = 0;
	float y = 0;
	float left = 0;
	float right = 0;
	float lateral = 0;
	float angle = 0;

	// Start tracking loop
	while(1) {
		float localX, localY;

		// Get encoder data
		float leftEncoder = leftTrackingWheel.get_value();
		float rightEncoder = rightTrackingWheel.get_value();
		float backEncoder = backTrackingWheel.get_value();

		// Calculate deltas
		lDelta = leftEncoder - lLast;
		rDelta = rightEncoder - rLast;
		bDelta = backEncoder - bLast;
		lDist = lDelta * TRACKING_WHEEL_DEGREE_TO_INCH;
		rDist = rDelta * TRACKING_WHEEL_DEGREE_TO_INCH;
		bDist = bDelta * TRACKING_WHEEL_DEGREE_TO_INCH;

		// Store readings for next deltas
		lLast = leftEncoder;
		rLast =	rightEncoder;
		bLast = backTrackingWheel.get_value();

		// Increment persistent variables
		left += lDist;
		right += rDist;
		lateral += bDist;

		//aDelta = (lDist - rDist)/(lrOffset*2.0f);
		// Calculate arc angle
		float holdAngle = angle;
		angle = (left - right)/(lrOffset * 2.0f);
		// angle = ((leftEncoder * DRIVE_DEGREE_TO_INCH) - (rightEncoder * DRIVE_DEGREE_TO_INCH))/(lrOffset * 2.0f);
		aDelta = angle - holdAngle;

		// If theres an arc
		if(aDelta) {
			float radius = rDist / aDelta;
			halfA = aDelta/2.0f;
			float sinHalf = sin(halfA);
			localY = ((radius + lrOffset) * sinHalf) * 2.0f;

			float backRadius = bDist / aDelta;
			localX = ((backRadius + bOffset) * sinHalf) * 2.0f;
		}
		// If no arc
		else {
			halfA = 0;
			aDelta = 0;
			localY = (rDist+lDist)/2;
			localX = bDist;
		}

		float p = halfA + holdAngle; // The global ending angle of the robot
		float cosP = cos(p);
		float sinP = sin(p);

		// Update the global position
		y += localY * cosP - localX * sinP;
		x += localY * sinP + localX * cosP;

		trackingData.update(x, y, angle);

		//Update angle
		//angle += aDelta;

		// Print debug
		pros::lcd::print(1, "X: %f, Y: %f", trackingData.getX(), trackingData.getY());
		pros::lcd::print(2, "A: %f", trackingData.getHeading()/M_PI*180);
		pros::lcd::print(3, "L: %i R: %i B: %i", (int)leftEncoder, (int)rightEncoder, (int)bLast);

		pros::delay(5);
	}
}

// ----------------- Math utilities ----------------- //

Vector2 toLocalCoordinates(Vector2 vec) {
	double localAngle = trackingData.getHeading();
	double angle = localAngle - vec.getAngle();

	return rotateVector(vec, angle);
}

Vector2 rotateVector(Vector2 vec, double angle) {
	// x = cos(a), y = sin(a)
	// cos(a + b) = cos(a)cos(b) - sin(a)sin(b)
	double newX = (vec.getX() * cos(angle)) - (vec.getY() * sin(angle));

	// sin(a + b) = sin(a)cos(b) + cos(a)sin(b)
	double newY = (vec.getY() * cos(angle)) + (vec.getX() * sin(angle));

	return Vector2(newX, newY);
}

// ----------------- Tracking Data Struct ----------------- //

TrackingData::TrackingData(double _x, double _y, double _h) {
	this->pos = Vector2(_x, _y);
	this->heading = fmod(_h, 360);
}

double TrackingData::getX() {
	return pos.getX();
}
double TrackingData::getY() {
	return pos.getY();
}
double TrackingData::getHeading() {
	return heading;
}
Vector2 TrackingData::getPos() {
	return pos;
}

void TrackingData::update(double _x, double _y, double _h) {
	this->pos = Vector2(_x, _y);
	this->heading = _h;
}

// ----------------- Vector2 Struct ----------------- //

Vector2 operator + (const Vector2 &v1, const Vector2 &v2) {
	Vector2 vec(v1.x + v2.x, v1.y + v2.y);
	return vec;
}

Vector2 operator - (const Vector2 &v1, const Vector2 &v2) {
	Vector2 vec(v1.x - v2.x, v1.y - v2.y);
	return vec;
}

Vector2 operator * (const Vector2 &v1, double scalar) {
	Vector2 vec(v1.x * scalar, v1.y * scalar);
	return vec;
}

Vector2::Vector2(double _x, double _y) {
	this->x = _x;
	this->y = _y;
}
Vector2::Vector2() {
	this->x = 0;
	this->y = 0;
}

double Vector2::getX() {
	return x;
}
double Vector2::getY() {
	return y;
}

double Vector2::getMagnitude() {
	return sqrt((x*x) + (y*y));
}
double Vector2::getAngle() {
	return atan2(y, x);
}

void Vector2::normalize() {
	double divisor = this->getMagnitude();
	x = x/divisor;
	y = y/divisor;
}