#include "displayController.h"
#include <string>

const int button_h = 25;
const int button_w = 40;

lv_color_t red = LV_COLOR_MAKE(234, 35, 58);
lv_color_t blue = LV_COLOR_MAKE(41, 130, 198);
lv_color_t purple = LV_COLOR_MAKE(35, 44, 101);

// Pop-up (message box) style
lv_style_t mBoxStyle;
// Button styles
lv_style_t redStyle;
lv_style_t blueStyle;
lv_style_t neutralStyle;

// Page themes
lv_theme_t *mainTheme;
lv_theme_t *colouredBackground;

// Page object
static lv_obj_t *scr;

static lv_res_t autonSelected(lv_obj_t *btn) {
    int autoID = lv_obj_get_free_num(btn);
    // Some code to set autonomous to autoID

    return LV_RES_OK;
}

// Might cause errors with pointers !!!!!
lv_theme_t* DisplayController::initTheme(lv_color_t borderColour, lv_color_t mainColour) {
    lv_theme_t* theme;
    theme = lv_theme_alien_init(10, NULL);
    theme->win.bg->body.border.color = LV_COLOR_WHITE;
    theme->win.bg->body.border.width = 10;
    theme->win.bg->body.border.color = borderColour;
    theme->win.bg->body.main_color = mainColour;
    theme->win.bg->body.grad_color = mainColour;

    return theme;
}

void DisplayController::clearLogs() {
    for(auto i : this->logMessages) {
        lv_obj_del(i);
    }
}

void DisplayController::logMessage(std::string message, logging_level priority = DEBUG) {
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

    // Set up styles
    lv_style_copy(&redStyle, &lv_style_plain_color);
    redStyle.body.main_color = red;
    redStyle.body.grad_color = red;
    redStyle.body.radius = 4;
    redStyle.body.border.width = 0;
    redStyle.body.border.color = red;
    redStyle.body.padding.ver = 5;
    redStyle.body.padding.hor = 5;
    redStyle.body.padding.inner = 0;
    redStyle.body.shadow.width = 0;
    redStyle.body.shadow.color = red;
    redStyle.text.color = LV_COLOR_WHITE;
    //	redPreChosen.text.font = &GOTHAM_20;

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

    mainTheme = initTheme(LV_COLOR_MAKE(35, 44, 101), LV_COLOR_WHITE);

    colouredBackground = initTheme(purple, LV_COLOR_WHITE);
}

lv_obj_t* DisplayController::renderLabel(std::string text, int x, int y, lv_obj_t * host) {
    lv_obj_t *label = lv_label_create(host, NULL);
    lv_obj_set_pos(label, x, y);
    lv_label_set_text(label, text.c_str());

    return label;
}

lv_obj_t *DisplayController::renderButton(int id, int x, int y, int h, int w, std::string labelText, lv_action_t action, lv_style_t *relStyle, lv_obj_t *host) {
    lv_obj_t * button = lv_btn_create(host, NULL);
    lv_obj_t * buttonLabel = renderLabel(labelText, 0, 0, button);
    lv_obj_align(buttonLabel, button, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_free_num(button, id);
    lv_btn_set_style(button, LV_BTN_STYLE_REL, relStyle);
    lv_obj_set_pos(button, x, y);
    lv_obj_set_size(button, w, h);
    lv_btn_set_action(button, LV_BTN_ACTION_CLICK, action);

    return button;
}

void DisplayController::startSelectorMode() {
    this->mode = SELECTOR;
    lv_theme_set_current(mainTheme);
    scr = lv_page_create(NULL, NULL);
    lv_scr_load(scr);
    lv_page_set_sb_mode(scr, LV_SB_MODE_OFF);

    renderButton(AUTON_RED_1, -100, -100, button_h, button_w, "Unprotected Zone \n(5-point)", autonSelected, &redStyle, scr);
    renderButton(AUTON_RED_2, -100, 90, button_h, button_w, "Protected Zone \n(5-point)", autonSelected, &redStyle, scr);
    renderButton(AUTON_BLUE_1, 100, 0, button_h, button_w, "Unprotected Zone \n(5-point)", autonSelected, &blueStyle, scr);
    renderButton(AUTON_BLUE_2, 100, 90, button_h, button_w, "Protected Zone \n(5-point)", autonSelected, &blueStyle, scr);
    renderButton(AUTON_SIMPLE, 100, 0, button_h, button_w, "One ball", autonSelected, &neutralStyle, scr);
    renderButton(AUTON_DEPLOY, -100, 0, button_h, button_w, "Deploy", autonSelected, &neutralStyle, scr);
}