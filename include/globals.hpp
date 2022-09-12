#include "api.h"
#include "autoaim.hpp"
#include "flywheel.hpp"
#include "lvglautonselector.hpp"

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

// Controller
extern pros::Controller master;