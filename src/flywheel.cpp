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
// Set the distance from the goal
void FlywheelController::set_RPM(int rpm) {
	std::lock_guard<pros::Mutex> lock(flywheel_RPM_guard);
	flywheel_RPM = rpm;
};
// Set target RPM
void FlywheelController::set_target_RPM(int rpm) {
	std::lock_guard<pros::Mutex> lock(flywheel_target_RPM_guard);
	flywheel_target_RPM = rpm;
};
// Read the current RPM
int FlywheelController::RPM() {
	std::lock_guard<pros::Mutex> lock(flywheel_RPM_guard);
	return flywheel_RPM;
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

	int previous_error = 0;
	double integral = target_RPM() * 3;

	double kP = 0.01;
	double kI = 0.03;
	double kD = 0.00;
	
	while (true) {

		if (!is_active()) {

			fly.brake();

			pros::Task::delay_until(&t, 10);
			continue;
		}

		set_RPM((int) (fly.get_actual_velocity() * 6.0));

		double error = target_RPM() - RPM();
		integral = integral + error;
		integral = clip(integral, 400000, -400000);
		double derivative = (error - previous_error) / 0.010;
		double volt = kP * error + kI * integral + kD * derivative;
		if (volt < 0.0) {
			volt = 0.0;
		}
		volt = clip(volt, 12000, 0);
		printf("%.2f %d %d\n", volt, RPM(), target_RPM());
		fly.move_voltage(volt);
		previous_error = error;

		// Delay next loop until 10 ms have passed from the start of this loop
		pros::Task::delay_until(&t, 10);

	}

};