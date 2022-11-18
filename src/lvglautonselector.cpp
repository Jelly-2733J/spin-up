#include "pros/apix.h"
#include <mutex>

#include "lvglautonselector.hpp"
#include "globals.hpp"
#include "pros/misc.h"

// LVGL SETUP
lv_obj_t * icon;
lv_obj_t * large_icon;
lv_obj_t * skills;
lv_obj_t * confirm;
lv_obj_t * autonDD;
lv_obj_t * skillsLabel;
lv_obj_t * confirmLabel;
lv_obj_t * autonLabel;
lv_obj_t * win;
lv_style_t styleDD;
lv_style_t skillsREL;
lv_style_t skillsPR;
lv_style_t confirmREL;
lv_style_t confirmPR;
lv_style_t autonStyle;
lv_style_t winStyle;

int autonomousMode = -1;

// Select an auton (0-5)
void LVGLAutonSelector::selectAuton(int auton_number) {
    selected_auton_guard.take();
	selected_auton = auton_number;
    selected_auton_guard.give();
}

// Get the user selected auton number
int LVGLAutonSelector::selectedAuton() {
    selected_auton_guard.take();
    int to_return = selected_auton;
    selected_auton_guard.give();
    return to_return;
}

lv_res_t btn_click_action(lv_obj_t * btn) {
    char selected_auton[32];
    lv_ddlist_get_selected_str(autonDD, selected_auton);

    uint8_t id = lv_obj_get_free_num(btn);

    if (id == 0) {
        if (strcmp(selected_auton, "    Choose Auton     ") == 0){
            autonomousMode = 0;
        }
        else if (strcmp(selected_auton, "         Right") == 0) {
            autonomousMode = 1;
        }
        else if (strcmp(selected_auton, "         Left") == 0) {
            autonomousMode = 2;
        }
        else if (strcmp(selected_auton, "         Solo") == 0) {
            autonomousMode = 3;
        }
        else {
            return LV_RES_OK;
        }
    }
    else if (id == 1) {
        autonomousMode = 4;
    }

    printf("Auton: %s\n", selected_auton);

    lv_obj_set_hidden(autonDD, true);
    lv_obj_set_hidden(skills, true);
    lv_obj_set_hidden(confirm, true);
    lv_obj_set_hidden(skillsLabel, true);
    lv_obj_set_hidden(confirmLabel, true);

    lv_label_set_text(autonLabel, std::to_string(autonomousMode).c_str());
    lv_obj_align(autonLabel, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
    lv_obj_set_hidden(autonLabel, false);

    printf("Auton: %d\n", autonomousMode);

    return LV_RES_OK;
}

lv_res_t ddlist_action(lv_obj_t * ddlist) {
    uint8_t id = lv_obj_get_free_num(ddlist);
    char sel_str[32];
    lv_ddlist_get_selected_str(ddlist, sel_str);
    printf("ID %d: %s\n", id, sel_str);
    return LV_RES_OK;
}

// Create the auton selector
void LVGLAutonSelector::create() {

    lv_style_copy(&skillsREL, &lv_style_plain);
    skillsREL.body.main_color = LV_COLOR_MAKE(50, 50, 50);
    skillsREL.body.grad_color = LV_COLOR_MAKE(50, 50, 50);
    skillsREL.body.radius = 10;
    skillsREL.text.color = LV_COLOR_MAKE(200, 200, 200);

    lv_style_copy(&skillsPR, &lv_style_plain);
    skillsPR.body.main_color = LV_COLOR_MAKE(237, 28, 36);
    skillsPR.body.grad_color = LV_COLOR_MAKE(237, 28, 36);
    skillsPR.body.radius = 10;
    skillsPR.text.color = LV_COLOR_WHITE;

    lv_style_copy(&confirmREL, &lv_style_plain);
    confirmREL.body.main_color = LV_COLOR_MAKE(50, 50, 50);
    confirmREL.body.grad_color = LV_COLOR_MAKE(50, 50, 50);
    confirmREL.body.radius = 10;
    confirmREL.text.color = LV_COLOR_MAKE(200, 200, 200);

    lv_style_copy(&confirmPR, &lv_style_plain);
    confirmPR.body.main_color = LV_COLOR_MAKE(237, 28, 36);;
    confirmPR.body.grad_color = LV_COLOR_MAKE(237, 28, 36);;
    confirmPR.body.radius = 10;
    confirmPR.text.color = LV_COLOR_WHITE;

    lv_style_copy(&winStyle, &lv_style_plain);
    winStyle.body.main_color = LV_COLOR_BLACK;
    winStyle.body.grad_color = LV_COLOR_BLACK;

    lv_style_copy(&styleDD, &lv_style_pretty);
    styleDD.text.color = LV_COLOR_BLACK;

    win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_style(win, LV_WIN_STYLE_BG, &winStyle);
    lv_win_set_style(win, LV_WIN_STYLE_HEADER, &winStyle);
    lv_win_set_title(win, "");

    icon = lv_img_create(lv_scr_act(), NULL);
    LV_IMG_DECLARE(jelly);
    lv_img_set_src(icon, &jelly);
    lv_obj_align(icon, NULL, LV_ALIGN_IN_LEFT_MID, 20, 0);

    // Skills
    skills = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_free_num(skills, 1);
    lv_btn_set_action(skills, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_style(skills, LV_BTN_STYLE_REL, &skillsREL);
    lv_btn_set_style(skills, LV_BTN_STYLE_PR, &skillsPR);
    lv_obj_set_size(skills, 100, 50);
    lv_obj_align(skills, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -115, -10);

    // Confirmation Button
    confirm = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_free_num(confirm, 0);
    lv_btn_set_action(confirm, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_btn_set_style(confirm, LV_BTN_STYLE_REL, &confirmREL);
    lv_btn_set_style(confirm, LV_BTN_STYLE_PR, &confirmPR);
    lv_obj_set_size(confirm, 100, 50);
    lv_obj_align(confirm, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -10, -10);

    // Auton Dropdown
    autonDD = lv_ddlist_create(lv_scr_act(), NULL);
    lv_ddlist_set_options(autonDD, "    Choose Auton     \n"
                                "         Left\n"
                                "         Right\n"
                                "         Solo");
    lv_obj_align(autonDD, NULL, LV_ALIGN_IN_RIGHT_MID, -10, -38);
    lv_obj_set_free_num(autonDD, 3);
    lv_ddlist_set_action(autonDD, ddlist_action);
    lv_ddlist_set_style(autonDD, LV_DDLIST_STYLE_SEL, &styleDD);

    skillsLabel = lv_label_create(skills, NULL);
    lv_label_set_text(skillsLabel, "Run Skills");

    confirmLabel = lv_label_create(confirm, NULL);
    lv_label_set_text(confirmLabel, "Confirm");

    autonLabel = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_hidden(autonLabel, true);

    while (autonomousMode == -1) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            autonomousMode = 0;
            lv_obj_set_hidden(autonDD, true);
            lv_obj_set_hidden(skills, true);
            lv_obj_set_hidden(confirm, true);
            lv_obj_set_hidden(skillsLabel, true);
            lv_obj_set_hidden(confirmLabel, true);

            lv_label_set_text(autonLabel, std::to_string(autonomousMode).c_str());
            lv_obj_align(autonLabel, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
            lv_obj_set_hidden(autonLabel, false);

            printf("Auton: %d\n", autonomousMode);
            break;
        }
        pros::delay(10);
    }

    end();

}

// End the auton selector and display logo
void LVGLAutonSelector::end() {
    lv_obj_set_hidden(icon, true);
    lv_obj_set_hidden(skills, true);
    lv_obj_set_hidden(confirm, true);
    lv_obj_set_hidden(autonDD, true);
    lv_obj_set_hidden(skillsLabel, true);
    lv_obj_set_hidden(confirmLabel, true);

    large_icon = lv_img_create(lv_scr_act(), NULL);
    LV_IMG_DECLARE(jellylarge);
    lv_img_set_src(large_icon, &jellylarge);
    lv_obj_align(large_icon, NULL, LV_ALIGN_CENTER, 0, 0);

    selectAuton(autonomousMode);
}