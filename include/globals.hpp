#include "api.h"
#include "autoaim.hpp"
#include "flywheel.hpp"
#include "roller.hpp"
#include "lvglautonselector.hpp"

#pragma once

// Classes

// Instantiate the AutoAim class
extern AutoAim aim;

// Instantiate the FlywheelController class
extern FlywheelController flywheel;

// Instantiate the AutoRoller class
extern AutoRoller roller;

// Instantiate the AutonSelector class
extern LVGLAutonSelector selector;

// Ports
extern pros::Motor intake;				// 9
extern pros::Motor fly;				    // 10
extern pros::Vision vis1;				// 16
extern pros::Vision vis2; 				// 17
extern pros::Imu inertial;              // 18
extern pros::Optical optical;			// 19
extern pros::ADIDigitalOut shooter;		// A

// Controller
extern pros::Controller master;