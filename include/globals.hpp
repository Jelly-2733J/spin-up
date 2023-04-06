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
extern pros::Optical optical;               // 14

// 3-Wire
extern pros::ADIDigitalOut actuated_intake; // A
extern pros::ADIDigitalOut blooper;         // B
extern pros::ADIDigitalOut endgame;         // C

// Controller
extern pros::Controller master;