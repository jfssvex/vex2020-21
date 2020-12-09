#pragma once
#include "main.h"
#include "vector"

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

#define TRAY_PORT 101
#define LIFT_PORT 102

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
