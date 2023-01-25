#include "EZ-Template/util.hpp"
#include "main.h"
#include <cmath>
#include <mutex>

#include "globals.hpp"
#include "cata.hpp"
#include "pros/misc.h"

// Set operation state
void CataController::set_operating(bool state) {
	operating_guard.take();
	operating = state;
	operating_guard.give();
};
// Check if operating
bool CataController::is_operating() {
	operating_guard.take();
	bool to_return = operating;
	operating_guard.give();
	return to_return;
};
// Set speed of cata motors
void CataController::set_motors(int speed) {
	cata1.move(speed);
	cata2.move(speed);
};
// Activate or deactivate the cata control task
void CataController::set_active(bool state) {
	active_guard.take();
	active = state;
	active_guard.give();
};
// Check if the cata control task is active
bool CataController::is_active() {
	active_guard.take();
	bool to_return = active;
	active_guard.give();
	return to_return;
};
// Set driver control state
void CataController::driver_control(bool state) {
	driver_guard.take();
	driver = state;
	driver_guard.give();
};
// Check if in driver control
bool CataController::is_driver() {
	driver_guard.take();
	bool to_return = driver;
	driver_guard.give();
	return to_return;
};
// Check to see if the cata arm is down
bool CataController::cata_ready() {
	return cata_switch.get_value();
};
// Shoot the cata
void CataController::shoot() {
	
	// Set operating state to true
	set_operating(true);

	int timeout = 0;

	// Reverse cata motors to pull back until cata isn't ready anymore (meaning it has shot)
	while (cata_ready() && timeout < 1000) {
		if (is_driver()) {
			chassis.tank();
		}
		timeout += 10;
		set_motors(-127);
		pros::delay(ez::util::DELAY_TIME);
	}

	// Set operating state to false
	set_operating(false);

}
// Cata task
void CataController::cata_control() {

	// t is the last loop timestamp in milliseconds
	// This is used to ensure consistent loop intervals with pros::Task::delay_until
	uint32_t t = pros::millis();

	bool was_ready = true;
	
	while (true) {

		// Check if the cata control task is active with is_active() and that the cata isn't in operation with is_operating()
		if (!is_active() || is_operating()) {

			master.clear();

			// Delay next loop until 10 ms have passed from the start of this loop
			pros::Task::delay_until(&t, 10);

			// Continue to next loop
			continue;
		}
		// Cata should pull back if not already down
		else if (!cata_ready()) {
			set_motors(-127);
		}
		// Cata should stop if not in operation and not in driver control
		else if (!is_driver()) {
			set_motors(0);
		}
		// If cata is in driver control, it should be controlled by R1 with L1 overriding the cata_ready() check
		else {
			set_motors((master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && cata_ready()) || master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) ? 127 : 0);
		}

		// Print ready status
		if (cata_ready() && !was_ready) {
			master.rumble(".");
			master.print(0, 0, "READY: YES");
			was_ready = true;
		} else if (!cata_ready() && was_ready) {
			master.print(0, 0, "READY: NO");
			was_ready = false;
		}

		// Print temperature
		if (t % 100 == 0) {
			master.print(1, 0, "TEMP: %d %d", (int) cata1.get_temperature(), (int) cata2.get_temperature());
		}

		// Delay next loop until 10 ms have passed from the start of this loop
		pros::Task::delay_until(&t, 10);

	}

};