#include "api.h"

// Define ports here
pros::Vision vis1 (7);
pros::Vision vis2 (8);
pros::Motor fly1 (9, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor fly2 (10, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::ADIDigitalOut pto ('A');
pros::ADIDigitalOut shooter ('B');