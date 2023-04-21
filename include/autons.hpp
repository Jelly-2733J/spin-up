#pragma once

#include "EZ-Template/drive/drive.hpp"

extern Drive chassis;

void no_auton();
void right_max();
void right_safe();
void right_rush();
void left_max();
void left_safe();
void left_rush();
void solo_wp();
void auton_skills();

void default_constants();
void one_mogo_constants();
void two_mogo_constants();
void exit_condition_defaults();
void modified_exit_condition();