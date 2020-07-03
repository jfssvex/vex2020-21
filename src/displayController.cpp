#include "displayController.h"

void DisplayController::clear() {
    for(auto i : this->logMessages) {
        lv_obj_del(i);
    }
}

void DisplayController::logMessage(std::string message, logging_levels priority = DEBUG) {
    if(priority == ERROR) {
        this->renderError(message);
    }
    else if(priority == WARNING) {
        this->renderWarning(message);
    }
    else {
        this->renderDebug(message);
    }
}

DisplayController::DisplayController() {
    
}