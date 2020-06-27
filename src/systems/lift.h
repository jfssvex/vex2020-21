#pragma once

#include "main.h"
#include "systemManager.h"
#include "intake.h"
#include "tray.h"
#include "chassis.h"

class Lift : public SystemManager
{
public:
    static const uint8_t IDLE_STATE = 0x10;
    static const uint8_t LIFT_STATE = 0x11;
    static const uint8_t LOWER_STATE = 0x12;
    static const uint8_t HOLD_STATE = 0x13;

    void move(bool up);
    void lock();
    void moveTo(double _target, void(*_callback)());
    void setCallback(void(*_callback)());

    void update() override;
    void fullReset() override;
    uint8_t getState();
    Lift(uint8_t _defaultState);
    
private:
    void stop();
    void setTarget(double target);
    bool changeState(uint8_t newState) override;

    void(*callback)(){nullCallback};
    bool lockState = false;
    pros::Motor liftMotor = pros::Motor(LIFT_PORT);
    bool mid = false;
};
