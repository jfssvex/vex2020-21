/**
 * @file include/displayController.h
 * 
 * Contains definition for DisplayController class and related
 * display data structures
 */

#pragma once

#include "pros/apix.h"
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

/**
 * Autonomous routine enumerator
 */
enum {
    AUTO_RED_1,
    AUTO_RED_2,
    AUTO_BLUE_1,
    AUTO_BLUE_2,
    AUTO_SIMPLE,
    AUTO_DEPLOY
}; typedef int auton_route;

/**
 * Logging level enumerator
 */
enum {
    LOG,
    WARNING,
    ERROR
}; typedef int logging_level;

/**
 * Display mode enumerator
 */
enum {
    MATCH,
    SELECTOR,
    DEBUG
}; typedef int display_mode;

/**
 * @brief Set the auto routine to run
 * 
 * @param route the routine code to set
 */
void setAuton(auton_route route);

/**
 * @brief Get the currently set auto routine
 * 
 * @return auton_route - the routine code
 */
auton_route getAuton();

/**
 * @brief A printf-style formatting function
 * 
 * @tparam Args
 * @param format The string with format specifiers
 * @param args A series of arguments to fill format specifiers
 * @return std::string - the formatted string
 */
template <typename... Args>
std::string string_format(const std::string &format, Args... args) {
    size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if (size <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

/**
 * Used to display fixed logging messages that are bound to a function
 * that can update the message's text.
 * For example, passing a callback that returns a tracking coordinate
 * should consistently update the message with the coordinate value
 * Currently very buggy!!!
 */
typedef struct fixed_debug_info {
    std::string format;
    void * callback;
    char type;
    
    /**
     * @brief Construct a new fixed debug info object
     * 
     * @param _format A string to print before the callback function value
     * @param _callback The callback to get the message contents
     * @param _type Format specifier for callback return. 'i' = integer, 'd' = double, 'b' = bool, 's' = string
     */
    fixed_debug_info(std::string _format, void * _callback, char _type) {
        this->format = _format;
        this->callback = _callback;
        this->type = _type;
    }

    /**
     * @brief Generates the message label
     * 
     * @return std::string - the label using callback value
     */
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

class DisplayController {
public:
    /**
     * @brief Construct a new Display Controller object
     */
    DisplayController();

    /**
     * @brief Get the current display mode
     * 
     * @return display_mode - the display mode enum
     */
    display_mode getMode();

    // Change display mode

    /**
     * @brief Display the auto selector screen.
     * Should be run when connected to competition switch.
     */
    void startSelectorMode();

    /**
     * @brief Display the auto routine that was selected
     */
    void showAutonSelected();

    /**
     * @brief Display the logo.
     * Should be run at the start of opcontrol/autonomous when connected
     * to competition switch.
     */
    void startMatchMode();

    /**
     * @brief Display custom debugging output.
     * Should be run when not connected to competition switch.
     */
    void startDebugMode();

    // Logging

    /**
     * @brief Log a message in the custom debugging message
     * 
     * @param message The message to display
     * @param priority The logging level of the message, defaults to LOG
     */
    void logMessage(std::string message, logging_level priority = LOG);

    /**
     * @brief Clear the logs currently on the screen
     */
    void clearLogs();

    /**
     * @brief Add a fixed logging message to the display
     * 
     * @param format A string to print before the callback function value
     * @param type Format specifier for callback return. 'i' = integer, 'd' = double, 'b' = bool, 's' = string
     * @param callback The callback to get the message contents
     */
    void addFixedMessage(std::string format, char type, void *callback);

private:
    static bool initialized;    // Check if controller has initialized itself
    display_mode mode;          // Current controller mode
    
    // Render calls

    /**
     * @brief Draw a button using LVGL
     * 
     * @param id ID to assign the button
     * @param x x-position
     * @param y y-position
     * @param h Height
     * @param w Width
     * @param labelText Text on button label
     * @param action Callback when the button is clicked
     * @param relStyle Style to render with
     * @param host LVGL object which will act as parent
     * @return lv_obj_t* - a reference to the new button
     */
    lv_obj_t* renderButton(int id, int x, int y, int h, int w, std::string labelText, lv_action_t action, lv_style_t *relStyle, lv_obj_t *host);

    /**
     * @brief Draw a label using LVGL
     * 
     * @param text Text on label
     * @param x x-position
     * @param y y-position
     * @param host LVGL object which will act as parent
     * @return lv_obj_t* - a reference to the new label
     */
    lv_obj_t* renderLabel(std::string text, int x, int y, lv_obj_t * host);

    /**
     * @brief Draw a warning message
     * 
     * @param message The message to display
     */
    void renderWarning(std::string message);

    /**
     * @brief Draw an error message
     * 
     * @param message The message to display
     */
    void renderError(std::string message);

    /**
     * @brief Draw a regular debug message
     * 
     * @param message The message to display
     */
    void renderDebug(std::string message);

    /**
     * @brief Initialize a new style
     * 
     * @param hue
     * @param borderColour
     * @param mainColour
     * @param borderWidth
     * @return lv_theme_t* - a reference to the new theme
     */
    lv_theme_t *initTheme(int hue, lv_color_t borderColour, lv_color_t mainColour, int borderWidth);
};