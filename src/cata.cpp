#include "main.h"
#include <cmath>
#include <mutex>

#include "globals.hpp"
#include "cata.hpp"

// Activate or deactivate the catapult control task
void CataController::set_active(bool state) {
	active_guard.take();
	active = state;
	active_guard.give();
};
// Check if the catapult control task is active
bool CataController::is_active() {
	active_guard.take();
	bool to_return = active;
	active_guard.give();
	return to_return;
};
// Set shooting state
void CataController::set_shooting(int discs) {
	shooting_guard.take();
	shooting = discs;
	shooting_guard.give();
};
// Wait until catapult is loaded
// timeout is 0 for no timeout
void CataController::wait_for_load(int timeout) {
	if (timeout == 0) {
		while (!cata_switch.get_value()) {
			pros::delay(10);
		}
	}
	else {
		int start_time = pros::millis();
		while (!cata_switch.get_value() && pros::millis() - start_time < timeout) {
			pros::delay(10);
		}
	}
};
// Returns the number of discs the catapult is shooting
// 0 if the catapult is not shooting
int CataController::is_shooting() {
	shooting_guard.take();
	bool to_return = shooting;
	shooting_guard.give();
	return to_return;
};
// Shoot a number of discs
void CataController::shoot(int num_discs, bool wait) {
	set_shooting(num_discs);
	if (wait) {
		while (is_shooting() > 0) {
			pros::delay(10);
		}
	}
};
// Catapult task
void CataController::cataControl() {

	// t is the last loop timestamp in milliseconds
	// This is used to ensure consistent loop intervals with pros::Task::delay_until
	uint32_t t = pros::millis();

	bool previous_switch = cata_switch.get_value();

	double error;
	double last_error;
	double voltage = 0.0;

	int count = 0;
	
	while (true) {

		// Check if the catapult control task is active with is_active()
		if (!is_active()) {

			// If not active, catapult should stop
			cata = 0;

			master.clear();

			// Delay next loop until 10 ms have passed from the start of this loop
			pros::Task::delay_until(&t, 10);

			// Continue to next loop, skipping motor control
			continue;
		}
		
		if (!cata_switch.get_value() || is_shooting()) {
			cata = 100;
		}
		else {
			cata = 0;
		}

		if (cata_switch.get_value() != previous_switch) {
			set_shooting(0);
		}

		if (count == 200) {
			master.print(1, 0, "TEMP: %d C", (int) cata.get_temperature());
			count = 0;
		}
		else if (count == 100) {
			master.print(0, 0, "LOADED: %s", cata_switch.get_value() ? "YES" : "NO");
		}
		
		count += 10;

		// Delay next loop until 10 ms have passed from the start of this loop
		pros::Task::delay_until(&t, 10);

	}

};