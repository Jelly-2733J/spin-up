#include "api.h"

// Define ports here
pros::Motor drivefl (1, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor drivefr (11, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor drivebl (2, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor drivebr (12, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor ptol (5, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor ptor (15, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor fly1 (9, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor fly2 (10, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Vision vis1 (16);
pros::Vision vis2 (17);
pros::Optical optical (19);
pros::ADIDigitalOut pto ('A');
pros::ADIDigitalOut shooter ('B');

// Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);