#include "main.h"
#include "EZ-Template/sdcard.hpp"
#include "autons.hpp"
#include "globals.hpp"
#include "pros/misc.h"
#include "pros/screen.hpp"

// Chassis constructor
Drive chassis (
	// Left Chassis Ports (negative port will reverse it!)
	//   the first port is the sensored port (when trackers are not used!)
	{-10, -9, -8}

	// Right Chassis Ports (negative port will reverse it!)
	//   the first port is the sensored port (when trackers are not used!)
	,{20, 19, 18}

	// IMU Port
	,17

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
	,1.5

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

	// Cata is inactive for initialization
	cata.set_active(false);

	// Print our branding over your terminal :D
	ez::print_ez_template();
	
	// Configure your chassis controls
	chassis.toggle_modify_curve_with_controller(false); // Enables modifying the controller curve with buttons on the joysticks
	chassis.set_active_brake(0.1); // Sets the active brake kP. We recommend 0.1.
	chassis.set_curve_default(2.5, 2.5); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
	default_constants(); // Set the drive to your own constants from autons.cpp!
	exit_condition_defaults(); // Set the exit conditions to your own constants from autons.cpp!
	
	// These are already defaulted to these buttons, but you can change the left/right curve buttons here!
	// chassis.set_left_curve_buttons (pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT); // If using tank, only the left side is used. 
	// chassis.set_right_curve_buttons(pros::E_CONTROLLER_DIGITAL_Y,    pros::E_CONTROLLER_DIGITAL_A);

	// Autonomous Selector using LVGL
	ez::as::auton_selector.add_autons({
		Auton("No Auton              ", no_auton),
		Auton("Right Winpoint", right_winpoint),
		Auton("Left Winpoint", left_winpoint),
		Auton("Solo Winpoint", solo_winpoint),
		Auton("Skills", auton_skills),
	});

	// Initialize chassis and auton selector
	chassis.initialize(3900, "/usd/rengoku.gif");
	ez::as::initialize("/usd/jellyblackoutmenuglitch.gif", "/usd/rengokujellydragon.gif");

	pros::delay(250); // Wait for auton selector to finish

	// Disable driver control for the cata
	cata.driver_control(false);

	// Create the cata control task
	pros::Task cata_control([&]{ cata.cata_control(); });
	
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	// Disable driver control for the cata
	cata.driver_control(false);

	// Set cata to inactive to prevent the code from attempting to control a disabled motor
	cata.set_active(false);
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
	// Disable driver control for the cata
	cata.driver_control(false);

	chassis.reset_pid_targets(); // Resets PID targets to 0
	chassis.reset_gyro(); // Reset gyro position to 0
	chassis.reset_drive_sensor(); // Reset drive sensors to 0
	chassis.set_drive_brake(pros::E_MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency.
	chassis.set_active_brake(0.1); // Sets the active brake kP. We recommend 0.1.

	ez::as::auton_selector.call_selected_auton(); // Calls selected auton from autonomous selector.
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

	// Activate cata and enable driver control
	cata.set_active(true);
	cata.driver_control(true);

	chassis.set_drive_brake(pros::E_MOTOR_BRAKE_COAST);
	chassis.set_active_brake(0.0); // Sets the active brake kP to 0.0.  This disables active braking.

	bool endgame_state = false;
	bool blooper_state = true;
	bool new_press = true;

	// Keep track of when teleop starts to prevent early expansion
	uint32_t driver_start = pros::millis();

	while (true) {

		chassis.tank(); // Tank drive

		// Endgame
		if ((pros::millis() - driver_start > 96000 || master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) && new_press && master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && master.get_digital(pros::E_CONTROLLER_DIGITAL_L2) && master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			endgame_state = !endgame_state;
			endgame.set_value(endgame_state);
			new_press = false;

			// Skip the rest of the driver control loop to avoid accidental shooting
			pros::delay(ez::util::DELAY_TIME); // Used for timing calculations and reasonable loop speeds
			continue;
		}
		else if (!master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_L2) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			new_press = true;
		}

		// Shoot the cata
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
			cata.shoot();
		}

		pros::delay(ez::util::DELAY_TIME); // Used for timing calculations and reasonable loop speeds

	}
}