#include "displayController.h"
#include "autonSelector.h"
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

    mainTheme = initTheme(LV_COLOR_MAKE(35, 44, 101), LV_COLOR_WHITE);

    colouredBackground = initTheme(purpleC, LV_COLOR_WHITE);
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
    // lv_obj_set_pos(button, x, y);
    lv_obj_align(button, host, LV_ALIGN_CENTER, x, y);
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

    renderButton(AUTO_RED_1, -150, -100, button_h, button_w, "Unprotected Zone \n(5-point)", autonSelected, &redStyle, scr);
    renderButton(AUTO_RED_2, -150, 90, button_h, button_w, "Protected Zone \n(5-point)", autonSelected, &redStyle, scr);
    renderButton(AUTO_BLUE_1, 70, -50, button_h, button_w, "Unprotected Zone\n(5-point)", autonSelected, &blueStyle, scr);
    renderButton(AUTO_BLUE_2, 70, 80, button_h, button_w, "Protected Zone\n(5-point)", autonSelected, &blueStyle, scr);
    renderButton(AUTO_DEPLOY, 70, 15, button_h, button_w, "Deploy", autonSelected, &neutralStyle, scr);
    renderButton(AUTO_SIMPLE, -150, 15, button_h, button_w, "One ball", autonSelected, &neutralStyle, scr);

    // lv_obj_t *red1 = lv_btn_create(scrc, NULL);
    // lv_obj_t *label = lv_label_create(red1, NULL);
    // lv_label_set_text(label, "Unprotected Zone \n(5-point)");
    // lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
    // lv_btn_set_style(red1, LV_BTN_STYLE_REL, &redStyle);
    // lv_cont_set_fit(red1, true, true);
    // lv_obj_align(red1, scrc, LV_ALIGN_CENTER, -100, -100);
    // lv_btn_set_action(red1, LV_BTN_ACTION_CLICK, autonSelected);

    // lv_obj_t *red2 = lv_btn_create(scrc, NULL);
    // label = lv_label_create(red2, NULL);
    // lv_label_set_text(label, "Protected Zone \n(5-point)");
    // lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
    // lv_btn_set_style(red2, LV_BTN_STYLE_REL, &redStyle);
    // lv_obj_set_height(red2, lv_obj_get_height(red1));
    // lv_obj_set_width(red2, lv_obj_get_width(red1));
    // lv_obj_align(red2, scrc, LV_ALIGN_CENTER, -100, 90);
    // lv_btn_set_action(red2, LV_BTN_ACTION_CLICK, autonSelected);

    // lv_obj_t *blu1 = lv_btn_create(scrc, NULL);
    // lv_btn_set_style(blu1, LV_BTN_STYLE_REL, &blueStyle);
    // lv_cont_set_fit(blu1, true, true);
    // lv_obj_align(blu1, red1, LV_ALIGN_CENTER, 200, 0);
    // label = lv_label_create(blu1, NULL);
    // lv_label_set_text(label, "Unprotected Zone \n(5-point)");
    // lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
    // lv_btn_set_action(blu1, LV_BTN_ACTION_CLICK, autonSelected);

    // lv_obj_t *blu2 = lv_btn_create(scrc, NULL);
    // lv_btn_set_style(blu2, LV_BTN_STYLE_REL, &blueStyle);
    // lv_obj_set_height(blu2, lv_obj_get_height(blu1));
    // lv_obj_set_width(blu2, lv_obj_get_width(blu1));
    // lv_obj_align(blu2, red2, LV_ALIGN_CENTER, 200, 0);
    // label = lv_label_create(blu2, NULL);
    // lv_label_set_text(label, "Protected Zone \n(5-point)");
    // lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
    // lv_btn_set_action(blu2, LV_BTN_ACTION_CLICK, autonSelected);

    // lv_obj_t *pur1 = lv_btn_create(scrc, NULL);
    // lv_btn_set_style(pur1, LV_BTN_STYLE_REL, &neutralStyle);
    // lv_obj_set_height(pur1, lv_obj_get_height(blu1));
    // lv_obj_set_width(pur1, lv_obj_get_width(blu1));
    // lv_obj_align(pur1, scrc, LV_ALIGN_CENTER, 100, 0);
    // label = lv_label_create(pur1, NULL);
    // lv_label_set_text(label, "Flipout");
    // lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
    // lv_btn_set_action(pur1, LV_BTN_ACTION_CLICK, autonSelected);

    // lv_obj_t *pur2 = lv_btn_create(scrc, NULL);
    // lv_btn_set_style(pur2, LV_BTN_STYLE_REL, &neutralStyle);
    // lv_obj_set_height(pur2, lv_obj_get_height(blu1));
    // lv_obj_set_width(pur2, lv_obj_get_width(blu1));
    // lv_obj_align(pur2, scrc, LV_ALIGN_CENTER, -100, 0);
    // label = lv_label_create(pur2, NULL);
    // lv_label_set_text(label, "One Cube");
    // lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
    // lv_btn_set_action(pur2, LV_BTN_ACTION_CLICK, autonSelected);
}