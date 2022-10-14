#include "api.h"
#include "globals.hpp"

// Classes

// AutoAim
AutoAim aim;

// Flywheel control and telemetry
FlywheelController flywheel;

// Auton Selector
LVGLAutonSelector selector;

// Ports
pros::Motor intake (19, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor fly (20, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Vision vis1 (16);
pros::Vision vis2 (17);
pros::Imu inertial (18);

// 3-Wire
pros::ADIDigitalOut endgame1 ('A', false);
pros::ADIDigitalOut endgame2 ('B', false);

// Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Functions
void endgame(bool state) {
    endgame1.set_value(state);
    endgame2.set_value(state);
}