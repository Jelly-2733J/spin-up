#include "api.h"

#pragma once

// FlywheelController class allows for automated flywheel control and smart shooting
class FlywheelController {
	private:
		int flywheel_target_RPM;
		pros::Mutex flywheel_target_RPM_guard;
		bool active = false;
		pros::Mutex active_guard;
		bool full = false;
		pros::Mutex full_guard;
		// Convert degrees to radians
		static double rads(double deg);
		// Check the sign of a number
		bool check_sign(double num);
	public:
		// Set target RPM
		void set_target_RPM(int rpm);
		// Read the current RPM
		double RPM();
		// Read the current target RPM
		double target_RPM();
		// Activate or deactivate the flywheel control task
		void set_active(bool state);
		// Check if the flywheel control task is active
		bool is_active();
		// Toggle going full voltage on the flywheel
		void full_voltage(bool state);
		// Check if set to full voltage
		bool is_full();
		// Check to see if a disc is properly indexed and ready to shoot
		bool disc_indexed();
		// Shoot a number of discs
		void shoot(int num_discs, int timeout = 3000, int rpm_accuracy = 30);
		// Flywheel task
		void fly_control();
};