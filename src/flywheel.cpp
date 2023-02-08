#include "main.h"
#include <cmath>
#include <mutex>
#include <vector>

#include "globals.hpp"
#include "flywheel.hpp"

// Constructor
FlywheelController::FlywheelController(int sma) {
	std::vector<double> prev_RPMs(sma, 0.0);
}

// Convert degrees to radians
double FlywheelController::rads(double deg) {
	return deg * PI / 180;
}
// Check the sign of a number
bool FlywheelController::check_sign(double num) {
	if (num > 0) {
		return true;
	}
	else {
		return false;
	}
}
// Set target RPM
void FlywheelController::set_target_RPM(int rpm) {
	flywheel_target_RPM_guard.take();
	flywheel_target_RPM = (double) rpm;
	flywheel_target_RPM_guard.give();
};
// Read the current RPM using a simple moving average
double FlywheelController::RPM() {

	// Get the current flywheel RPM
	double current_RPM = fly.get_actual_velocity() * 6.0;

	// Remove the first element of the vector and add the current RPM to the end
	prev_RPMs.erase(prev_RPMs.begin());
	prev_RPMs.push_back(current_RPM);

	// Calculate and return the average of the vector using std::accumulate
	return std::accumulate(prev_RPMs.begin(), prev_RPMs.end(), 0.0) / prev_RPMs.size();
	
};
// Read the current target RPM
double FlywheelController::target_RPM() {
	flywheel_target_RPM_guard.take();
	double to_return = flywheel_target_RPM;
	flywheel_target_RPM_guard.give();
	return to_return;
};
// Activate or deactivate the flywheel control task
void FlywheelController::set_active(bool state) {
	active_guard.take();
	active = state;
	active_guard.give();
};
// Check if the flywheel control task is active
bool FlywheelController::is_active() {
	active_guard.take();
	bool to_return = active;
	active_guard.give();
	return to_return;
};
// Toggle going full voltage on the flywheel
void FlywheelController::full_voltage(bool state) {
	full_guard.take();
	full = state;
	full_guard.give();
};
// Check if set to full voltage
bool FlywheelController::is_full() {
	full_guard.take();
	bool to_return = full;
	full_guard.give();
	return to_return;
};
// Check to see if a disc is properly indexed and ready to shoot
bool FlywheelController::disc_indexed() {
	// return optical.get_proximity() > 235;
	return true;
};
// Shoot a number of discs
void FlywheelController::shoot(int num_discs, int timeout, int rpm_accuracy) {
	
	int count = 0;

	// Shoot number of times specified
	for (int i = 0; i < num_discs; i++) {

		std::cout << "WAITING FOR RPM\n" << std::endl;

		// Wait for RPM to be within accuracy and a disc to be in the proper indexing position
		while (!(abs(flywheel.target_RPM() - flywheel.RPM()) < rpm_accuracy)) {
			// If the timeout is reached, exit
			if (count >= timeout) {
				intake = 0;
				return;
			}
			count += 10;
			pros::delay(10);
		}

		intake = -127;
		
		// Fire disc
		fire();

		intake = 0;

		// Wait 300 ms for next disc to fall into proper indexing position
		pros::delay(300);

	}

	intake = 0;
}
// Flywheel task
void FlywheelController::fly_control() {

	// t is the last loop timestamp in milliseconds
	// This is used to ensure consistent loop intervals with pros::Task::delay_until
	uint32_t t = pros::millis();

	double gain = 0.08;
	double takeback = 0.0;
	double tbv = 0.0;
	bool first_cross = false;
	double max_rpm = 3500.0;

	double error;
	double last_error;
	double voltage = 0.0;

	int count = 0;
	
	while (true) {

		// Check if the flywheel control task is active with is_active()
		if (!is_active()) {

			// If not active, flywheel should stop
			fly = 0;

			// Reset all control variables
			takeback = 0.0;
			first_cross = false;
			error = 0.0;
			last_error = 0.0;
			voltage = 0.0;

			master.clear();

			// Delay next loop until 10 ms have passed from the start of this loop
			pros::Task::delay_until(&t, 10);

			// Continue to next loop, skipping take back variable calculation and voltage control
			continue;
		}
		
		if (is_full()) {
			// If we're shooting, we want full power for best recovery
			fly.move_voltage(12000);
		}
		else {
			// Calculate error
			error = target_RPM() - RPM();

			// Calculate variable take back
			tbv = 0.5 - (2050.0 - target_RPM()) / 100000.0;

			// Integrate
			voltage += gain * error;

			// Keep voltage within bounds
			voltage = std::clamp(voltage, -12000.0, 12000.0);

			// Take back variable if there is a switch in the sign of the errors
			if (check_sign(last_error) != check_sign(error)) {
				// On first error cross, set takeback to theoretical voltage estimate to reach a quicker stable state
				if (first_cross == false) {
					first_cross = true;
					takeback = ((double) target_RPM() / max_rpm) * 12000;
				}

				// Perform take back variable calculation and clamp voltage to bounds
				voltage = std::clamp(tbv * (voltage + takeback), -12000.0, 12000.0);

				// Set takeback to new voltage
				takeback = voltage;
				
				// printf("     TBV %.2f     \n", tbv);
			}

			// Set last_error to current error for next loop
			last_error = error;

			// When we're not shooting, we want to maintain a stable state
			fly.move_voltage(voltage);
		}

		printf("%.2f %d %d\n", voltage, (int) RPM(), (int) target_RPM());

		if (count == 300) {
			master.print(2, 0, "TEMP: %d C", (int) fly.get_temperature());
			count = 0;
		} else if (count == 200) {
			master.print(1, 0, "TGT: %d", (int) target_RPM());
		} else if (count == 100) {
			master.print(0, 0, "RPM: %d", (int) RPM());
		}
		
		count += 10;

		// Delay next loop until 10 ms have passed from the start of this loop
		pros::Task::delay_until(&t, 10);

	}

};