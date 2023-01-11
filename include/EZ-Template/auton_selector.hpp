/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once
#include <tuple>

#include "EZ-Template/auton.hpp"

using namespace std;
class AutonSelector {
 public:
  std::vector<Auton> Autons;
  int selected_auton;
  int auton_count;
  std::string menu_gif_path;
  std::string gif_path;
  AutonSelector();
  AutonSelector(std::vector<Auton> autons);
  
  char* generateAutonList();
  void call_selected_auton();
  void call_auton(int auton_number);
  void print_selected_auton();
  void add_autons(std::vector<Auton> autons);
  void create();
  void end();
};
