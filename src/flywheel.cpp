#include "main.h"
#include <cmath>
#include <mutex>

#include "globals.hpp"
#include "flywheel.hpp"

// Convert degrees to radians
double FlywheelController::rads(double deg) {
	return deg * 3.14159265358979323846 / 180;
}
// Calculate optimal RPM
int FlywheelController::calculate_RPM(double distance) {
	
	// TODO: Experimentally find best fit curve for different distances
	// Graph should be RPM (dependent) vs. Distance (independent)
	return distance; // Add equation here
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
// Clip a number to a certain range
double FlywheelController::clip(double num, double max, double min) {
	if (num < min) {
		return min;
	}
	else if (num > max) {
		return max;
	}
	else {
		return num;
	}
}
// Set target RPM
void FlywheelController::set_target_RPM(int rpm) {
	flywheel_target_RPM_guard.take();
	flywheel_target_RPM = rpm;
	flywheel_target_RPM_guard.give();
};
// Read the current RPM
int FlywheelController::RPM() {
	return (int) (fly.get_actual_velocity() * 6.0);
};
// Read the current RPM
int FlywheelController::target_RPM() {
	flywheel_target_RPM_guard.take();
	int to_return = flywheel_target_RPM;
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
// Shoot a number of discs
void FlywheelController::shoot(int num_discs, int timeout) {
	int count = 0;
	for (int i = 0; i < num_discs; i++) {
		while (flywheel.RPM() < flywheel.target_RPM() - 80) {
			if (count >= timeout) {
				break;
			}
			count += 10;
			pros::delay(10);
		}

		intake = -80;

		while (flywheel.RPM() > flywheel.target_RPM() - 300) {
			if (count >= timeout) {
				break;
			}
			count += 10;
			pros::delay(10);
		}

		intake = 100;
	}
}
// Ripple discs
void FlywheelController::ripple(int num_discs, int timeout) {
	int count = 0;
	for (int i = 0; i < num_discs; i++) {

		intake = -80;

		while (flywheel.RPM() > flywheel.target_RPM() - 300) {
			if (count >= timeout) {
				break;
			}
			count += 10;
			pros::delay(10);
		}
	}
}
// Flywheel task
void FlywheelController::flyControl() {

	// t is the last loop timestamp in milliseconds
	// This is used to ensure consistent loop intervals with pros::Task::delay_until
	uint32_t t = pros::millis();

	double gain = 0.08;
	double tbh = 0.0;
	bool first_cross = false;
	int max_rpm = 3000;

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
			tbh = 0.0;
			first_cross = false;
			error = 0.0;
			last_error = 0.0;
			voltage = 0.0;

			master.clear();

			// Delay next loop until 10 ms have passed from the start of this loop
			pros::Task::delay_until(&t, 10);

			// Continue to next loop, skipping tbh calculation and voltage control
			continue;
		}

		// Calculate error
		error = target_RPM() - RPM();

		// Integrate
		voltage += gain * error;

		// Keep voltage within bounds
		voltage = clip(voltage, 12000, -12000);

		// TBH if there is a switch in the sign of the errors
		if (check_sign(last_error) != check_sign(error)) {
			// On first error cross, set tbh to theoretical voltage estimate to reach a quicker stable state
			if (first_cross == false) {
				first_cross = true;
				tbh = ((double) target_RPM() / max_rpm) * 12000;
			}
			
			// Perform TBH calculation and clip voltage to bounds
			voltage = clip(0.7 * (voltage + tbh), 12000, -12000);

			// Set tbh to new voltage
			tbh = voltage;
			
			printf("     TBH     \n");
		}

		// Set last_error to current error for next loop
		last_error = error;
		
		// Set flywheel voltage
		fly.move_voltage(voltage);

		printf("%.2f %d %d\n", voltage, RPM(), target_RPM());

		if (count == 300) {
			master.print(2, 0, "TEMP: %d C", (int) fly.get_temperature());
			count = 0;
		} else if (count == 200) {
			master.print(1, 0, "TGT: %d", target_RPM());
		} else if (count == 100) {
			master.print(0, 0, "RPM: %d", RPM());
		}
		
		count += 10;

		// Delay next loop until 10 ms have passed from the start of this loop
		pros::Task::delay_until(&t, 10);

	}

};