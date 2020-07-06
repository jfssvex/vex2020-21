#pragma once

#include "apix.h"
#include <string>
#include <vector>

enum {
    AUTON_RED_1,
    AUTON_RED_2,
    AUTON_BLUE_1,
    AUTON_BLUE_2,
    AUTON_SIMPLE,
    DEPLOY
}; typedef int auton_options;

enum {
    DEBUG,
    WARNING,
    ERROR
}; typedef int logging_levels;

class DisplayController
{
public:
    // Constructor
    DisplayController();

    // Change display mode
    void startDebugMode();
    void startMatchMode();

    // Logging
    void logMessage(std::string message, logging_levels priority = DEBUG);
    void clear();

    // Render loop initialized as task
    void render(void *param);

private:
    static bool initialized;
    std::vector<lv_obj_t*> logMessages;
    
    // Render calls
    void renderButton();
    void renderWarning(std::string message);
    void renderError(std::string message);
    void renderDebug(std::string message);    
};