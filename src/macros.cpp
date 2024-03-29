/**
 * @file src/macros.cpp
 * 
 * Contains definitions for macro functions to automate
 * tasks on the field for use in driver or autonomous
 * control periods.
 */

#include "macros.h"
#include "globals.h"
#include "main.h"
#include "motionControl.h"

bool suspendDrive = false;

int callbackCount = 0;

const int initialShotDelay = 500;       // ms
const int subsequentShotDelay = 1000;    // ms

void flipout() {
    // Run flipout routine from state machine
    rollers.flipout();
}

void in() {
    rollers.intake();
    intake.intake(127);
}

void stopRollers() {
    rollers.stop();
    intake.stop();
}

void ballDelay(uint8_t balls) {
    // Calculate delay based on how many balls are needed
    // First ball is always faster because of second ball-delay
    auto d = initialShotDelay + (subsequentShotDelay * (balls - 1));
    pros::delay(d);

    printf("%i BALL DELAY \n", balls);
}

void shootClean(uint8_t balls) {
    rollers.eject();
    ballDelay(balls);
}

void shootCleanIntake(uint8_t balls) {
    rollers.eject();
    intake.intake(127);
    ballDelay(balls);
}

void shootStaggered(uint8_t balls) {
    rollers.shoot();
    ballDelay(balls);
}

void shootStaggeredIntake(uint8_t balls) {
    rollers.shoot();
    intake.intake(127);
    ballDelay(balls);
}

void outtake(uint8_t balls) {
    intake.out(127);
    rollers.out();
    ballDelay(balls);
    pros::delay(200);
    stopRollers();
}