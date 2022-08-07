#include "api.h"

#pragma once

// FlywheelController class allows for automated flywheel control and RPM calculation.
class FlywheelController {
	private:
		int flywheel_RPM;
		pros::Mutex flywheel_RPM_guard;
		int flywheel_target_RPM;
		pros::Mutex flywheel_target_RPM_guard;
		bool active;
		pros::Mutex active_guard;
		// Convert degrees to radians
		static double rads(double deg);
		// Set the flywheel to a certain RPM
		void set_RPM(int rpm);
		// Calculate optimal RPM
		int calculate_RPM(double distance);
		// Set voltage of flywheel motors
		void set_voltage(double voltage);
		// Check the sign of a number
		bool check_sign(double num);
		// Clip a number to a certain range
		double clip(double num, double min, double max);
	public:
		// Set target RPM
		void set_target_RPM(int rpm);
		// Read the current RPM
		int RPM();
		// Read the current target RPM
		int target_RPM();
		// Activate or deactivate the flywheel control task
		void set_active(bool state);
		// Check if the flywheel control task is active
		bool is_active();
		// Flywheel task
		void flyControl();
};