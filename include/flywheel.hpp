#include "api.h"

#pragma once

// FlywheelController class allows for automated flywheel control and smart shooting
class FlywheelController {
	private:
		double prev_RPMs [5];
		int flywheel_target_RPM;
		pros::Mutex flywheel_target_RPM_guard;
		bool active = false;
		pros::Mutex active_guard;
		int controlmode = 0;
		pros::Mutex controlmode_guard;
		// Convert degrees to radians
		static double rads(double deg);
		// Check the sign of a number
		bool check_sign(double num);
	public:
		// Constructor
		FlywheelController();
		// Set target RPM
		void set_target_RPM(int rpm);
		// Read the current RPM using an Exponential Moving Average
		double RPM();
		// Read the current target RPM
		double target_RPM();
		// Activate or deactivate the flywheel control task
		void set_active(bool state);
		// Check if the flywheel control task is active
		bool is_active();
		// Set flywheel control mode
		// 0: TBV
		// 1: Bang-bang
		// 2: Full voltage
		void set_control_mode(int mode);
		// Check flywheel control mode
		int control_mode();
		// Wait for the flywheel to reach the target RPM
		void wait_for_target_RPM(int timeout = 3000);
		// Shoot a number of discs
		void shoot(int num_discs, int current_discs = 3, int delay3 = 650, int delay2 = 500);
		// Flywheel task
		void fly_control();
};