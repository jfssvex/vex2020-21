#pragma once

#include "main.h"

void stackCubes();
void disengageStack();
void dropTray();
void halfStack();
void liftToMid();
void liftToLow();
void dropLift();
void flipout();
void incrementLift(int dir);

void in();
void stopRollers();
void shootClean();
void shootStaggered();
void shootStaggeredIntake();

extern bool trayUp;
extern bool stacking;
extern bool lifting;
extern bool suspendDrive;
