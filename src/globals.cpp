/**
 * @file src/globals.cpp
 * 
 * Contains definitions for all global extern variables
 */

#include "globals.h"
#include "chassis.h"

// V5 controller
okapi::Controller master;

// State machines
Intake intake(Intake::OPERATOR_OVERRIDE, master);
Rollers rollers(Rollers::IDLE_STATE, master);

// LVGL display controller
DisplayController display = DisplayController();

// Chassis motors
pros::Motor frontLeftDrive(FL_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor backLeftDrive(BL_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor frontRightDrive(FR_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor backRightDrive(BR_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

// Do-nothing task function used to initialize tasks
void nullTask(void *param) {
    while(true) {
        pros::delay(10);
    }
}
pros::Task update(nullTask, (void *)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "null");

// Tracking wheel encoders
pros::ADIEncoder backTrackingWheel(BACK_TRACK_TOP, BACK_TRACK_BOT, true);
pros::ADIEncoder leftTrackingWheel(LEFT_TRACK_TOP, LEFT_TRACK_BOT, false);
pros::ADIEncoder rightTrackingWheel(RIGHT_TRACK_TOP, RIGHT_TRACK_BOT, true);
TrackingData trackingData(36, 0, 0);

// Bumper switch for goal alignment
pros::ADIButton alignerSwitch(ALIGNER_SWITCH_PORT);

using namespace okapi::literals;

// Tank chassis object (not sure why this is here, probably didn't remove it after TT)
std::shared_ptr<okapi::ChassisController> drive = okapi::ChassisControllerBuilder()
    .withMotors(
        {+FL_PORT, +BL_PORT},
        {FR_PORT, -BR_PORT}
    )
    .withDimensions(okapi::AbstractMotor::gearset::green, {{4_in, 13_in}, okapi::imev5GreenTPR})
    .build();

// X-drive chassis object
std::shared_ptr<okapi::ChassisController> chassis = okapi::ChassisControllerBuilder()
    .withMotors(FL_PORT, FR_PORT, BR_PORT, BL_PORT)
    .withDimensions(okapi::AbstractMotor::gearset::green, {{4_in, 17_in}, okapi::imev5GreenTPR})
    .build();

// X-drive model for easier strafe driving in operator control
std::shared_ptr<okapi::XDriveModel> xDrive = std::dynamic_pointer_cast<okapi::XDriveModel>(chassis->getModel());

// Do-nothing callback for when functions ask for callbacks and you don't have one
void nullCallback() {
    pros::delay(5);
}
