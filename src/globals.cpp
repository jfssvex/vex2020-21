#include "globals.h"
#include "chassis.h"

okapi::Controller master;

Intake intake(Intake::OPERATOR_OVERRIDE, master);
Tray tray(Tray::IDLE_STATE, master);
Lift lift(Lift::IDLE_STATE);

pros::Motor frontLeftDrive(FL_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor backLeftDrive(BL_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor frontRightDrive(FR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor backRightDrive(BR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor lift_motor(LIFT_PORT, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor tray_motor(TRAY_PORT, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightIntake_motor(RINTAKE_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor leftIntake_motor(LINTAKE_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

void nullTask(void *param) {
    while(true) {
        pros::delay(10);
    }
}
pros::Task update(nullTask, (void *)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "null");

pros::ADIPotentiometer trayPot(TRAY_POT_PORT);
pros::ADIPotentiometer liftPot(LIFT_POT_PORT);
pros::ADIEncoder backTrackingWheel('A', 'B', true);

using namespace okapi::literals;
okapi::ChassisControllerIntegrated drive = okapi::ChassisControllerFactory::create(
    {+FL_PORT, +BL_PORT}, {-FR_PORT, -BR_PORT},
    okapi::AbstractMotor::gearset::green,
    {3.25_in, 13_in});

void nullCallback() {
    pros::delay(5);
}
