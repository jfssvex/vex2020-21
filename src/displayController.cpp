/**
 * @file src/displayController.cpp
 * 
 * Contains logic for the display controller to manage the V5
 * LCD screen with autonomous selector, logger, and logo display
 * during match.
 * 
 * Logger still has some bugs.
 */

#include "displayController.h"
#include "globals.h"
#include <string>

// Constants
const int button_h = 60;
const int button_w = 200;
const int maxMessages = 7;

bool DisplayController::initialized = false;
auton_route selectedRoute = AUTO_DEPLOY;

// C suffix temporary until autonSelector.cpp removed
lv_color_t red = LV_COLOR_MAKE(234, 35, 58);
lv_color_t blue = LV_COLOR_MAKE(41, 130, 198);
lv_color_t purple = LV_COLOR_MAKE(88, 36, 133);
lv_color_t grey = LV_COLOR_MAKE(69, 69, 69);
lv_color_t yellow = LV_COLOR_MAKE(255, 210, 72);

// Lists for debug menu
std::vector<lv_obj_t *> logMessages;
std::vector<lv_obj_t *> fixedMessages;
std::vector<fixed_debug_info> fixedMessageData;

// Pop-up (message box) style
lv_style_t mBoxStyle;
// Button styles
lv_style_t redStyle;
lv_style_t blueStyle;
lv_style_t neutralStyle;
lv_style_t logStyle;
lv_style_t warningStyle;
// Page style
lv_style_t pageStyle;

// Page themes
lv_theme_t *mainTheme;
lv_theme_t *colouredBackground;
lv_theme_t *borderless;

// Page object
static lv_obj_t *scr;
// List object for messages
lv_obj_t *messsageList;

// Callback function for when an auto routine selected. Updates autoID and changes screen
static lv_res_t autonSelected(lv_obj_t *btn) {
    int autoID = lv_obj_get_free_num(btn);
    setAuton(autoID);
    display.showAutonSelected();

    return LV_RES_OK;
}

static lv_res_t nullButtonCallback(lv_obj_t *btn){ return LV_RES_OK; }

void setAuton(auton_route route) {
    selectedRoute = route;
}

auton_route getAuton() {
    return selectedRoute;
}

// Task to update the labels on fixed messages. Low priority.
void updateFixedMessages(void *param) {
    while(true) {
        for(int i = 0; i < fixedMessages.size(); i++) {
            lv_label_set_text(lv_obj_get_child(fixedMessages[i], NULL), fixedMessageData[i].getLabel().c_str());
        }
        pros::delay(20);
    }
}

// Might cause errors with pointers !!!!! I don't think it did tho
lv_theme_t* DisplayController::initTheme(int hue, lv_color_t borderColour, lv_color_t mainColour, int borderWidth) {
    lv_theme_t* theme;
    theme = lv_theme_alien_init(hue, NULL);
    theme->win.bg->body.border.color = LV_COLOR_WHITE;
    theme->win.bg->body.border.width = borderWidth;
    theme->win.bg->body.border.color = borderColour;
    theme->win.bg->body.main_color = mainColour;
    theme->win.bg->body.grad_color = mainColour;

    return theme;
}

display_mode DisplayController::getMode() {
    return this->mode;
}

void DisplayController::logMessage(std::string message, logging_level priority) {
    // Only allow logging if its an error if the controller is in display mode
    if(this->mode != DEBUG && priority != ERROR) {
        return;
    }

    // Create the new message object
    lv_obj_t * newMessage = lv_list_add(messsageList, NULL, message.c_str(), nullButtonCallback);

    // Customise message according to logging level
    lv_style_t messageStyle;
    switch(priority) {
        case(LOG):
            lv_btn_set_style(newMessage, LV_BTN_STYLE_REL, &logStyle);
            break;
        case(WARNING):
            lv_btn_set_style(newMessage, LV_BTN_STYLE_REL, &warningStyle);
            break;
        case(ERROR):
            lv_btn_set_style(newMessage, LV_BTN_STYLE_REL, &redStyle);
            break;
        default:
            lv_btn_set_style(newMessage, LV_BTN_STYLE_REL, &logStyle);
            break;
    }
    logMessages.push_back(newMessage);

    if(logMessages.size() + fixedMessages.size() > maxMessages) {
        lv_obj_del(logMessages[0]);
    }
}

void DisplayController::addFixedMessage(std::string format, char type, void *callback) {
    // Don't remember how this function was supposed to work
    fixed_debug_info info(format, callback, type);
    fixedMessageData.push_back(info);
}

void DisplayController::clearLogs() {
    for(auto i : logMessages) {
        lv_obj_del(i);
    }
}

