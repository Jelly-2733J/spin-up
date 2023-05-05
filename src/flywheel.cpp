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
// Read the current RPM using an Exponential Moving Average
double FlywheelController::RPM() {

	// Get the current flywheel RPM
	double current_RPM = fly.get_actual_velocity() * 6.0;

	// Update the previous RPMs array
	for (int i = 0; i < 4; i++) {
		prev_RPMs[i] = prev_RPMs[i + 1];
	}
	prev_RPMs[4] = current_RPM;

	// Calculate the Exponential Moving Average
	return prev_RPMs[4] * 0.5 + prev_RPMs[3] * 0.25 + prev_RPMs[2] * 0.125 + prev_RPMs[1] * 0.0625 + prev_RPMs[0] * 0.0625;
	
	
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
// Set flywheel control mode
// 0: TBV
// 1: Bang-bang
// 2: Full voltage
void FlywheelController::set_control_mode(int mode) {
	controlmode_guard.take();
	controlmode = mode;
	controlmode_guard.give();
};
// Check flywheel control mode
int FlywheelController::control_mode() {
	controlmode_guard.take();
	int to_return = controlmode;
	controlmode_guard.give();
	return to_return;
};
// Wait for the flywheel to reach the target RPM
void FlywheelController::wait_for_target_RPM(int timeout) {
	int time = 0;
	while (target_RPM() > RPM() && time < timeout) {
		pros::delay(10);
		time += 10;
	}
	if (time >= timeout) {
		printf("Target of %f not reached in %d ms\n", target_RPM(), time);
	}
	else {
		printf("Target of %f reached in %d ms\n", target_RPM(), time);
	}
};
// Shoot a number of discs
void FlywheelController::shoot(int num_discs, int current_discs, int delay3, int delay2) {

	// Set flywheel to bang-bang mode
	set_control_mode(1);

	// If 3 discs
	if (current_discs == 3 && num_discs > 0) {
		intake = -80;
		pros::delay(100);
		intake = 127;
		pros::delay(300);
		flywheel.wait_for_target_RPM(delay3 - 300);
		current_discs--;
		num_discs--;
	}

	// If 2 discs
	if (current_discs == 2 && num_discs > 0) {
		intake = -80;
		pros::delay(100);
		intake = 127;
		pros::delay(300);
		flywheel.wait_for_target_RPM(delay2 - 300);
		current_discs--;
		num_discs--;
	}

	// If 1 disc
	if (current_discs == 1 && num_discs > 0) {
		intake = -127;
		pros::delay(220);
		intake = 127;
		current_discs--;
		num_discs--;
	}

	// Set flywheel to TBV mode
	set_control_mode(0);
}
// Flywheel task
void FlywheelController::fly_control() {

	// t is the last loop timestamp in milliseconds
	// This is used to ensure consistent loop intervals with pros::Task::delay_until
	uint32_t t = pros::millis();

	double gain = 0.9;
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
		
		// Full voltage
		if (control_mode() == 2) {
			fly.move_voltage(12000);
		}
		// Bang-bang control
		else if (control_mode() == 1) {
			if (RPM() < target_RPM()) {
				fly.move_voltage(12000);
			}
			else {
				fly.move_voltage(target_RPM() / 4000.0 * 12000.0);
			}
		}
		// Take-back-variable control
		else {
			// Calculate error
			error = target_RPM() - RPM();

			// Calculate variable take back
			tbv = 0.498 + (target_RPM() - 2100.0) / 300000.0;

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

		// printf("%.2f %d %d\n", voltage, (int) RPM(), (int) target_RPM());

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