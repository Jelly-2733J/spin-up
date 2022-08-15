#include "api.h"

#pragma once

extern pros::Motor drivefl;				// 1
extern pros::Motor drivefr;				// 11
extern pros::Motor driveml;				// 2
extern pros::Motor drivemr;				// 12
extern pros::Motor drivebl;				// 3
extern pros::Motor drivebr;				// 13
extern pros::Motor intake;				// 9
extern pros::Motor fly;				    // 10
extern pros::Vision vis1;				// 16
extern pros::Vision vis2; 				// 17
extern pros::Imu inertial;              // 18
extern pros::Optical optical;			// 19
extern pros::ADIDigitalOut shooter;		// A

// Controller
extern pros::Controller master;