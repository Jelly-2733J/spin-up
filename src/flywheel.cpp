#include "main.h"
#include <cmath>
#include <mutex>

#include "ports.hpp"
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
	std::lock_guard<pros::Mutex> lock(flywheel_target_RPM_guard);
	flywheel_target_RPM = rpm;
};
// Read the current RPM
int FlywheelController::RPM() {
	return (int) (fly.get_actual_velocity() * 6.0);
};
// Read the current RPM
int FlywheelController::target_RPM() {
	std::lock_guard<pros::Mutex> lock(flywheel_target_RPM_guard);
	return flywheel_target_RPM;
};
// Activate or deactivate the flywheel control task
void FlywheelController::set_active(bool state) {
	std::lock_guard<pros::Mutex> lock(active_guard);
	active = state;
};
// Check if the flywheel control task is active
bool FlywheelController::is_active() {
	std::lock_guard<pros::Mutex> lock(active_guard);
	return active;
};
// Flywheel task
void FlywheelController::flyControl() {

	// t is the last loop timestamp in milliseconds
	// This is used to ensure consistent loop intervals with pros::Task::delay_until
	uint32_t t = pros::millis();

	// Variable to hold previous loop's error
	int previous_error = 0;

	// Start integral at a calculated value to reduce spin up time
	double integral = target_RPM() * 3;

	// PID constants
	double kP = 0.01;
	double kI = 0.03;
	double kD = 0.00;
	
	while (true) {

		// Stop flywheel if not active
		if (!is_active()) {

			fly.brake();

			pros::Task::delay_until(&t, 10);
			continue;
		}

		// Calculate error
		double error = target_RPM() - RPM();

		// Calculate integral
		integral = integral + error;

		// Keep integral in sane range
		integral = clip(integral, 400000, -400000);

		// Calculate derivative
		double derivative = (error - previous_error) / 0.010;

		// Calculate output of PID controller
		double volt = kP * error + kI * integral + kD * derivative;

		// We're not going backwards!
		if (volt < 0.0) {
			volt = 0.0;
		}

		// Clip voltage to motor limits
		volt = clip(volt, 12000, 0);

		printf("%.2f %d %d\n", volt, RPM(), target_RPM());

		// Set motor voltage
		fly.move_voltage(volt);

		// Update previous error
		previous_error = error;

		// Delay next loop until 10 ms have passed from the start of this loop
		pros::Task::delay_until(&t, 10);

	}

};