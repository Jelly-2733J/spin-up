#include "api.h"
#include "globals.hpp"
#include "pros/optical.hpp"
#include <strings.h>

// Classes

// Flywheel control and telemetry
FlywheelController flywheel;

// Ports
pros::Motor intake (16, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor fly (15, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);

// 3-Wire
pros::ADIDigitalOut endgame ({ 3, 'A' }, false);
pros::ADIDigitalOut blooper ({ 3, 'B' }, false);

// Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);