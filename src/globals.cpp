#include "api.h"
#include "globals.hpp"
#include <strings.h>

// Classes

// AutoAim
AutoAim aim;

// Flywheel control and telemetry
FlywheelController flywheel;

// Auton Selector
LVGLAutonSelector selector;

// Ports
pros::Motor intake (19, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor fly (20, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Vision vis1 (16);
pros::Vision vis2 (17);
pros::Optical optical (7);
pros::Imu inertial (18);

// 3-Wire
pros::ADIDigitalOut endgame1 ('A', false);
pros::ADIDigitalOut endgame2 ('B', false);
pros::ADIDigitalOut blooper ('C', false);
pros::ADIDigitalOut indexer ('D', false);
pros::ADIDigitalOut pressure_bar ('E', false);

// Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Functions
void endgame(bool state) {
    endgame1.set_value(state);
    endgame2.set_value(state);
}

void fire() {
    indexer.set_value(true);
    pros::delay(200);
    indexer.set_value(false);
}