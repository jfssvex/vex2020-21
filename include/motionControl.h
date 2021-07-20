/**
 * @file include/motionControl.h
 * 
 * Declarations for motion control macros and related data structures.
 * Not called control.h to match the source file because control
 * is a header in the standard library and causes a name conflict.
 */

#pragma once

#include "tracking.h"
#include "main.h"
#include "chassis.h"
#include <math.h>

#define SETTLE_DELAY 200 // How long to wait after error < threshold before stopping PID (ms)

/**
 * Contains pid constants
 */
typedef struct PIDInfo {
    double p, i, d, motor;

    /**
     * @brief Construct a new PIDInfo object
     * 
     * @param _p P constant
     * @param _i I constant
     * @param _d D constant
     */
    PIDInfo(double _p, double _i, double _d);
    // Null constructor
    PIDInfo() {};
} PIDInfo;

// PID main tolerances and integral tolerances
#define TURN_TOLERANCE 0.04           // rad
#define DISTANCE_TOLERANCE 0.7        // inch
#define TURN_INTEGRAL_TOLERANCE 0.3   // rad
#define DISTANCE_INTEGRAL_TOLERANCE 3 // inch

#define MAX_ACCELERATION 10 // 127-scale. Acceleration limiting is currently not being used.

/**
 * Manages PID loops. Construct with appropriate information and 
 * call step() within a loop with new sensor data as the parameter
 * until PID settles.
 */
typedef struct PIDController {
private:
    // PID state variables
	double sense;               // Current sensor reading
	double lastError;           // Previous error value for derivate calculation
	double integral;            // Current integral. Reset to 0 if error > integralTolerance
	double error;               // Current error (target - sense)
    double derivative;          // Current derivative (error - lastError)
    double speed;               // Speed output
    double target;              // Target position
    double settleStart;         // Timestamp of when the loop started settling (error < tolerance)
    bool settling = false;      // If the loop is currently settling
    bool settled = false;       // If the loop has completely settled

    // constants
    double tolerance;           // Acceptable error to cancel the loop
    double integralTolerance;   // Acceptable error to start calculating integral
    PIDInfo constants;          // PID constants for this loop

    bool first = true;          // Track if this is the first time the loop has been stepped

public:
    /**
     * @brief Construct a new PIDController object
     * 
     * @param _target Target value
     * @param _constants PID constants to use
     * @param _tolerance Settling tolerance 
     * @param _integralTolerance Threshold of error to start calculating integral
     */
	PIDController(double _target, PIDInfo _constants, double _tolerance, double _integralTolerance);

    /**
     * @brief Calculate the next output from the PID loop
     * 
     * @param newSense Most recent sensor reading
     * @return double - the speed output
     */
    double step(double newSense);

    /**
     * @brief Reset all state variables
     */
    void reset();

    /**
     * @brief Get the current error
     * 
     * @return double - error
     */
    double getError();

    /**
     * @brief Check if the loop has settled
     * 
     * @return true if settled
     * @return false if still working
     */
    bool isSettled();

} PIDController;

/**
 * @brief Set x-drive chassis motors to strafe and turn simultanesouly
 * 
 * @param dir Vector representing direction and speed of motion (0 <= ||v|| <= 1)
 * @param turn Turning speed (-1 <= t <= 1). -tive = cc, +tive = ccw
 */
void strafe(Vector2 dir, double turn);

/**
 * @brief Strafe + turn x-drive to a given position + orientation in field coords
 * 
 * @param target Target position on the field
 * @param angle Target angle in degrees
 * @param distanceTolerance Acceptable distance error
 * @param angleTolerance Acceptable angle error in radians
 */
void strafeToOrientation(Vector2 target, double angle, double distanceTolerance = DISTANCE_TOLERANCE, double angleTolerance = TURN_TOLERANCE);

/**
 * @brief Strafe x-drive to a given position in field coords
 * 
 * @param target Target position on the field
 * @param distanceTolerance Acceptable distance error
 */
void strafeToPoint(Vector2 target, double distanceTolerance = DISTANCE_TOLERANCE);

/**
 * @brief Turn x-drive to a given angle in field space
 * 
 * @param target Target angle in degrees
 * @param tolerance Acceptable angle error in radians
 */
void turnToAngle(double target, double tolerance = TURN_TOLERANCE);

/**
 * @brief Strafe + turn to a position + orientation relative to current.
 * NOT in local space.
 * 
 * @param offset Distance offset of target from current position
 * @param aOffset Angle offset of target from current heading in degrees
 */
void strafeRelative(Vector2 offset, double aOffset = 0);

/**
 * @brief Drive to a goal and shoot when the front bumper is triggered or
 * when PID settles.
 * 
 * @param goal Position of the goal
 * @param angle Heading to shoot at in degrees
 * @param balls Number of balls to shoot
 * @param intake Whether to intake while shooting
 * @param distanceTolerance Acceptable distance error
 * @param angleTolerance Acceptable angle error in radians
 */
void alignAndShoot(Vector2 goal, double angle, uint8_t balls = 1, bool intake = false, double distanceTolerance = DISTANCE_TOLERANCE, double angleTolerance = TURN_TOLERANCE);