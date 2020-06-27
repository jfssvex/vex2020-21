#ifndef AUTON_SELECTOR
#define AUTON_SELECTOR

#include "main.h"

enum {
    AUTON_RED_PROTECTED,
    AUTON_RED_UNPROTECTED,
    AUTON_BLUE_PROTECTED,
    AUTON_BLUE_UNPROTECTED,
    SIMPLE_ONE_CUBE,
    FLIPOUT
 };
 typedef int auton_options;

static void hal_init(void);
static int tick_thread(void * data);
static void memory_monitor(lv_task_t * param);
static lv_res_t changeBlue(lv_obj_t * btn);
static lv_res_t changeRed(lv_obj_t * btn);
static lv_res_t changePurple(lv_obj_t * btn);
void deleteButtons();
void autonSelector(void* param);
static int confirmChoice(char autonString[]);
static lv_res_t click_action(lv_obj_t * m, const char* btn_txt);
void endRun(int choice);
void initMainTheme();
void deleteScr();
auton_options getAutonId();
int getComplete();
void setAutonId(auton_options x);
auton_options getAutonId();

static lv_theme_t * th;
static lv_theme_t * blueBack;
static lv_obj_t * scr;
static lv_theme_t * actTheme;
static lv_obj_t * mbox;
static lv_style_t mboxStyle;
static lv_style_t greyPreChosen;

#endif
