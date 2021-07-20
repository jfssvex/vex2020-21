/**
 * @file src/tracking.cpp
 * 
 * Contains tracking task used to locate the robot on the field using
 * tracking wheel encoder readings. Also contains definitions for helper
 * classes and math utilities to handle vector transformations.
 * Algorithm from http://thepilons.ca/wp-content/uploads/2018/10/Tracking.pdf
 */

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

double roundUp(double v, int places) {
    const double mult = std::pow(10.0, places);
	return std::ceil(v * mult) / mult;
}

double radToDeg(double r) {
	return r * 180 / M_PI;
}
double degToRad(double d) {
	return d * M_PI / 180;
}

void tracking(void* param) {

	// Initialize variables
	lLast = 0;
	rLast = 0;
	bLast = 0;
	float x = trackingData.getX();
	float y = trackingData.getY();
	float left = 0;
	float right = 0;
	float lateral = 0;
	float angle = 0;

	auto printTime = pros::millis();

	// Start tracking loop
	while(1) {
		float localX, localY = 0;

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
		bLast = backEncoder;

		// Increment persistent variables
		left += lDist;
		right += rDist;
		lateral += bDist;

		// Calculate arc angle
		float holdAngle = angle;
		angle = (right - left)/(lrOffset * 2.0f);
		aDelta = angle - holdAngle;

		// If theres an arc
		if(aDelta) {
			float radius = (rDist / aDelta) - lrOffset;
			halfA = aDelta/2.0f;
			float sinHalf = sin(halfA);
			localY = (radius * sinHalf) * 2.0f;

			float backRadius = (bDist / aDelta) - BACK_WHEEL_OFFSET;
			localX = (backRadius * sinHalf) * 2.0f;
		}
		// If no arc
		else {
			halfA = 0;
			aDelta = 0;
			localY = (rDist+lDist)/2;
			localX = bDist;
		}

		float p = -(halfA + holdAngle); // The global ending angle of the robot
		float cosP = cos(p);
		float sinP = sin(p);

		// Update the global position
		float dY = (localY * cosP) - (localX * sinP);
		float dX = (localY * sinP) + (localX * cosP);

		trackingData.update(trackingData.getX() + dX, trackingData.getY() +dY, trackingData.getHeading() + aDelta);

		// Print debug
		pros::lcd::print(1, "X: %f, Y: %f", trackingData.getX(), trackingData.getY());
		pros::lcd::print(2, "A: %f", trackingData.getHeading()*180/M_PI);

		if(pros::millis() - printTime > 50) {
			printf("X: %f, Y: %f, A: %f\n", trackingData.getX(), trackingData.getY(), radToDeg(trackingData.getHeading()));
			printTime = pros::millis();
			// printf("X: %f, Y: %f", left, right);
			// printf("A: %f", lateral);
		}

		pros::delay(5);
	}
}

// ----------------- Math utilities ----------------- //

Vector2 toLocalOrientation(Vector2 vec) {
	double localAngle = -trackingData.getHeading();

	return rotateVector(vec, localAngle);
}

Vector2 toGlobalOrientation(Vector2 vec) {
	double localAngle = trackingData.getHeading();

	return rotateVector(vec, localAngle);
}

double dot(Vector2 v1, Vector2 v2) {
	// a dot b = a_1 * b_1 + a_2 * b_2 ... + a_n * b_n
	return (v1.getX() * v2.getX()) + (v1.getY() * v2.getY());
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
	this->heading = fmod(_h, 2*M_PI);
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
Vector2 TrackingData::getForward() {
	return toGlobalOrientation(Vector2(0, 1));
}

void TrackingData::update(double _x, double _y, double _h) {
	this->pos = Vector2(_x, _y);
	if(!suspendModulus) {
		// Make sure heading is between -2pi and +2pi
		this->heading = fmod(_h, 2 * M_PI);
	}
	else {
		this->heading = _h;
	}
}
void TrackingData::update(Vector2 _pos, double _h) {
	this->pos = _pos;
	if(!suspendModulus) {
		// Make sure heading is between -2pi and +2pi
		this->heading = fmod(_h, 2 * M_PI);
	}
	else {
		this->heading = _h;
	}
}

void TrackingData::suspendAngleModulus() {
	suspendModulus = true;
}
void TrackingData::resumeAngleModulus() {
	suspendModulus = false;
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

Vector2 Vector2::normalize() {
	double divisor = this->getMagnitude();
	double nx = x/divisor;
	double ny = y/divisor;
	return Vector2(nx, ny);
}