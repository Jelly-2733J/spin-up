#include "main.h"
#include "pros/misc.hpp"

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
	pros::Task::delay(1000);

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

	while (true) {
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
			pto.set_value(!state);
			state = !state;
		}
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
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
	}
}
