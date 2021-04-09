#include "macros.h"
#include "globals.h"
#include "main.h"
#include "motionControl.h"

bool suspendDrive = false;

int callbackCount = 0;

const int initialShotDelay = 600;         // ms
const int subsequentShotDelay = 400;    // ms

void flipout() {
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
    auto d = initialShotDelay + (subsequentShotDelay * (balls - 1));
    pros::delay(d);
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