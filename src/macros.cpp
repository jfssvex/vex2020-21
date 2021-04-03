#include "macros.h"
#include "globals.h"
#include "main.h"
#include "motionControl.h"

bool suspendDrive = false;

int callbackCount = 0;

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

void shootClean() {
    rollers.eject();
}

void shootStaggered() {
    rollers.shoot();
}

void shootStaggeredIntake() {
    rollers.shoot();
    intake.intake(127);
}