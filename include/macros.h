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

extern bool trayUp;
extern bool stacking;
extern bool lifting;
extern bool suspendDrive;
