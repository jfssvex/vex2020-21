#pragma once

#include "main.h"
#include "systemManager.h"
#include "chassis.h"

#define DEFAULT_ROLLER_SPEED 127
#define ROLLER_HOLD_SPEED 80

class Rollers : public SystemManager
{
public:
    static const uint8_t IDLE_STATE = 0x10;
    static const uint8_t IN_STATE = 0x11;
    static const uint8_t OUT_STATE = 0x12;
    static const uint8_t EJECT_STATE = 0x13;
    static const uint8_t SHOOT_STATE = 0x15;
    static const uint8_t HOLD_STATE = 0x16;

    void intake();
    void out();
    void eject();
    void shoot();
    void stop();

    void update() override;
    void fullReset() override;

    Rollers(uint8_t _defaultState, okapi::Controller _controller);

private:
    void setPower(double _power);
    void setBrakeMode(enum pros::motor_brake_mode_e mode);
    bool changeState(uint8_t newState) override;
    pros::Motor botRollerMotor = pros::Motor(BOTROLLER_PORT);
    pros::Motor topRollerMotor = pros::Motor(TOPROLLER_PORT);
    okapi::Controller controller;
};
