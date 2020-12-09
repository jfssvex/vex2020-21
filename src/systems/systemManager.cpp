#include "main.h"
#include "systemManager.h"

// Constructor
SystemManager::SystemManager(uint8_t _defaultState) {
    this->state = _defaultState;
    this->lastState = _defaultState;
    this->defaultState = _defaultState;
}

// Fixed public functions
double SystemManager::getError() {
    return this->error;
}
double SystemManager::getPosition() {
    return this->position;
}
double SystemManager::getPower() {
    return this->power;
}
double SystemManager::getTarget() {
    return this->target;
}

void SystemManager::reset() {
    this->changeState(RESET_STATE);
}
bool SystemManager::enabled() {
    return this->state != DISABLED_STATE;
}
void SystemManager::disable() {
    this->changeState(DISABLED_STATE);
}
void SystemManager::revertState() {
    this->changeState(lastState);
}

uint8_t SystemManager::getState() {
	return this->state;
}

// Fixed protected functions
bool SystemManager::timedOut(uint32_t timeout) {
    return this->timeOfLastChange + timeout < pros::millis();
}

void SystemManager::enable() {
    this->lastState = DISABLED_STATE;
    this->state = this->defaultState;
    this->timeOfLastChange = pros::millis();
}

// Virtual functions
bool SystemManager::changeState(uint8_t newState) {
    if(this->state == DISABLED_STATE) {
        return false;
    }
    this->lastState = this->state;
    this->state = newState;
    this->timeOfLastChange = pros::millis();
    return true;
}

void SystemManager::fullReset() {
    this->target = 0;
    this->error = 0;
    this->position = 0;
    this->power = 0;
}