#include "globals.h"
#include "chassis.h"

okapi::Controller master;

Intake intake(Intake::OPERATOR_OVERRIDE, master);
Rollers rollers(Rollers::IDLE_STATE, master);
DisplayController display = DisplayController();

pros::Motor frontLeftDrive(FL_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor backLeftDrive(BL_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor frontRightDrive(FR_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor backRightDrive(BR_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

void nullTask(void *param) {
    while(true) {
        pros::delay(10);
    }
}
pros::Task update(nullTask, (void *)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "null");

pros::ADIEncoder backTrackingWheel('E', 'F', true);
pros::ADIEncoder leftTrackingWheel('A', 'B', false);
pros::ADIEncoder rightTrackingWheel('C', 'D', true);
TrackingData trackingData(0, 0, 0);

using namespace okapi::literals;

std::shared_ptr<okapi::ChassisController> drive = okapi::ChassisControllerBuilder()
    .withMotors(
        {+FL_PORT, +BL_PORT},
        {FR_PORT, -BR_PORT}
    )
    .withDimensions(okapi::AbstractMotor::gearset::green, {{4_in, 13_in}, okapi::imev5GreenTPR})
    .build();

std::shared_ptr<okapi::ChassisController> chassis = okapi::ChassisControllerBuilder()
    .withMotors(FL_PORT, FR_PORT, BR_PORT, BL_PORT)
    .withDimensions(okapi::AbstractMotor::gearset::green, {{4_in, 17_in}, okapi::imev5GreenTPR})
    .build();

std::shared_ptr<okapi::XDriveModel> xDrive = std::dynamic_pointer_cast<okapi::XDriveModel>(chassis->getModel());

void nullCallback() {
    pros::delay(5);
}