DisplayController::DisplayController() {
    // Make sure this object is a singleton. Don't want conflicting controllers
    if(initialized) {
        return;
    }
    initialized = true;

    // Set up styles
    lv_style_copy(&redStyle, &lv_style_plain_color);
    redStyle.body.main_color = red;
    redStyle.body.grad_color = red;
    redStyle.body.radius = 4;
    redStyle.body.border.width = 2;
    redStyle.body.border.color = LV_COLOR_WHITE;
    redStyle.body.padding.ver = 5;
    redStyle.body.padding.hor = 5;
    redStyle.body.padding.inner = 0;
    redStyle.body.shadow.width = 0;
    redStyle.body.shadow.color = red;
    redStyle.text.color = LV_COLOR_WHITE;
    //	redPreChosen.text.font = &GOTHAM_20;

    lv_style_copy(&logStyle, &redStyle);
    logStyle.body.main_color = grey;
    logStyle.body.grad_color = grey;

    lv_style_copy(&warningStyle, &redStyle);
    warningStyle.body.main_color = yellow;
    warningStyle.body.grad_color = yellow;
    warningStyle.text.color = LV_COLOR_BLACK;

    lv_style_copy(&mBoxStyle, &redStyle);
    mBoxStyle.body.main_color = LV_COLOR_WHITE;
    mBoxStyle.body.grad_color = LV_COLOR_WHITE;
    mBoxStyle.text.color = LV_COLOR_BLACK;

    lv_style_copy(&blueStyle, &redStyle);
    blueStyle.body.main_color = blue;
    blueStyle.body.grad_color = blue;

    lv_style_copy(&neutralStyle, &blueStyle);
    neutralStyle.body.main_color = purple;
    neutralStyle.body.grad_color = purple;

    colouredBackground = initTheme(10, purple, purple, 0);
}

lv_obj_t* DisplayController::renderLabel(std::string text, int x, int y, lv_obj_t * host) {
    lv_obj_t *label = lv_label_create(host, NULL);      // Create an LVGl label
    lv_obj_align(label, host, LV_ALIGN_CENTER, x, y);   // Align it to the position
    lv_label_set_text(label, text.c_str());             // Set the label text

    return label;
}

lv_obj_t *DisplayController::renderButton(int id, int x, int y, int h, int w, std::string labelText, lv_action_t action, lv_style_t *relStyle, lv_obj_t *host) {
    // Create the LVGL button parented to the host
    lv_cont_set_fit(host, false, false);
    lv_obj_t *button = lv_btn_create(host, NULL);

    // Position appropriately
    lv_obj_set_pos(button, x, y);
    lv_obj_set_size(button, w, h);

    // Set ID and style
    lv_obj_set_free_num(button, id);
    lv_btn_set_style(button, LV_BTN_STYLE_REL, relStyle);
    lv_btn_set_action(button, LV_BTN_ACTION_CLICK, action);

    // Set the label and return the object
    lv_obj_t *buttonLabel = renderLabel(labelText, 0, 0, button);
    lv_obj_align(buttonLabel, button, LV_ALIGN_CENTER, 0, 0);

    return button;
}

void DisplayController::startSelectorMode() {
    this->mode = SELECTOR; // Update state
    // Set themes and load page
    lv_theme_set_current(colouredBackground);
    scr = lv_page_create(NULL, NULL);
    lv_scr_load(scr);
    lv_page_set_sb_mode(scr, LV_SB_MODE_OFF);

    // Draw a button for each routine
    renderButton(AUTO_RED_1, LV_HOR_RES / 2, 0, button_h, button_w, "Protected zone \n(5-point)", autonSelected, &redStyle, scr);
    renderButton(AUTO_BLUE_1, 0, LV_VER_RES - 80, button_h, button_w, "Unprotected Zone\n(5-point)", autonSelected, &blueStyle, scr);
    renderButton(AUTO_BLUE_2, LV_HOR_RES/2, LV_VER_RES - 80, button_h, button_w, "Protected Zone\n(5-point)", autonSelected, &blueStyle, scr);
    renderButton(AUTO_DEPLOY, 0, LV_VER_RES/2 - 30, button_h, button_w, "Deploy", autonSelected, &neutralStyle, scr);
    renderButton(AUTO_SIMPLE, LV_HOR_RES/2, LV_VER_RES/2 - 30, button_h, button_w, "One ball", autonSelected, &neutralStyle, scr);
    renderButton(AUTO_RED_2, 0, 20, button_h, button_w, "Unprotected Zone \n(5-point)", autonSelected, &redStyle, scr);
}

