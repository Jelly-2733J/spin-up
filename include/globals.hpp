#include "api.h"
#include "flywheel.hpp"

#pragma once

#define PI 3.14159265358979323846

// Classes

// Flywheel control and telemetry
extern FlywheelController flywheel;

// Ports
extern pros::Motor intake;				// 16
extern pros::Motor fly;				    // 15

// 3-Wire
extern pros::ADIDigitalOut endgame;     // A
extern pros::ADIDigitalOut blooper;     // B

// Controller
extern pros::Controller master;