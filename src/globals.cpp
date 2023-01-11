#include "api.h"
#include "globals.hpp"
#include "pros/optical.hpp"
#include <strings.h>

// Classes

// Flywheel control and telemetry
FlywheelController flywheel;

// Odometry
Odometry odom(1.0, 1.0, 1.0, 2.75, 'A', 'B', 'C', 'D', 'E', 'F');

// Ports
pros::Motor intake (16, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor fly (15, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Optical optical (10);

// 3-Wire
pros::ADIDigitalOut endgame ({ 17, 'A' }, false);
pros::ADIDigitalOut blooper ({ 17, 'B' }, false);
pros::ADIDigitalOut indexer ({ 17, 'C' }, false);
pros::ADIDigitalOut pressure_bar ({ 17, 'D' }, false);

// Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Functions
void fire() {
    indexer.set_value(true);
    intake = -100;
    pros::delay(200);
    indexer.set_value(false);
    intake = 0;
}