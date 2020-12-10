#pragma once

#include "main.h"
#include <vector>

class Toggle {
    private:
    std::vector<okapi::ControllerDigital> buttons;
    okapi::Controller controller;
    bool state;
    bool held = false;

    public:
    int checkState();
    bool getHeld();
    Toggle(std::vector<okapi::ControllerDigital> _buttons, okapi::Controller _controller, bool defaultState = false);
};