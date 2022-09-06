#include "api.h"
#include "globals.hpp"

// Classes

// Instantiate the AutoAim class
AutoAim aim;

// Instantiate the FlywheelController class
FlywheelController flywheel;

// Instantiate the AutoRoller class
AutoRoller roller;

// Instantiate the AutonSelector class
LVGLAutonSelector selector;

// Ports
pros::Motor intake (19, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor fly (20, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Vision vis1 (16);
pros::Vision vis2 (17);
pros::Imu inertial (18);
pros::Optical optical (19);

// Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);