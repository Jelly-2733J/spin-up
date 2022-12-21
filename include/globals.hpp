#include "api.h"
#include "flywheel.hpp"
#include "robotmotion.hpp"
#include "lvglautonselector.hpp"
#include "pros/adi.hpp"

#pragma once

#define PI 3.14159265358979323846

// Classes

// Flywheel control and telemetry
extern FlywheelController flywheel;

// Odometry
extern RobotMotion motion;

// Auton Selector
extern LVGLAutonSelector selector;

// Ports
extern pros::Motor intake;				// 16
extern pros::Motor fly;				    // 15

// 3-Wire
extern pros::ADIDigitalOut endgame;     // A
extern pros::ADIDigitalOut blooper;     // B
extern pros::ADIDigitalOut indexer;     // C
extern pros::ADIDigitalOut pressure_bar;// D

// Controller
extern pros::Controller master;

// Functions
extern void fire();