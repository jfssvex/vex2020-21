/**
 * @file include/globals.h
 * 
 * Contains extern declarations for global variables
 */

#include "systems/intake.h"
#include "systems/rollers.h"
#include "displayController.h"
#include "main.h"
#include "tracking.h"

// Primary V5 controller
extern okapi::Controller master;

// State machines
extern Intake intake;
extern Rollers rollers;

// Display controllers
extern DisplayController display;

// Motors
extern pros::Motor frontLeftDrive;
extern pros::Motor backLeftDrive;
extern pros::Motor frontRightDrive;
extern pros::Motor backRightDrive;
extern pros::Motor lift_motor;
extern pros::Motor tray_motor;
extern pros::Motor rightIntake_motor;
extern pros::Motor leftIntake_motor;

// 3-pin sensors
extern pros::ADIPotentiometer trayPot;
extern pros::ADIPotentiometer liftPot;
extern pros::ADIEncoder backTrackingWheel;
extern pros::ADIEncoder rightTrackingWheel;
extern pros::ADIEncoder leftTrackingWheel;
extern pros::ADIButton alignerSwitch;

// Tracking structures
extern TrackingData trackingData;

// State machine update task
extern pros::Task update;

// Okapi objects
extern std::shared_ptr<okapi::ChassisController> drive;
extern std::shared_ptr<okapi::XDriveModel> xDrive;
