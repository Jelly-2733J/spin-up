#include "api.h"
#include "globals.hpp"
#include "pros/adi.hpp"
#include <strings.h>

// Classes

// Flywheel control and telemetry
FlywheelController flywheel;

// Ports
pros::Motor intake (16, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor fly (11, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);

// 3-Wire
pros::ADIDigitalOut actuated_intake ('A', false);
pros::ADIDigitalOut blooper ('B', false);
pros::ADIDigitalOut side_endgame ('G', false);
pros::ADIDigitalOut front_endgame ('H', false);

// Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Functions
void endgame(bool state) {
    side_endgame.set_value(state);
    front_endgame.set_value(state);
    printf("Endgame state: %d", state);
}