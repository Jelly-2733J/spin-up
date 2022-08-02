#include "main.h"

#pragma once

#include "autoaim.hpp"

// FlywheelController class allows for automated flywheel control and RPM calculation.
class FlywheelController {
	private:
        int flywheel_RPM;
        pros::Mutex flywheel_RPM_guard;
		int flywheel_target_RPM;
		pros::Mutex flywheel_target_RPM_guard;
        // Convert degrees to radians
        static double rads(double deg);
		// Set the flywheel to a certain RPM
		void set_RPM(int rpm);
		// Calculate optimal RPM
		int calculate_RPM(double distance);
	public:
		// Set target RPM
		void set_target_RPM(int rpm);
		// Read the current RPM
		int RPM();
		// Read the current target RPM
		int target_RPM();
		// Flywheel task
		void flyControl();
};