#include "api.h"
#include "autoaim.hpp"
#include "flywheel.hpp"
#include "lvglautonselector.hpp"
#include "pros/adi.hpp"

#pragma once

// Classes

// AutoAim
extern AutoAim aim;

// Flywheel control and telemetry
extern FlywheelController flywheel;

// Auton Selector
extern LVGLAutonSelector selector;

// Ports
extern pros::Motor intake;				// 19
extern pros::Motor fly;				    // 20
extern pros::Vision vis1;				// 16
extern pros::Vision vis2; 				// 17
extern pros::Imu inertial;              // 18

// 3-Wire
extern pros::ADIDigitalOut endgame1;    // A
extern pros::ADIDigitalOut endgame2;    // B

// Controller
extern pros::Controller master;

// Functions
extern void endgame(bool state);