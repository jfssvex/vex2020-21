#include "autonSelector.h"

lv_obj_t *red1;
lv_obj_t *red2;
lv_obj_t *blu1;
lv_obj_t *blu2;
lv_obj_t *pur1;
lv_obj_t *pur2;
lv_color_t red = LV_COLOR_MAKE(234, 35, 58);
lv_color_t blue = LV_COLOR_MAKE(41, 130, 198);
lv_color_t purple = LV_COLOR_MAKE(35, 44, 101);
static const char *btns[] = {"\221Confirm", "Decline", ""};
bool confirm = false;
void initiate();
int count{};
int confirmCounter{};
int complete = 0;

auton_options auton_id = 8;

void autonSelector(void *param)
{
	static lv_style_t redPreChosen;
	lv_style_copy(&redPreChosen, &lv_style_plain_color);
	redPreChosen.body.main_color = red;
	redPreChosen.body.grad_color = red;
	redPreChosen.body.radius = 4;
	redPreChosen.body.border.width = 0;
	redPreChosen.body.border.color = red;
	redPreChosen.body.padding.ver = 5;
	redPreChosen.body.padding.hor = 5;
	redPreChosen.body.padding.inner = 0;
	redPreChosen.body.shadow.width = 0;
	redPreChosen.body.shadow.color = red;
	redPreChosen.text.color = LV_COLOR_WHITE;
	//	redPreChosen.text.font = &GOTHAM_20;

	lv_style_copy(&mboxStyle, &redPreChosen);
	mboxStyle.body.main_color = LV_COLOR_WHITE;
	mboxStyle.body.grad_color = LV_COLOR_WHITE;
	mboxStyle.text.color = LV_COLOR_BLACK;

	static lv_style_t bluPreChosen;
	lv_style_copy(&bluPreChosen, &redPreChosen);
	bluPreChosen.body.main_color = blue;
	bluPreChosen.body.grad_color = blue;

	lv_style_copy(&greyPreChosen, &bluPreChosen);
	greyPreChosen.body.main_color = purple;
	greyPreChosen.body.grad_color = purple;

	static lv_style_t purPreChosen;
	lv_style_copy(&purPreChosen, &bluPreChosen);
	purPreChosen.body.main_color = purple;
	purPreChosen.body.grad_color = purple;

	th = lv_theme_alien_init(50, NULL);
	th->win.bg->body.main_color = LV_COLOR_WHITE;
	th->win.bg->body.grad_color = LV_COLOR_WHITE;
	th->win.bg->body.border.color = LV_COLOR_MAKE(35, 44, 101);
	th->win.bg->body.border.width = 10;
	actTheme = th;

	lv_theme_set_current(actTheme);
	scr = lv_page_create(NULL, NULL);
	lv_scr_load(scr);
	lv_page_set_sb_mode(scr, LV_SB_MODE_OFF);

	initMainTheme();
	blueBack->win.bg->body.border.color = purple;
	blueBack->win.bg->body.main_color = LV_COLOR_WHITE;
	blueBack->win.bg->body.grad_color = LV_COLOR_WHITE;

	red1 = lv_btn_create(scr, NULL);
	lv_obj_t *label = lv_label_create(red1, NULL);
	lv_label_set_text(label, "Unprotected Zone \n(5-point)");
	lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
	lv_btn_set_style(red1, LV_BTN_STYLE_REL, &redPreChosen);
	lv_cont_set_fit(red1, true, true);
	lv_obj_align(red1, scr, LV_ALIGN_CENTER, -100, -100);
	lv_btn_set_action(red1, LV_BTN_ACTION_CLICK, changeRed);

	red2 = lv_btn_create(scr, NULL);
	label = lv_label_create(red2, NULL);
	lv_label_set_text(label, "Protected Zone \n(5-point)");
	lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
	lv_btn_set_style(red2, LV_BTN_STYLE_REL, &redPreChosen);
	lv_obj_set_height(red2, lv_obj_get_height(red1));
	lv_obj_set_width(red2, lv_obj_get_width(red1));
	lv_obj_align(red2, scr, LV_ALIGN_CENTER, -100, 90);
	lv_btn_set_action(red2, LV_BTN_ACTION_CLICK, changeRed);

	blu1 = lv_btn_create(scr, NULL);
	lv_btn_set_style(blu1, LV_BTN_STYLE_REL, &bluPreChosen);
	lv_cont_set_fit(blu1, true, true);
	lv_obj_align(blu1, red1, LV_ALIGN_CENTER, 200, 0);
	label = lv_label_create(blu1, NULL);
	lv_label_set_text(label, "Unprotected Zone \n(5-point)");
	lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
	lv_btn_set_action(blu1, LV_BTN_ACTION_CLICK, changeBlue);

	blu2 = lv_btn_create(scr, NULL);
	lv_btn_set_style(blu2, LV_BTN_STYLE_REL, &bluPreChosen);
	lv_obj_set_height(blu2, lv_obj_get_height(blu1));
	lv_obj_set_width(blu2, lv_obj_get_width(blu1));
	lv_obj_align(blu2, red2, LV_ALIGN_CENTER, 200, 0);
	label = lv_label_create(blu2, NULL);
	lv_label_set_text(label, "Protected Zone \n(5-point)");
	lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
	lv_btn_set_action(blu2, LV_BTN_ACTION_CLICK, changeBlue);

	pur1 = lv_btn_create(scr, NULL);
	lv_btn_set_style(pur1, LV_BTN_STYLE_REL, &purPreChosen);
	lv_obj_set_height(pur1, lv_obj_get_height(blu1));
	lv_obj_set_width(pur1, lv_obj_get_width(blu1));
	lv_obj_align(pur1, scr, LV_ALIGN_CENTER, 100, 0);
	label = lv_label_create(pur1, NULL);
	lv_label_set_text(label, "Flipout");
	lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
	lv_btn_set_action(pur1, LV_BTN_ACTION_CLICK, changePurple);

	pur2 = lv_btn_create(scr, NULL);
	lv_btn_set_style(pur2, LV_BTN_STYLE_REL, &purPreChosen);
	lv_obj_set_height(pur2, lv_obj_get_height(blu1));
	lv_obj_set_width(pur2, lv_obj_get_width(blu1));
	lv_obj_align(pur2, scr, LV_ALIGN_CENTER, -100, 0);
	label = lv_label_create(pur2, NULL);
	lv_label_set_text(label, "One Cube");
	lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
	lv_btn_set_action(pur2, LV_BTN_ACTION_CLICK, changePurple);
}

