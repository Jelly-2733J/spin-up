#pragma once

#include "EZ-Template/drive/drive.hpp"

extern Drive chassis;

void no_auton();
void right_max();
void right_ni();
void right_r();
void left_max();
void left_ni();
void left_r();
void solo_wp();
void nerfed_skills();
void auton_skills();

void default_constants();
void one_mogo_constants();
void two_mogo_constants();
void exit_condition_defaults();
void modified_exit_condition();