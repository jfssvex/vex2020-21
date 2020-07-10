#pragma once

#include "pros/apix.h"
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

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

void setAuton(auton_route route);
auton_route getAuton();

template <typename... Args>
std::string string_format(const std::string &format, Args... args)
{
    size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if (size <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

typedef struct fixed_debug_info {
    std::string format;
    void * callback;
    char type;
    
    fixed_debug_info(std::string _format, void * _callback, char _type) {
        this->format = _format;
        this->callback = _callback;
        this->type = _type;
    }

    std::string getLabel() {
        std::string output;
        switch(this->type) {
            case 'i':
                output = std::to_string(*(int*)callback); break;
            case 'd':
                output = std::to_string(*(double*)callback); break;
            case 'b':
                output = std::to_string(*(bool*)callback); break;
            case 's':
                output = (*(std::string*)callback); break;
            default:
                output = std::to_string(*(double*)callback); break;
        }
        return this->format + output;
    }
} fixed_debug_info;

class DisplayController
{
public:
    // Constructor
    DisplayController();
    display_mode getMode();

    // Change display mode

    // Enabled when competition switch is connected
    void startSelectorMode();
    void showAutonSelected();
    void startMatchMode();

    // Enabled without competition switch
    void startDebugMode();

    // Logging
    void logMessage(std::string message, logging_level priority = LOG);
    void clearLogs();
    void addFixedMessage(std::string format, char type, void *callback);

private:
    static bool initialized;
    display_mode mode;
    
    // Render calls
    lv_obj_t* renderButton(int id, int x, int y, int h, int w, std::string labelText, lv_action_t action, lv_style_t *relStyle, lv_obj_t *host);
    lv_obj_t* renderLabel(std::string text, int x, int y, lv_obj_t * host);
    void renderWarning(std::string message);
    void renderError(std::string message);
    void renderDebug(std::string message);

    // Style initialization
    lv_theme_t *initTheme(int hue, lv_color_t borderColour, lv_color_t mainColour, int borderWidth);
};