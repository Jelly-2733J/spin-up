#include "api.h"

#pragma once

// CataController class allows for catapult control during autonomous and driver control
class CataController {
	private:
		bool active = false;
		pros::Mutex active_guard;
		bool operating = false;
		pros::Mutex operating_guard;
		bool driver = false;
		pros::Mutex driver_guard;
	public:
		// Set operation state
		void set_operating(bool state);
		// Check if operating
		bool is_operating();
		// Set speed of cata motors
		void set_motors(int speed);
		// Activate or deactivate the cata control task
		void set_active(bool state);
		// Check if the cata control task is active
		bool is_active();
		// Set driver control state
		void driver_control(bool state);
		// Check if in driver control
		bool is_driver();
		// Check to see if the catapult arm is down
		bool cata_ready();
		// Shoot the cata
		void shoot();
		// Cata task
		void cata_control();
};