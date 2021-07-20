/**
 * @file src/systems/toggle.cpp
 * 
 * Contains Toggle class function definitions
 */

#include "toggle.h"

Toggle::Toggle(std::vector<okapi::ControllerDigital> _buttons, okapi::Controller _controller, bool defaultState) {
    this->controller = _controller;
    this->buttons = _buttons;
    this->state = defaultState;
}

int Toggle::checkState() {
    for(int i = 0; i < buttons.size(); i++) {
        if(!controller.getDigital(buttons[i])) {
            held = false;
            return -1;
        }
    }

    if(!held) {
        state = !state;
    }
    held = true;
    return state ? 1 : 0;
}

bool Toggle::getHeld() {
    return this->held;
}