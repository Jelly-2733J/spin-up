#include "api.h"
#include "cata.hpp"
#include "pros/adi.hpp"

#pragma once

#define PI 3.14159265358979323846

// Classes

// Cata control and telemetry
extern CataController cata;

// Ports
extern pros::Motor cata1;				// 16
extern pros::Motor cata2;				// 15

// 3-Wire
extern pros::ADIDigitalOut endgame;     // A
extern pros::ADIDigitalOut band_assist; // B
extern pros::ADIDigitalIn cata_switch;  // C

// Controller
extern pros::Controller master;