void initMainTheme()
{
	blueBack = lv_theme_alien_init(10, NULL);
	blueBack->win.bg->body.border.color = LV_COLOR_WHITE;
	blueBack->win.bg->body.border.width = 10;
}

static lv_res_t changeBlue(lv_obj_t *btn)
{
	++count;
	if (count == 1)
	{
		char s[30];
		initMainTheme();
		if (btn == blu1)
		{
			strcpy(s, "Blue Auton Unprotected Zone");
			auton_id = AUTON_BLUE_UNPROTECTED;
		}
		else if (btn == blu2)
		{
			strcpy(s, "Blue Auton Protected Zone");
			auton_id = AUTON_BLUE_PROTECTED;
		}
		deleteButtons();
		blueBack->win.bg->body.main_color = blue;
		blueBack->win.bg->body.grad_color = blue;
		pros::delay(500);
		confirmChoice(s);
	}
}

static lv_res_t changeRed(lv_obj_t *btn)
{
	++count;
	if (count == 1)
	{
		char s[30];
		initMainTheme();
		if (btn == red1)
		{
			strcpy(s, "Red Auton Unprotected Zone");
			auton_id = AUTON_RED_UNPROTECTED;
		}
		else if (btn == red2 || btn == pur1)
		{
			strcpy(s, "Red Auton Protected Zone");
			auton_id = AUTON_RED_PROTECTED;
		}
		deleteButtons();
		blueBack->win.bg->body.main_color = red;
		blueBack->win.bg->body.grad_color = red;
		pros::delay(500);
		confirmChoice(s);
	}
}

static lv_res_t changePurple(lv_obj_t *btn)
{
	++count;
	if (count == 1)
	{
		char s[30];
		initMainTheme();
		if (btn == pur1)
		{
			strcpy(s, "Flipout");
			setAutonId(FLIPOUT);
		}
		else if (btn == pur2)
		{
			strcpy(s, "One Cube Auton");
			setAutonId(SIMPLE_ONE_CUBE);
		}
		deleteButtons();
		blueBack->win.bg->body.main_color = purple;
		blueBack->win.bg->body.grad_color = purple;
		pros::delay(500);
		int i = confirmChoice(s);
	}
}

int getComplete()
{
	return complete;
}

int confirmChoice(char autonString[])
{
	char s[100];
	strcpy(s, "S'il vous plait, confirmez-vous que vous voulez choisir <<");
	strcat(s, autonString);
	strcat(s, ">> pour l'auton?");
	mbox = lv_mbox_create(scr, NULL);
	lv_mbox_set_text(mbox, s);
	lv_mbox_add_btns(mbox, btns, NULL);
	lv_mbox_set_action(mbox, click_action);
	lv_obj_set_width(mbox, 250);
	lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &mboxStyle);
	lv_mbox_set_style(mbox, LV_MBOX_STYLE_BTN_REL, &greyPreChosen);
	return 0;
}

static lv_res_t click_action(lv_obj_t *m, const char *btn_txt)
{
	++confirmCounter;
	if (confirmCounter == 1)
	{
		if (strcmp(btn_txt, btns[1]))
		{
			endRun(0);
			return LV_RES_INV;
		}
		else if (strcmp(btn_txt, btns[0]))
		{
			endRun(1);
			return LV_RES_INV;
		}
	}
}

void endRun(int choice)
{
	printf("This is auton #: %d", auton_id);
	if (choice == 0)
	{
		//	lv_obj_del(mbox);
		complete = 1;
		deleteScr();
		initiate();
	}
	else if (choice == 1)
	{
		auton_id = -1;
		count = 0;
		confirmCounter = 0;
		//lv_obj_del(mbox);
		//	deleteScr();
		autonSelector((void *)"PROS");
	}
}

void initiate()
{
	//lv_scr_load(scr2);
	blueBack->win.bg->body.main_color = purple;
	blueBack->win.bg->body.grad_color = purple;
	lv_style_t sicko;
	lv_style_copy(&sicko, &greyPreChosen);
	sicko.text.color = LV_COLOR_WHITE;

	lv_obj_t *toast;
	lv_obj_t *royals;

	toast = lv_label_create(scr, NULL);
	lv_label_set_style(toast, &sicko);
	lv_label_set_text(toast, "TOAST 2");
	lv_obj_set_pos(toast, 20, 20);
}

auton_options getAutonId()
{
	return auton_id;
}

void deleteScr()
{
	lv_obj_clean(scr);
	//    lv_obj_set_hidden(scr, false);
	//    lv_obj_t * scr2 = lv_page_create(NULL, NULL);
	//    lv_scr_load(scr2);
	//    deleteButtons();
}

void deleteButtons()
{
	lv_obj_del(pur2);
	lv_obj_del(pur1);
	lv_obj_del(blu1);
	lv_obj_del(blu2);
	lv_obj_del(red1);
	lv_obj_del(red2);
}

void setAutonId(auton_options x)
{
	auton_id = x;
}
