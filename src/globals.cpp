#include "EZ-Template/sdcard.hpp"
#include "api.h"
#include "globals.hpp"
#include "pros/adi.hpp"
#include "pros/misc.hpp"
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
bool endgame(bool state, int driver_start) {

    bool output = false;

    // If false, set endgame to false because that doesn't do any harm
    if (!state) {
        side_endgame.set_value(false);
        front_endgame.set_value(false);
    }
    // Else if in auto, I'm not goofy enough to fire endgame when I don't want to, trust the given state
    else if (pros::competition::is_autonomous()) {
        side_endgame.set_value(state);
        front_endgame.set_value(state);
        output = state;
    }
    // Else we are in driver control
    else {
        std::string auton_name = ez::as::auton_selector.Autons[ez::as::auton_selector.selected_auton].Name;
        // If we are in skills or no auton, this is driver skills or practice, so we allow endgame after 50 seconds or if the Y override is pressed
        if ((auton_name == "Skills" || auton_name == "No Auton              ") && (pros::millis() - driver_start > 50000 || master.get_digital(pros::E_CONTROLLER_DIGITAL_Y))) {
            side_endgame.set_value(state);
            front_endgame.set_value(state);
            output = state;
        }
        // Else we are in a match, so we allow endgame after 95 seconds or if the Y override is pressed
        else if (pros::millis() - driver_start > 95000 || master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
            side_endgame.set_value(state);
            front_endgame.set_value(state);
            output = state;
        }
    }

    printf("Endgame %s", state ? "fired" : "off");
    return output;
}