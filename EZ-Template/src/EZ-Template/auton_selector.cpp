/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "main.h"
#include "gif-pros/gifclass.hpp"

// LVGL SETUP
lv_obj_t * skills;
lv_obj_t * confirm;
lv_obj_t * autonDD;
lv_obj_t * skillsLabel;
lv_obj_t * confirmLabel;
lv_obj_t * autonLabel;
lv_obj_t * win;
lv_obj_t * menu_gif_obj;
lv_obj_t * gif_obj;
lv_style_t styleDD;
lv_style_t skillsREL;
lv_style_t skillsPR;
lv_style_t confirmREL;
lv_style_t confirmPR;
lv_style_t autonStyle;
lv_style_t winStyle;

int auton = -1;

AutonSelector::AutonSelector() {
  auton_count = 0;
  selected_auton = 0;
  Autons = {};
}

AutonSelector::AutonSelector(std::vector<Auton> autons) {
  auton_count = autons.size();
  selected_auton = 0;
  Autons = {};
  Autons.assign(autons.begin(), autons.end());
}

char* AutonSelector::generateAutonList() {
  if (auton_count == 0) {
    return (char*) "No Autons";
  }
  std::string auton_list;

  // Copy the auton names into the list
  for (Auton _auton : Autons) {
    auton_list.append(_auton.Name + '\n');
  }

  // Remove the last newline
  auton_list.pop_back();

  return const_cast<char*>(auton_list.c_str());
}

lv_res_t btn_click_action(lv_obj_t * btn) {

  uint8_t id = lv_obj_get_free_num(btn);

  // Confirm
  if (id == 0) {
    auton = lv_ddlist_get_selected(autonDD);
  }
  // Skills
  else if (id == 1) {
    // Get the auton list string
    std::string auton_list = lv_ddlist_get_options(autonDD);

    // Count the number of newlines to determine last index
    int auton_index = 0;
    for (char c : auton_list) {
      if (c == '\n') auton_index++;
    }

    auton = auton_index;
  }

  printf("Auton: %d\n", auton);

  lv_obj_set_hidden(autonDD, true);
  lv_obj_set_hidden(skills, true);
  lv_obj_set_hidden(confirm, true);
  lv_obj_set_hidden(skillsLabel, true);
  lv_obj_set_hidden(confirmLabel, true);

  return LV_RES_OK;
}

lv_res_t ddlist_action(lv_obj_t * ddlist) {
  uint8_t id = lv_obj_get_free_num(ddlist);
  char sel_str[32];
  lv_ddlist_get_selected_str(ddlist, sel_str);
  return LV_RES_OK;
}

void AutonSelector::print_selected_auton() {
  if (auton_count == 0 || auton == -1) return ez::print_to_screen("None");
  for (int i = 0; i < 8; i++)
    pros::lcd::clear_line(i);
  ez::print_to_screen(Autons[auton].Name);
}

void AutonSelector::call_selected_auton() {
  if (auton_count == 0) return;
  Autons[selected_auton].auton_call();
}

void AutonSelector::call_auton(int auton_number) {
  if (auton_count == 0) return;
  Autons[auton_number].auton_call();
}

void AutonSelector::add_autons(std::vector<Auton> autons) {
  auton_count += autons.size();
  selected_auton = 0;
  Autons.assign(autons.begin(), autons.end());
}

// Create the auton selector
void AutonSelector::create() {

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

  menu_gif_obj = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(menu_gif_obj, 480, 240);
  lv_obj_set_style(menu_gif_obj, &lv_style_transp); // make the container invisible
  lv_obj_align(menu_gif_obj, NULL, LV_ALIGN_CENTER, 0, 0);

  static Gif menu_gif(const_cast<char*>(menu_gif_path.c_str()), menu_gif_obj);

  // Skills Button
  skills = lv_btn_create(lv_scr_act(), NULL);
  lv_obj_set_free_num(skills, 1);
  lv_btn_set_action(skills, LV_BTN_ACTION_CLICK, btn_click_action);
  lv_btn_set_style(skills, LV_BTN_STYLE_REL, &skillsREL);
  lv_btn_set_style(skills, LV_BTN_STYLE_PR, &skillsPR);
  lv_obj_set_size(skills, 107, 50);
  lv_obj_align(skills, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -125, -10);

  // Confirmation Button
  confirm = lv_btn_create(lv_scr_act(), NULL);
  lv_obj_set_free_num(confirm, 0);
  lv_btn_set_action(confirm, LV_BTN_ACTION_CLICK, btn_click_action);
  lv_btn_set_style(confirm, LV_BTN_STYLE_REL, &confirmREL);
  lv_btn_set_style(confirm, LV_BTN_STYLE_PR, &confirmPR);
  lv_obj_set_size(confirm, 107, 50);
  lv_obj_align(confirm, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -10, -10);

  // Auton Dropdown
  autonDD = lv_ddlist_create(lv_scr_act(), NULL);
  lv_ddlist_set_options(autonDD, generateAutonList());
  lv_ddlist_set_sb_mode(autonDD, LV_SB_MODE_ON);
  lv_ddlist_set_fix_height(autonDD, 200);
  lv_obj_align(autonDD, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 10);
  lv_obj_set_free_num(autonDD, 3);
  lv_ddlist_set_action(autonDD, ddlist_action);
  lv_ddlist_set_style(autonDD, LV_DDLIST_STYLE_SEL, &styleDD);

  skillsLabel = lv_label_create(skills, NULL);
  lv_label_set_text(skillsLabel, "Run skills");

  confirmLabel = lv_label_create(confirm, NULL);
  lv_label_set_text(confirmLabel, "Confirm");

  while (auton == -1) {
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      auton = 0;
      printf("Auton: %d\n", auton);
      break;
    }
    pros::delay(10);
  }

  menu_gif.clean();
  end();

}

// End the auton selector and display logo
void AutonSelector::end() {
  lv_obj_set_hidden(menu_gif_obj, true);
  lv_obj_set_hidden(skills, true);
  lv_obj_set_hidden(confirm, true);
  lv_obj_set_hidden(autonDD, true);
  lv_obj_set_hidden(skillsLabel, true);
  lv_obj_set_hidden(confirmLabel, true);

  pros::delay(500);

  gif_obj = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(gif_obj, 480, 240);
  lv_obj_set_style(gif_obj, &lv_style_transp); // make the container invisible
  lv_obj_align(gif_obj, NULL, LV_ALIGN_CENTER, 0, 0);

  static Gif gif(const_cast<char*>(gif_path.c_str()), gif_obj);

  autonLabel = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_hidden(autonLabel, true);

  lv_label_set_text(autonLabel, std::to_string(auton).c_str());
  lv_obj_align(autonLabel, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
  lv_obj_set_hidden(autonLabel, false);

  selected_auton = auton;
}