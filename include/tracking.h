/**
 * @file include/tracking.h
 * 
 * Declarations for tracking function/data structure as well
 * as linear algebra structures and utility functions
 */

#ifndef _TRACKING_H_
#define _TRACKING_H_

#include "tracking.h"
#include "main.h"
#include "chassis.h"
#include <math.h> 
#define _USE_MATH_DEFINES

/**
 * @brief Convert radians to degrees
 * 
 * @param r Angle in radians
 * @return double - Angle in degrees
 */
double radToDeg(double r);
/**
 * @brief Convert degrees to radians
 * 
 * @param d Angle in degrees
 * @return double - Angle in radians
 */
double degToRad(double d);

/**
 * Represents 2D vectors for tracking/motion calculations
 */
typedef struct Vector2 {
private:
	double x, y;

public:
	/**
	 * @brief Construct a new Vector2 with 
	 * 
	 * @param _x x-value
	 * @param _y y-value
	 */
	Vector2(double _x, double _y);

	// Null constructor
	Vector2();

	/**
	 * @brief Get the x-value
	 * 
	 * @return double - x-value
	 */
	double getX();
	/**
	 * @brief Get the y-value
	 * 
	 * @return double - y-value
	 */
	double getY();

	/**
	 * @brief Get the magnitude of the vector
	 * 
	 * @return double - The vector's magnitude
	 */
	double getMagnitude();

	/**
	 * @brief Get the angle between the vector and the positive x-axis
	 * 
	 * @return double - The angle
	 */
	double getAngle();

	/**
	 * @brief Get a normalized version of this vector
	 * 
	 * @return Vector2 - The normalized vector
	 */
	Vector2 normalize();

	// Vector addition/subtraction and scalar multiplication operator overrides
	// friend keyword allows access to private members
	friend Vector2 operator + (const Vector2 &v1, const Vector2 &v2);
	friend Vector2 operator - (const Vector2 &v1, const Vector2 &v2);
	friend Vector2 operator * (const Vector2 &v1, double scalar);
} Vector2;

/**
 * Used to track current tracking position instead of 3 loose floats.
 */
typedef struct TrackingData {
private:
	bool suspendModulus;	// Whether to suspend the reduction of the angle to prevent PID errors
	double heading;			// Current robot heading in radians
	Vector2 pos;			// Current robot position

public:
	/**
	 * @brief Construct a new Tracking Data object
	 * 
	 * @param _x Starting x-coordinate
	 * @param _y Starting y-coordinate
	 * @param _h Starting heading
	 */
	TrackingData(double _x, double _y, double _h);

	/**
	 * @brief Get robot's x-coordinate
	 * 
	 * @return double - x-coordinate
	 */
	double getX();

	/**
	 * @brief Get robot's y-coordinate
	 * 
	 * @return double - y-coordinate
	 */

	double getY();
	/**
	 * @brief Get robot's heading
	 * 
	 * @return double - Heading in radians
	 */
	double getHeading();

	/**
	 * @brief Get robot's position as a vector
	 * 
	 * @return Vector2 - Position vector
	 */
	Vector2 getPos();

	/**
	 * @brief Get the robot's normalized forward vector
	 * 
	 * @return Vector2 - Forward vector
	 */
	Vector2 getForward();

	/**
	 * @brief Suspend the reduction of the heading angle to fit within (-360, 360)
	 */
	void suspendAngleModulus();

	/**
	 * @brief Resume the reduction of the heading angle to fit within (-360, 360)
	 */
	void resumeAngleModulus();

	/**
	 * @brief Update the tracking position and heading
	 * 
	 * @param _x New x-coordinate
	 * @param _y New y-coordinate
	 * @param _h New heading
	 */
	void update(double _x, double _y, double _h);

	/**
	 * @brief Update the tracking position and heading
	 * 
	 * @param _pos New position
	 * @param _h New heading
	 */
	void update(Vector2 _pos, double _h);

} TrackingData;

/**
 * @brief Tracks robot's position using tracking wheel encoders. High priority task.
 * 
 * @param param Empty task parameter
 */
void tracking(void* param);

/**
 * @brief Rotate a 2D vector about the origin by a given angle in radians
 * 
 * @param vec Vector to rotate
 * @param angle Angle of rotation in radians
 * @return Vector2 - Rotated vector
 */
Vector2 rotateVector(Vector2 vec, double angle);

/**
 * @brief Rotate a direction vector to the robot's local space
 * 
 * @param vec Vector to convert from global
 * @return Vector2 - Direction in local space
 */
Vector2 toLocalOrientation(Vector2 vec);

/**
 * @brief Rotate a direction vector to global space
 * 
 * @param vec Vector to convert from local
 * @return Vector2 - Direction in global space
 */
Vector2 toGlobalOrientation(Vector2 vec);

/**
 * @brief Get the dot product of 2 vectors
 * 
 * @param v1 Vector 1
 * @param v2 Vector 2
 * @return double - Dot product
 */
double dot(Vector2 v1, Vector2 v2);

#endif
