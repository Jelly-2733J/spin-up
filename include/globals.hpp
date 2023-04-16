#include "api.h"
#include "flywheel.hpp"

#pragma once

#define PI 3.14159265358979323846

// Classes

// Flywheel control and telemetry
extern FlywheelController flywheel;

// Ports
extern pros::Motor intake;				    // 16
extern pros::Motor fly;				        // 15

// 3-Wire
extern pros::ADIDigitalOut actuated_intake; // A
extern pros::ADIDigitalOut blooper;         // B
extern pros::ADIDigitalOut side_endgame;   // G
extern pros::ADIDigitalOut front_endgame;    // H

// Controller
extern pros::Controller master;

// Functions
void endgame(bool state = true);