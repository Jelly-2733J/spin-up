#include "api.h"

#pragma once

extern pros::Motor drivefl;				// 1
extern pros::Motor drivefr;				// 2
extern pros::Motor drivebl;				// 3
extern pros::Motor drivebr;				// 4
extern pros::Motor ptol;				// 5
extern pros::Motor ptor;				// 6
extern pros::Motor fly1;				// 7
extern pros::Motor fly2;				// 8
extern pros::Vision vis1;				// 9
extern pros::Vision vis2; 				// 10
extern pros::Optical optical;			// 11
extern pros::ADIDigitalOut pto;			// A
extern pros::ADIDigitalOut shooter;		// B

// Controller
extern pros::Controller master;