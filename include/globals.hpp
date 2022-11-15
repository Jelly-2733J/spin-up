#include "api.h"
#include "cata.hpp"
#include "lvglautonselector.hpp"
#include "pros/adi.hpp"

#pragma once

// Classes

// Catapult control and telemetry
extern CataController catapult;

// Auton Selector
extern LVGLAutonSelector selector;

// Ports
extern pros::Motor intake;				// 19
extern pros::Motor cata;		        // 20
extern pros::Vision vis1;				// 16
extern pros::Vision vis2; 				// 17
extern pros::Imu inertial;              // 18

// 3-Wire
extern pros::ADIDigitalOut endgame1;    // A
extern pros::ADIDigitalOut endgame2;    // B
extern pros::ADIDigitalIn cata_switch;  // C

// Controller
extern pros::Controller master;

// Functions
extern void endgame(bool state);