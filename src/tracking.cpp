#include "tracking.h"
#include "chassis.h"
#include "globals.h"
#define _USE_MATH_DEFINES
#include <math.h>

//Globals
float x, y, angle;
//Sensors
float rDelta, lDelta, bDelta;
std::vector<double> encoders;
//Temp
float rDist, lDist, aDelta, rLast, lLast, halfA;
float bDist, bLast;
//constants
const float lrOffset = 9.5f/2.0f; //9.5 inch wheel base, halved
const float bOffset = -6.0f;

#define DRIVE_DEGREE_TO_INCH (M_PI * 2.75 / 360) //inches per tick
#define TRACKING_WHEEL_DEGREE_TO_INCH (M_PI * 2.75 / 360)

void tracking(void* param) {
	resetChassis();
	lLast = 0;
	rLast = 0;
	bLast = 0;
	x = 0;
	y = 0;
	float left = 0;
	float right = 0;
	float lateral = 0;
	angle = 0;
	while(1) {
		float localCoord[2];
		// std::vector<double> rawEncoders = getEncoders({FL, BL, FR, BR});
		// encoders.push_back((rawEncoders[1] + rawEncoders[0])/2);
		// encoders.push_back((rawEncoders[2] + rawEncoders[3])/2);

		float leftEncoder = leftTrackingWheel.get_value();
		float rightEncoder = rightTrackingWheel.get_value();
		lDelta = leftEncoder - lLast;
		rDelta = rightEncoder - rLast;
		lLast = leftEncoder;
		rLast =	rightEncoder;
		lDist = lDelta * DRIVE_DEGREE_TO_INCH;
		rDist = rDelta * DRIVE_DEGREE_TO_INCH;
		left+=lDist;
		right+=rDist;

		bDelta = backTrackingWheel.get_value()-bLast;
		bLast = backTrackingWheel.get_value();
		bDist = bDelta * TRACKING_WHEEL_DEGREE_TO_INCH;
		lateral += bDist;

		//aDelta = (lDist - rDist)/(lrOffset*2.0f);
		float holdAngle = angle;
		angle = ((leftEncoder * DRIVE_DEGREE_TO_INCH) - (rightEncoder * DRIVE_DEGREE_TO_INCH))/(lrOffset * 2.0f);
		aDelta = angle - holdAngle;
		if(aDelta) {
			float radius = rDist / aDelta;
			halfA = aDelta/2.0f;
			float sinHalf = sin(halfA);
			localCoord[1] = ((radius + lrOffset) * sinHalf) * 2.0f;

			float backRadius = bDist / aDelta;
			localCoord[0] = ((backRadius + bOffset) * sinHalf) * 2.0f;
		}
		else {
			halfA = 0;
			aDelta = 0;
			localCoord[1] = (rDist+lDist)/2;
			localCoord[0] = bDist;
		}

		float p = halfA + holdAngle; // The global ending angle of the robot
		float cosP = cos(p);
		float sinP = sin(p);

		// Update the global position
		y += localCoord[1] * cosP - localCoord[0] * sinP;
		x += localCoord[1] * sinP + localCoord[0] * cosP;

		//Update angle
		//angle += aDelta;

		pros::lcd::print(1, "X: %f, Y: %f", x, y);
		pros::lcd::print(2, "A: %f", angle/M_PI*180);
		pros::lcd::print(3, "L: %i R: %i B: %i", (int)leftEncoder, (int)rightEncoder, (int)bLast);

		pros::delay(5);
	}
}

#define TOLERANCE 1

typedef struct pid_info {
  double p, i, d, motor;

  pid_info(double _p, double _i, double _d) {
	  this->p = _p;
	  this->i = _i;
	  this->d = _d;
  }
} pid_info;

pid_info turnConstants(0.20, 0.012, 0.6);

typedef struct pidData
{
	float sense;
	float lastError;
	float integral;
	float error, derivative, speed;
	float target, lastTarget;

	pidData(int _target) {
		this->target = _target;
	}
} pidData;

pidData CalculatePID(pidData data, pid_info pid)
{
	//calculate the error from target to current readings
	data.error = data.target - data.sense;
	data.integral += data.error; //add the error to the integral
	//find the derivative by calculating the difference from the previous two
	//  errors
	data.derivative = data.error - data.lastError;

	//disable the integral until it comes into a usable range
	if (data.error == 0 || (abs(data.error) > (127 / 2)))
	{
		data.integral = 0;
	}

	//put the whole PID shenanigan together and calculate the speed
	data.speed = (pid.p * data.error) + (pid.i * data.integral) + (pid.d * data.derivative);

	//if the previous two errors were 0, then the robot has probably stopped,
	//  so exit the program
	if ((abs(data.error) <= 0 && abs(data.lastError) <= 0) || (data.target == data.lastTarget && data.error == data.lastError))
	{
		data.speed = 0;
		data.target = data.sense;
	}

	return data;
}

void turnToAngle(float target) {
	pidData turnPid(target);

	int count = 0;

	do {
		turnPid.sense = angle * 180 / M_PI;
		turnPid = CalculatePID(turnPid, turnConstants);
		float speed = turnPid.speed;
		if(abs(speed) > 80) {
			speed = speed < 0 ? -80 : 80;
		}
		frontLeftDrive.move(speed);
		backLeftDrive.move(speed);
		frontRightDrive.move(-speed);
		backRightDrive.move(-speed);
		pros::lcd::print(6, "%f, %f, %f", turnPid.error, target, TOLERANCE);
		pros::delay(5);
		if(abs(turnPid.error) <= TOLERANCE) {
			count++;
		}
		else {
			count = 0;
		}
	} while(count < 5);
}
