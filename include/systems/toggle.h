/**
 * @file include/systems/toggle.h
 * 
 * Contains definition for Toggle class that manages
 * togglable keys for operator controller
 */

#pragma once

#include "main.h"
#include <vector>

class Toggle {
    private:
    std::vector<okapi::ControllerDigital> buttons;  // A list of the buttons that need to be pressed to trigger the toggle
    okapi::Controller controller;                   // The V5 controller to take input from
    bool state;                                     // Current toggle state
    bool held = false;                              // If the buttons are currently pressed

    public:
    /**
     * @brief Check if the toggle has been triggered and update the state accordingly.
     * Should be called regularly while toggling is enabled.
     * 
     * @return int - 1 = toggled on, 0 = toggled off, -1 = no change
     */
    int checkState();

    /**
     * @brief Check if the toggle buttons are held down
     * 
     * @return true if they are being held
     * @return false otherwise
     */
    bool getHeld();

    /**
     * @brief Construct a new Toggle object
     * 
     * @param _buttons the buttons that together trigger the toggle
     * @param _controller the controller to poll for input
     * @param defaultState the starting state of the toggle
     */
    Toggle(std::vector<okapi::ControllerDigital> _buttons, okapi::Controller _controller, bool defaultState = false);
};