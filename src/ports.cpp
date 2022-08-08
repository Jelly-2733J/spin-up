#include "api.h"

// Define ports here
pros::Motor drivefl (1, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor drivefr (2, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor drivebl (3, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor drivebr (4, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor ptol (5, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor ptor (6, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor fly1 (7, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor fly2 (8, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Vision vis1 (9);
pros::Vision vis2 (10);
pros::Optical rollopt (11);
pros::ADIDigitalOut pto ('A');
pros::ADIDigitalOut shooter ('B');