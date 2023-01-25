#include "api.h"
#include "globals.hpp"
#include "pros/motors.h"

// Classes

// Cata control and telemetry
CataController cata;

// Ports
pros::Motor cata1 (16, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES); // 16
pros::Motor cata2 (15, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);	// 15

// 3-Wire
pros::ADIDigitalOut endgame ({ 3, 'A' }, false);        // A
pros::ADIDigitalOut band_assist ({ 3, 'B' }, false);    // B
pros::ADIDigitalIn cata_switch ({ 3, 'C' });            // C

// Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);