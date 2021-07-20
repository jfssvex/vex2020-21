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
    // Check if any of the buttons aren't pressed and return if they're not
    for(int i = 0; i < buttons.size(); i++) {
        if(!controller.getDigital(buttons[i])) {
            held = false;
            return -1;
        }
    }

    // Swap the state and then return it if the function hasn't exited yets
    if(!held) {
        state = !state;
    }
    held = true;
    return state ? 1 : 0;
}

bool Toggle::getHeld() {
    return this->held;
}