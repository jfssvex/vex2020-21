#include "displayController.h"
#include "autonSelector.h"
#include "globals.h"
#include <string>

const int button_h = 60;
const int button_w = 200;

bool DisplayController::initialized = false;

// C suffix temporary until autonSelector.cpp removed
lv_color_t redC = LV_COLOR_MAKE(234, 35, 58);
lv_color_t blueC = LV_COLOR_MAKE(41, 130, 198);
lv_color_t purpleC = LV_COLOR_MAKE(104, 11, 128);

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
    setAutonId(autoID);
    display.showAutonSelected();

    return LV_RES_OK;
}

static lv_res_t nullButtonCallback(lv_obj_t *btn){ return LV_RES_OK; }

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

void DisplayController::logMessage(std::string message, logging_level priority) {
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
    redStyle.body.main_color = redC;
    redStyle.body.grad_color = redC;
    redStyle.body.radius = 4;
    redStyle.body.border.width = 0;
    redStyle.body.border.color = redC;
    redStyle.body.padding.ver = 5;
    redStyle.body.padding.hor = 5;
    redStyle.body.padding.inner = 0;
    redStyle.body.shadow.width = 0;
    redStyle.body.shadow.color = redC;
    redStyle.text.color = LV_COLOR_WHITE;
    //	redPreChosen.text.font = &GOTHAM_20;

    lv_style_copy(&mBoxStyle, &redStyle);
    mBoxStyle.body.main_color = LV_COLOR_WHITE;
    mBoxStyle.body.grad_color = LV_COLOR_WHITE;
    mBoxStyle.text.color = LV_COLOR_BLACK;

    lv_style_copy(&blueStyle, &redStyle);
    blueStyle.body.main_color = blueC;
    blueStyle.body.grad_color = blueC;

    lv_style_copy(&neutralStyle, &blueStyle);
    neutralStyle.body.main_color = purpleC;
    neutralStyle.body.grad_color = purpleC;

    // mainTheme = initTheme(LV_COLOR_MAKE(35, 44, 101), LV_COLOR_WHITE);

    // colouredBackground = initTheme(purpleC, LV_COLOR_WHITE);

    mainTheme = lv_theme_alien_init(50, NULL);
    mainTheme->win.bg->body.main_color = LV_COLOR_WHITE;
    mainTheme->win.bg->body.grad_color = LV_COLOR_WHITE;
    mainTheme->win.bg->body.border.color = LV_COLOR_MAKE(35, 44, 101);
    mainTheme->win.bg->body.border.width = 10;

    colouredBackground = lv_theme_alien_init(10, NULL);
    colouredBackground->win.bg->body.border.color = LV_COLOR_WHITE;
    colouredBackground->win.bg->body.border.width = 10;
    colouredBackground->win.bg->body.border.color = purpleC;
    colouredBackground->win.bg->body.main_color = LV_COLOR_WHITE;
    colouredBackground->win.bg->body.grad_color = LV_COLOR_WHITE;
}

lv_obj_t* DisplayController::renderLabel(std::string text, int x, int y, lv_obj_t * host) {
    lv_obj_t *label = lv_label_create(host, NULL);
    lv_obj_align(label, host, LV_ALIGN_CENTER, x, y);
    lv_label_set_text(label, text.c_str());

    return label;
}

lv_obj_t *DisplayController::renderButton(int id, int x, int y, int h, int w, std::string labelText, lv_action_t action, lv_style_t *relStyle, lv_obj_t *host) {
    lv_obj_t *button = lv_btn_create(host, NULL);

    lv_obj_set_pos(button, x, y);
    // lv_obj_align(button, host, LV_ALIGN_CENTER, x, y);
    // lv_obj_align(button, scr, LV_ALIGN_IN_TOP_LEFT, x, y);
    lv_obj_set_size(button, w, h);

    lv_obj_set_free_num(button, id);
    lv_btn_set_style(button, LV_BTN_STYLE_REL, relStyle);
    lv_btn_set_action(button, LV_BTN_ACTION_CLICK, action);

    lv_obj_t *buttonLabel = renderLabel(labelText, 0, 0, button);
    lv_obj_align(buttonLabel, button, LV_ALIGN_CENTER, 0, 0);

    return button;
}

void DisplayController::startSelectorMode() {
    this->mode = SELECTOR;
    lv_theme_set_current(mainTheme);
    scr = lv_page_create(NULL, NULL);
    lv_scr_load(scr);
    lv_page_set_sb_mode(scr, LV_SB_MODE_OFF);

    // renderButton(AUTO_RED_1, 0, 100, button_h, button_w, "Unprotected Zone \n(5-point)", autonSelected, &redStyle, scr);
    // renderButton(AUTO_RED_2, LV_HOR_RES / 2, 0, button_h, button_w, "Protected Zone \n(5-point)", autonSelected, &redStyle, scr);

    renderButton(AUTO_RED_2, 0, 0, button_h, button_w, "Unprotected zone \n(5-point)", autonSelected, &redStyle, scr);
    renderButton(AUTO_BLUE_1, 0, LV_VER_RES - 80, button_h, button_w, "Unprotected Zone\n(5-point)", autonSelected, &blueStyle, scr);
    renderButton(AUTO_BLUE_2, LV_HOR_RES/2, LV_VER_RES - 80, button_h, button_w, "Protected Zone\n(5-point)", autonSelected, &blueStyle, scr);
    renderButton(AUTO_DEPLOY, 0, LV_VER_RES/2 - 30, button_h, button_w, "Deploy", autonSelected, &neutralStyle, scr);
    renderButton(AUTO_SIMPLE, LV_HOR_RES/2, LV_VER_RES/2 - 30, button_h, button_w, "One ball", autonSelected, &neutralStyle, scr);
    renderButton(AUTO_RED_1, LV_HOR_RES / 2, 20, button_h, button_w, "Protected Zone \n(5-point)", autonSelected, &redStyle, scr);
}

// Only called after autonomous is selected
void DisplayController::showAutonSelected() {
    this->mode = SELECTOR;
    lv_obj_clean(scr);
    lv_theme_set_current(colouredBackground);
    scr = lv_page_create(NULL, NULL);
    lv_scr_load(scr);
    lv_page_set_sb_mode(scr, LV_SB_MODE_OFF);

    auton_options autoID = getAutonId();
    std::string autoName;
    switch(autoID) {
        case (AUTON_RED_PROTECTED):
            autoName = "Red protected routine";
            break;
        case (AUTON_RED_UNPROTECTED):
            autoName = "Red unprotected routine";
            break;
        case (AUTON_BLUE_PROTECTED):
            autoName = "Blue protected routine";
            break;
        case (AUTON_BLUE_UNPROTECTED):
            autoName = "Blue unprotected routine";
            break;
        case (SIMPLE_ONE_CUBE):
            autoName = "One cube routine";
            break;
        case (FLIPOUT):
            autoName = "Deploy routine";
            break;
        default:
            autoName = "Unknown routine selected";
            break;
    }
    renderButton(-1, 80, 50, 150, 300, autoName, nullButtonCallback, &neutralStyle, scr);
}