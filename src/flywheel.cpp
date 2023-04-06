#include "main.h"
#include <cmath>
#include <mutex>

#include "globals.hpp"
#include "flywheel.hpp"

// Constructor
FlywheelController::FlywheelController() {
	for (int i = 0; i < 5; i++) {
		prev_RPMs[i] = 0.0;
	}
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

	// Update the SMA array
	for (int i = 0; i < 4; i++) {
		prev_RPMs[i] = prev_RPMs[i + 1];
	}
	prev_RPMs[4] = current_RPM;

	// Calculate the SMA
	double sum = 0.0;
	for (int i = 0; i < 5; i++) {
		sum += prev_RPMs[i];
	}
	return sum / 5.0;
	
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
// Dumbshoot a number of discs
void FlywheelController::dumbshoot(int num_discs, int current_discs) {

	full_voltage(true);

	// If 3 discs
	if (current_discs == 3 && num_discs > 0) {
		intake = -80;
		pros::delay(60);
		intake = 0;
		pros::delay(400);
		current_discs--;
		num_discs--;
	}

	// If 2 discs
	if (current_discs == 2 && num_discs > 0) {
		intake = -80;
		pros::delay(100);
		intake = 0;
		pros::delay(400);
		current_discs--;
		num_discs--;
	}

	// If 1 disc
	if (current_discs == 1 && num_discs > 0) {
		intake = -80;
		pros::delay(200);
		intake = 0;
		pros::delay(400);
		current_discs--;
		num_discs--;
	}

	full_voltage(false);
}
// Shoot a number of discs
void FlywheelController::shoot(int num_discs, int timeout, int rpm_accuracy) {
	int count = 0;

	// Shoot number of times specified
	for (int i = 0; i < num_discs; i++) {

		// Full voltage for best RPM recovery and fastest possible shooting
		full_voltage(true);

		// Wait for RPM to be within accuracy
		while (abs(flywheel.target_RPM() - flywheel.RPM()) > rpm_accuracy) {
			// If the timeout is reached, exit
			if (count >= timeout) {
				intake = 100;
				full_voltage(false);
				return;
			}
			// If flywheel RPM is above the target, disable full voltage so that it can decrease
			if (flywheel.RPM() > flywheel.target_RPM()) {
				full_voltage(false);
				fly.move_voltage(0);
			}
			count += 10;
			pros::delay(10);
		}

		// Outtake to shoot
		intake = -80;
		full_voltage(true);

		// Wait for optical sensor to detect a shot
		while (optical.get_proximity() < 70) {
			// If the timeout is reached, exit
			if (count >= timeout) {
				intake = 0;
				full_voltage(false);
				return;
			}
			count += 10;
			pros::delay(10);
		}

		// Intake at full to reduce the chance of an accidental double shot
		intake = 127;

		// Wait for optical sensor to detect that there isn't a disc being currently shot
		while (optical.get_proximity() > 70) {
			// If the timeout is reached, exit
			if (count >= timeout) {
				intake = 0;
				full_voltage(false);
				return;
			}
			count += 10;
			pros::delay(10);
		}
	}

	// Disable full voltage mode after shooting is complete.
	full_voltage(false);
}
// Flywheel task
void FlywheelController::fly_control() {

	// t is the last loop timestamp in milliseconds
	// This is used to ensure consistent loop intervals with pros::Task::delay_until
	uint32_t t = pros::millis();

	double gain = 0.095;
	double takeback = 0.0;
	double tbv = 0.0;
	bool first_cross = false;
	double max_rpm = 3000.0;

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
			tbv = 0.496 + (target_RPM() - 2100.0) / 300000.0;

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