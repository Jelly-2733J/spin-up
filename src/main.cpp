#include "main.h"
#include "pros/misc.h"
#include "pros/rtos.hpp"

// Instantiate the AutoAim class
AutoAim aim;

// Instantiate the FlywheelController class
FlywheelController flywheel;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

	// Initialize the LCD
	pros::lcd::initialize();
	pros::delay(1000);

	// Set all pneumatics to their default state
	pto.set_value(false);
	shooter.set_value(false);

	// Create the goal tracking task
	pros::Task goal_tracking([&]{ aim.goalSense(); });

	// Create the flywheel control task
	pros::Task flywheel_control([&]{ flywheel.flyControl(); });
	flywheel.set_active(false);

	// Set tracking target to signature 1, red goal
	aim.set_tracking_sig(1);

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	flywheel.set_active(false);
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	flywheel.set_active(true);
}

int curve(int input) {
	// Curve function (positive numbers only)
	// output = 1.02 ^ (input + 122) - 11.2
	if (input == 0) {
		return 0;
	}
	else if (abs(input) == 127) {
		return (input > 0 ? 1 : -1) * 127;
	}
	else {
		return (input > 0 ? 1 : -1) * (int) (1.02 * (pow(1.02, (abs(input) + 122)) - 11.2));
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	flywheel.set_active(true);
	flywheel.set_target_RPM(2000);

	bool state = false;

	pros::Controller master(pros::E_CONTROLLER_MASTER);

	uint32_t driver_start = pros::millis();

	// t is the last loop timestamp in milliseconds
	// This is used to ensure consistent loop intervals with pros::Task::delay_until
	uint32_t t = driver_start;

	while (true) {
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
			pto.set_value(!state);
			state = !state;
		}
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
			shooter.set_value(true);
			pros::Task::delay(50);
			shooter.set_value(false);
		}
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP) && flywheel.target_RPM() <= 3400) {
			flywheel.set_target_RPM(flywheel.target_RPM() + 100);
		}
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN) && flywheel.target_RPM() >= 100) {
			flywheel.set_target_RPM(flywheel.target_RPM() - 100);
		}

		// Tank drive with left and right joystick Y values
		drivefl.move(curve(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)));
		drivefr.move(curve(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)));
		drivebl.move(curve(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)));
		drivebr.move(curve(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)));
		if (state) {
			ptol.move(curve(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)));
			ptor.move(curve(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)));
		}
		// Intake forward
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			ptol.move(127);
			ptor.move(127);
		}
		// Intake backward
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
			ptol.move(-127);
			ptor.move(-127);
		}
		else {
			ptol.move(0);
			ptor.move(0);
		}

		// Endgame
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT) && t - driver_start > 95000) {
			// Do endgame stuff here
		}

		// Delay next loop until 10 ms have passed from the start of this loop
		pros::Task::delay_until(&t, 2);
	}
}
