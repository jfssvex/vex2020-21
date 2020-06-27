#include "motorTemp.h"
#include "main.h"
#include "tracking.h"
#include "autonSelector.h"

extern bool complete;
static const int _TESTING = 0;

/*void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}*/

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	// pros::Motor frontLeftMtr (FL);
	// pros::Motor backLeftMtr (BL);
	// pros::Motor frontRightMtr (FR);
	// pros::Motor backRightMtr (BR);

	pros::lcd::initialize();

	pros::Task trackingTask(tracking, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Tracking Wheels");
	pros::Task tempTask(motorTemp, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Temperature tracking");

	pros::delay(500);
	pros::Task autonSel(autonSelector, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Auton Selector");
	if (_TESTING == 0) 
	 	while (getComplete() == 0 && pros::millis() <= 5000) {
	 		pros::delay(2);
	 		printf("this is auton id %d, & complete %d\n", getAutonId(), getComplete());
	 	}
	 printf("THIS IS AUTON #: %d", getAutonId());
	pros::Task delTask(autonSel);
	//pros::delay(5000);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}
