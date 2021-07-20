/**
 * @file include/chassis.h
 * 
 * Information about the robot chassis including port numbers and dimensions
 */

#pragma once
#include "main.h"
#include "vector"

#define INCH_TO_CM 2.54f

// Robot dimensions in inches
#define DRIVE_WHEEL_DIAMETER 3.25f
#define TRACKING_WHEEL_DIAMETER 2.75f
#define WHEELBASE 10.375f
#define BACK_WHEEL_OFFSET 5.0f

// allMotors array index --- Drive Motors
#define FL 2
#define FR 0
#define BL 3
#define BR 1

// Physical motor port connections -- Drive Motors
#define FL_PORT 6
#define FR_PORT 8
#define BL_PORT 5
#define BR_PORT 7

// allMotors array index --- Misc. Motors
#define LINTAKE 4
#define RINTAKE 5
#define BOTROLLER 6
#define TOPROLLER 7

// Physical motor port connections -- Misc. Motors
#define LINTAKE_PORT 11
#define RINTAKE_PORT 12
#define BOTROLLER_PORT 13
#define TOPROLLER_PORT 14

// Sensor ports
#define ALIGNER_SWITCH_PORT 'A'
#define RIGHT_TRACK_TOP 'E'
#define RIGHT_TRACK_BOT 'F'
#define LEFT_TRACK_TOP 'G'
#define LEFT_TRACK_BOT 'H'
#define BACK_TRACK_TOP 'C'
#define BACK_TRACK_BOT 'D'

// Constants for motor speeds and encoder positions
#define AUTO_SPEED 80
#define INTAKE_SPEED 120
#define LIFT_LIMIT 2900
#define LIFT_SPEED 107

/**
 * @brief Curve joystick input to make driving more manageable.
 * 
 * @param input raw joystick input
 * @return double - curved joystick input
 */
double joystickSlew(double input);

/**
 * @brief Task to track motor temperatures and notify upon overheat.
 * 
 * @param param empty task parameter
 */
void motorTemp(void *param);

// -------------------------------------- The following functions are deprecated -------------------------------------- //

/**
 * @brief Get encoder readings from the allMotors array (deprecated)
 * 
 * @param ports a list of allMotors indices to read from
 * @return std::vector<double> - a list of the readings (same order as ports param)
 */
std::vector<double> getEncoders(std::vector<int> ports);

/**
 * @brief Reset all motor encoders (deprecated)
 */
void resetChassis();

/**
 * @brief Move the intake motors (deprecated)
 * 
 * @param cond which direction to move in (1 = intake, 0 = stop)
 */
void moveIntake(int cond);

/**
 * @brief Move motors from the allMotors array (deprecated)
 * 
 * @param ports a list of allMotors indices to move
 * @param speed the power to apply (127-scale)
 */
void move(std::vector<int> ports, float speed);

/**
 * @brief Move motors from the allMotors array to a certain encoder value (deprecated)
 * 
 * @param ports a list of allMotors indices to move
 * @param distance the target encoder value
 */
void moveAbsolute(std::vector<int> ports, float distance);

/**
 * @brief Set a motor's brake mode to hold
 * 
 * @param motor an allMotors index
 */
void hold(int motor);

/**
 * @brief Set a motor's brake mode to coast
 * 
 * @param motor an allMotors index
 */
void release(int motor);
