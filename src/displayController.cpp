#include "displayController.h"

lv_color_t red = LV_COLOR_MAKE(234, 35, 58);
lv_color_t blue = LV_COLOR_MAKE(41, 130, 198);
lv_color_t purple = LV_COLOR_MAKE(35, 44, 101);

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
    if(initialized) {
        return;
    }
    initialized = true;
}