void DisplayController::showAutonSelected() {
    this->mode = SELECTOR;
    // Clear selector screen and create a new one
    lv_obj_clean(scr);
    scr = lv_page_create(NULL, NULL);
    lv_scr_load(scr);
    lv_page_set_sb_mode(scr, LV_SB_MODE_OFF);

    // Draw a button based on the auto that was selected
    auton_route autoID = getAuton();
    std::string autoName;
    switch(autoID) {
        case (AUTO_RED_1):
            autoName = "Red protected routine";
            break;
        case (AUTO_RED_2):
            autoName = "Red unprotected routine";
            break;
        case (AUTO_BLUE_1):
            autoName = "Blue protected routine";
            break;
        case (AUTO_BLUE_2):
            autoName = "Blue unprotected routine";
            break;
        case (AUTO_SIMPLE):
            autoName = "One cube routine";
            break;
        case (AUTO_DEPLOY):
            autoName = "Deploy routine";
            break;
        default:
            autoName = "Unknown routine selected";
            break;
    }
    renderButton(-1, 80, 50, 150, 300, autoName, nullButtonCallback, &neutralStyle, scr);
}

// pcfs functions are used to open and display the binary image on the SD card
// for the match mode. I don't understand exactly how they work.
typedef FILE *pc_file_t;

static lv_fs_res_t pcfs_open(void *file_p, const char *fn, lv_fs_mode_t mode)
{
    errno = 0;
    const char *flags = "";
    if (mode == LV_FS_MODE_WR)
        flags = "wb";
    else if (mode == LV_FS_MODE_RD)
        flags = "rb";
    else if (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD))
        flags = "a+";

    char buf[256];
    sprintf(buf, "/%s", fn);
    pc_file_t f = fopen(buf, flags);

    if (f == NULL)
        return LV_FS_RES_UNKNOWN;
    else
    {
        fseek(f, 0, SEEK_SET);
        pc_file_t *fp = (pc_file_t *)file_p;
        *fp = f;
    }

    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_close(void *file_p)
{
    pc_file_t *fp = (pc_file_t *)file_p;
    fclose(*fp);
    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_read(void *file_p, void *buf, uint32_t btr, uint32_t *br)
{
    pc_file_t *fp = (pc_file_t *)file_p;
    *br = fread(buf, 1, btr, *fp);
    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_seek(void *file_p, uint32_t pos)
{
    pc_file_t *fp = (pc_file_t *)file_p;
    fseek(*fp, pos, SEEK_SET);
    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_tell(void *file_p, uint32_t *pos_p)
{
    pc_file_t *fp = (pc_file_t *)file_p;
    *pos_p = ftell(*fp);
    return LV_FS_RES_OK;
}

void DisplayController::startMatchMode() {
    // Set up state and clear screen if there was something on it (selector is the only other thing that could have run)
    if(this->mode == SELECTOR) {
        lv_obj_clean(scr);
    }
    this->mode = MATCH;
    scr = lv_page_create(NULL, NULL);
    lv_scr_load(scr);
    lv_page_set_sb_mode(scr, LV_SB_MODE_OFF);

    // Use pcfs functions to read in the binary image and display it in LVGL
    lv_fs_drv_t pcfs_drv;
    memset(&pcfs_drv, 0, sizeof(lv_fs_drv_t));

    pcfs_drv.file_size = sizeof(pc_file_t);
    pcfs_drv.letter = 'S';
    pcfs_drv.open = pcfs_open;
    pcfs_drv.close = pcfs_close;
    pcfs_drv.read = pcfs_read;
    pcfs_drv.seek = pcfs_seek;
    pcfs_drv.tell = pcfs_tell;
    lv_fs_add_drv(&pcfs_drv);

    lv_obj_t *img_var = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img_var, "S:/usd/royals_logo.bin");
    lv_obj_set_size(img_var, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(img_var, 0, -10);

    messsageList = lv_list_create(lv_scr_act(), NULL);
    lv_obj_set_width(messsageList, LV_HOR_RES - 40);
}

void DisplayController::startDebugMode() {
    // Set state and create LVGL screen
    this->mode = DEBUG;
    lv_theme_set_current(colouredBackground);
    scr = lv_page_create(NULL, NULL);
    lv_scr_load(scr);
    lv_page_set_sb_mode(scr, LV_SB_MODE_OFF);

    // Initialize list of logging messages
    messsageList = lv_list_create(lv_scr_act(), NULL);
    lv_obj_set_width(messsageList, LV_HOR_RES - 40);

    // Create blocks for every fixed message that was inserted
    for(int i = 0; i < fixedMessageData.size(); i++) {
        fixedMessages.push_back(lv_list_add(messsageList, NULL, fixedMessageData[i].getLabel().c_str(), nullButtonCallback));
        this->logMessage(std::to_string(lv_obj_count_children(fixedMessages[i])), ERROR);
        lv_obj_set_style(fixedMessages[i], &logStyle);
    }

    // Start the task to update fixed messages
    pros::Task udpateMessages(updateFixedMessages, (void *)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Updating Messages");
}