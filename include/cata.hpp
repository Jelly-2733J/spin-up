#include "api.h"

#pragma once

// CataController class allows for automated and asynchronous catapult control.
class CataController {
	private:
		bool active = false;
		pros::Mutex active_guard;
		bool is_loaded = false;
		pros::Mutex is_loaded_guard;
		bool shooting = 0;
		pros::Mutex shooting_guard;
	public:
		// Activate or deactivate the catapult control task
		void set_active(bool state);
		// Check if the catapult control task is active
		bool is_active();
		// Set shooting state
		void set_shooting(int discs);
		// Returns the number of discs the catapult is shooting
		// 0 if the catapult is not shooting
		int is_shooting();
		// Shoot a number of discs
		void shoot(int num_discs);
		// Catapult task
		void cataControl();
};