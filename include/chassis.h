#pragma once
#include "main.h"
#include "vector"

#define INCH_TO_CM 2.54f

// Robot dimensions in inches
#define DRIVE_WHEEL_DIAMETER 3.25f
#define TRACKING_WHEEL_DIAMETER 2.75f
#define WHEELBASE 10.25f
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

#define TRAY 8
#define LIFT 9

// Physical motor port connections -- Misc. Motors
#define LINTAKE_PORT 11
#define RINTAKE_PORT 12
#define BOTROLLER_PORT 13
#define TOPROLLER_PORT 14

// Sensor ports
#define ALIGNER_SWITCH_PORT 'G'
#define RIGHT_TRACK_TOP 'E'
#define RIGHT_TRACK_BOT 'F'
#define LEFT_TRACK_TOP 'A'
#define LEFT_TRACK_BOT 'B'
#define BACK_TRACK_TOP 'C'
#define BACK_TRACK_BOT 'D'

#define AUTO_SPEED 80
#define INTAKE_SPEED 120
#define LIFT_LIMIT 2900
#define LIFT_SPEED 107

double joystickSlew(double input);
std::vector<double> getEncoders(std::vector<int> ports);
void auton();
void resetChassis();
void moveIntake(int cond);
void move(std::vector<int> ports, float speed);
void motorTemp(void* param);
void moveAbsolute(std::vector<int> ports, float distance);
void hold(int motor);
void release(int motor);
