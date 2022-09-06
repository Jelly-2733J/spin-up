#include "main.h"

// Chassis constructor
Drive chassis (
	// Left Chassis Ports (negative port will reverse it!)
	//   the first port is the sensored port (when trackers are not used!)
	{-1, -2, -3}

	// Right Chassis Ports (negative port will reverse it!)
	//   the first port is the sensored port (when trackers are not used!)
	,{8, 9, 10}

	// IMU Port
	,18

	// Wheel Diameter (Remember, 4" wheels are actually 4.125!)
	//    (or tracking wheel diameter)
	,3.25

	// Cartridge RPM
	//   (or tick per rotation if using tracking wheels)
	,600

	// External Gear Ratio (MUST BE DECIMAL)
	//    (or gear ratio of tracking wheel)
	// eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
	// eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
	,1.667

	// Uncomment if using tracking wheels
	/*
	// Left Tracking Wheel Ports (negative port will reverse it!)
	// ,{1, 2} // 3 wire encoder
	// ,8 // Rotation sensor

	// Right Tracking Wheel Ports (negative port will reverse it!)
	// ,{-3, -4} // 3 wire encoder
	// ,-9 // Rotation sensor
	*/

	// Uncomment if tracking wheels are plugged into a 3 wire expander
	// 3 Wire Port Expander Smart Port
	// ,1
);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

	// Print our branding over your terminal :D
	ez::print_ez_template();

	pros::delay(500); // Stop the user from doing anything while legacy ports configure.
	
	flywheel.set_active(false);

	// Set roller and vision tracking target to signature 1, red goal
	roller.set_alliance(1);
	aim.set_tracking_sig(1);

	// Configure your chassis controls
	chassis.toggle_modify_curve_with_controller(false); // Enables modifying the controller curve with buttons on the joysticks
	chassis.set_active_brake(0.1); // Sets the active brake kP. We recommend 0.1.
	chassis.set_curve_default(0, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
	default_constants(); // Set the drive to your own constants from autons.cpp!
	exit_condition_defaults(); // Set the exit conditions to your own constants from autons.cpp!

	// These are already defaulted to these buttons, but you can change the left/right curve buttons here!
	// chassis.set_left_curve_buttons (pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT); // If using tank, only the left side is used. 
	// chassis.set_right_curve_buttons(pros::E_CONTROLLER_DIGITAL_Y,    pros::E_CONTROLLER_DIGITAL_A);

	// Autonomous Selector using LLEMU
	ez::as::auton_selector.add_autons({
		Auton("Drive and Turn\n\nDrive forward, turn, come back. ", drive_and_turn),
		Auton("No Auton", no_auton),
		Auton("Example Drive\n\nDrive forward and come back.", drive_example),
		Auton("Example Turn\n\nTurn 3 times.", turn_example),
		Auton("Drive and Turn\n\nSlow down during drive.", wait_until_change_speed),
		Auton("Swing Example\n\nSwing, drive, swing.", swing_example),
		Auton("Combine all 3 movements", combining_movements),
		Auton("Interference\n\nAfter driving forward, robot performs differently if interfered or not.", interfered_example),
	});

	// Initialize chassis and auton selector
	chassis.initialize();
	ez::as::initialize();

	// Clear the LCD for the auton selector
	pros::screen::erase();

	// Start the auton selector
	selector.create();

	pros::delay(250); // Wait for auton selector to finish

	// Create the flywheel control task
	pros::Task flywheel_control([&]{ flywheel.flyControl(); });

	// Create the goal tracking task
	pros::Task goal_tracking([&]{ aim.goalSense(); });

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
	chassis.reset_pid_targets(); // Resets PID targets to 0
	chassis.reset_gyro(); // Reset gyro position to 0
	chassis.reset_drive_sensor(); // Reset drive sensors to 0
	chassis.set_drive_brake(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency.

	ez::as::auton_selector.call_auton(selector.selectedAuton()); // Calls selected auton from autonomous selector.
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

	flywheel.set_active(false);
	flywheel.set_target_RPM(2050);

	uint32_t driver_start = pros::millis();

	while (true) {

		chassis.arcade_standard(ez::SPLIT); // Split arcade drive

		// Adjust flywheel RPM (up & down)
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP) && flywheel.target_RPM() <= 2450) {
			flywheel.set_target_RPM(flywheel.target_RPM() + 50);
		}
		else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN) && flywheel.target_RPM() >= 100) {
			flywheel.set_target_RPM(flywheel.target_RPM() - 50);
		}

		// Intake controls (R1 + R2)
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			intake = 100;
		}
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			intake = -100;
		}
		else {
			intake = 0;
		}

		// Toggle flywheel (Left)
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
			flywheel.set_active(!flywheel.is_active());
			master.clear();
		}

		// Endgame
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT) && pros::millis() - driver_start > 95000) {
			// Do endgame stuff here
		}
		/* if (roller.at_roller()) {
			roller.solve_roller();
		} */

		pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME

	}
}