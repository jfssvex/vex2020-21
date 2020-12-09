#include "systems/intake.h"
#include "systems/lift.h"
#include "systems/tray.h"
#include "systems/rollers.h"
#include "displayController.h"
#include "main.h"

extern okapi::Controller master;

extern Intake intake;
extern Tray tray;
extern Lift lift;
extern Rollers rollers;
extern DisplayController display;

extern pros::Motor frontLeftDrive;
extern pros::Motor backLeftDrive;
extern pros::Motor frontRightDrive;
extern pros::Motor backRightDrive;
extern pros::Motor lift_motor;
extern pros::Motor tray_motor;
extern pros::Motor rightIntake_motor;
extern pros::Motor leftIntake_motor;

extern pros::ADIPotentiometer trayPot;
extern pros::ADIPotentiometer liftPot;
extern pros::ADIEncoder backTrackingWheel;

extern pros::Task update;

extern std::shared_ptr<okapi::ChassisController> drive;
extern std::shared_ptr<okapi::XDriveModel> xDrive;
