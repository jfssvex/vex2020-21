#pragma once

#include "pros/apix.h"
#include <string>
#include <vector>

enum {
    AUTO_RED_1,
    AUTO_RED_2,
    AUTO_BLUE_1,
    AUTO_BLUE_2,
    AUTO_SIMPLE,
    AUTO_DEPLOY
}; typedef int auton_route;

enum {
    LOG,
    WARNING,
    ERROR
}; typedef int logging_level;

enum {
    MATCH,
    SELECTOR,
    DEBUG
}; typedef int display_mode;

class DisplayController
{
public:
    // Constructor
    DisplayController();

    // Change display mode
    // Enabled when competition switch is connected
    void startSelectorMode();
    void showAutonSelected();
    void startMatchMode();

    // Enabled without competition switch
    void startDebugMode();

    // Logging
    void logMessage(std::string message, logging_level priority = DEBUG);
    void clearLogs();

    // Render loop initialized as task
    void render(void *param);

private:
    static bool initialized;
    std::vector<lv_obj_t*> logMessages;
    display_mode mode;
    
    // Render calls
    lv_obj_t* renderButton(int id, int x, int y, int h, int w, std::string labelText, lv_action_t action, lv_style_t *relStyle, lv_obj_t *host);
    lv_obj_t* renderLabel(std::string text, int x, int y, lv_obj_t * host);
    void renderWarning(std::string message);
    void renderError(std::string message);
    void renderDebug(std::string message);

    // Style initialization
    lv_theme_t* initTheme(lv_color_t borderColour, lv_color_t mainColour);
};