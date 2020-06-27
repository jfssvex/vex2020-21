#include "chassis.h"
#include <vector>

pros::Motor allMotors[] = {
    pros::Motor(FR_PORT, pros::E_MOTOR_GEARSET_18),
    pros::Motor(BR_PORT, pros::E_MOTOR_GEARSET_18),
    pros::Motor(FL_PORT, pros::E_MOTOR_GEARSET_18),
    pros::Motor(BL_PORT, pros::E_MOTOR_GEARSET_18),
    pros::Motor(LINTAKE_PORT),
    pros::Motor(RINTAKE_PORT),
    pros::Motor(TRAY_PORT),
    pros::Motor(LIFT_PORT)};

void chassisSet(float leftSpeed, float rightSpeed) {
    allMotors[FR].move(rightSpeed);
    allMotors[BR].move(rightSpeed);
    allMotors[FL].move(leftSpeed);
    allMotors[BL].move(leftSpeed);
}

void hold(int port) {
	allMotors[port].set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

void release(int port) {
	allMotors[port].set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

void resetChassis() {
	for(auto i : allMotors) {
		i.tare_position();
	}
}

void move(std::vector<int> ports, float speed) {
    for(auto i : ports) {
        allMotors[i].move(speed);
    }
}

void auton() {
}

void moveIntake(int cond) {
	if (cond == 1) {
		move({LINTAKE}, -117);
		move({RINTAKE}, 117);
	} else if (cond == 0)
		move({LINTAKE,RINTAKE}, 0);
}
void moveAbsolute(std::vector<int> ports, float distance) {
    for(auto i : ports) {
        allMotors[i].move_absolute(distance, 100);
    }
}

/*void moveIntake() {
	
	if (MasterController.ButtonR2.pressing()) {
			allMotors[M1].move(50);
			allMotors[M2].move(-50);
		}
	else if (MasterController.ButtonR1.pressing()) {
			allMotors[M1].move(-50);
			allMotors[M2].move(50);
		}
	else {
			allMotors[M1].move(0);
			allMotors[M2].move(0);
		}
}*/

std::vector<double> getEncoders(std::vector<int> ports) {
    std::vector<double> returnVec;
    for(int i = 0; i < ports.size(); i++) {
        returnVec.push_back(allMotors[ports[i]].get_position());
    }
    return returnVec;
}

void motorTemp(void* param) {
	while (1) {	
		int overheatMotors = 0;
		for (int i = 0; i < 8; ++i) {
			if(allMotors[i].get_temperature() >= 55) {
				//pros::lcd::print(6, "Motor indexed at: %d is overheating", i);
				overheatMotors++;
				pros::delay(1000);
			}
		}
		if (!overheatMotors)
			//pros::lcd::print(6, "No motors overheating");
		pros::delay(2);
	}
}
