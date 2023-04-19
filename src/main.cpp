#include "main.h"
#include "EZ-Template/auton.hpp"
#include "EZ-Template/sdcard.hpp"
#include "autons.hpp"
#include "globals.hpp"
#include "pros/misc.h"
#include "pros/screen.hpp"

// Chassis constructor
Drive chassis (
	// Left Chassis Ports (negative port will reverse it!)
	//   the first port is the sensored port (when trackers are not used!)
	{-1, -2, 3}

	// Right Chassis Ports (negative port will reverse it!)
	//   the first port is the sensored port (when trackers are not used!)
	,{8, 9, -10}

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

	// Flywheel is inactive for initialization
	flywheel.set_active(false);

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
		Auton("Right Max", right_max),
		Auton("Right NI", right_ni),
		Auton("Right R", right_r),
		Auton("Left Max", left_max),
		Auton("Left NI", left_ni),
		Auton("Left R", left_r),
		Auton("Solo WP", solo_wp),
		Auton("Nerfed Skills", nerfed_skills),
		Auton("Skills", auton_skills),
	});

	master.clear();
	pros::delay(50);
	master.set_text(0, 0, "      YOU HAVE");
	pros::delay(50);
	master.set_text(1, 0, "     NO ENEMIES");

	// Initialize chassis and auton selector
	chassis.initialize(3000, "/usd/tanjirobootup.gif");
	ez::as::initialize("/usd/jellyblackoutmenuglitch.gif", "/usd/tanjirosword.gif");

	// Set intake brake mode to hold to improve roller consistency
	intake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	// Clear the LCD for the auton selector
	pros::screen::erase();

	master.clear();
	pros::delay(50);
	master.set_text(0, 0, ez::as::auton_selector.Autons[ez::as::auton_selector.selected_auton].Name);

	pros::delay(200); // Wait for auton selector to finish

	// Create the flywheel control task
	pros::Task flywheel_control([&]{ flywheel.fly_control(); });
	
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	// Set flywheel to inactive to prevent the code from attempting to control a disabled motor
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

	// Activate flywheel
	flywheel.set_active(true);

	// Set blooper to up
	blooper.set_value(true);

	// 2050 RPM is the default flywheel speed
	// It is optimal for ripple shots right at the goal
	flywheel.set_target_RPM(2050);

	chassis.set_drive_brake(pros::E_MOTOR_BRAKE_COAST);
	chassis.set_active_brake(0.0); // Sets the active brake kP to 0.0.  This disables active braking.

	bool endgame_state = false;
	bool blooper_state = true;
	bool actuated_state = false;
	bool new_press = true;

	master.clear();
	pros::delay(50);

	// Keep track of when teleop starts to prevent early expansion
	uint32_t driver_start = pros::millis();

	while (true) {

		chassis.arcade_standard(ez::SPLIT); // Split Arcade (left stick controls forward/backward, right stick controls turning)
		//chassis.tank();

		// Endgame
		if (new_press && master.get_digital(pros::E_CONTROLLER_DIGITAL_L2) && master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			endgame_state = !endgame_state;
			endgame(endgame_state);
			new_press = false;
		}
		else if (!master.get_digital(pros::E_CONTROLLER_DIGITAL_L2) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			new_press = true;
		}

		// Adjust flywheel RPM (up & down)
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP) && flywheel.target_RPM() <= 3950) {
			flywheel.set_target_RPM(flywheel.target_RPM() + 50);
		}
		else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN) && flywheel.target_RPM() >= 100) {
			flywheel.set_target_RPM(flywheel.target_RPM() - 50);
		}

		// Intake controls (R1 + R2)
		// R1 is intake, R2 is outtake
		if (new_press && master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			flywheel.set_control_mode(0);
			intake = 127; // Intake at full speed
		}
		else if (new_press && master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			flywheel.set_control_mode(2);
			intake = -127; // Outtake at full speed
		}
		else if (!master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			flywheel.set_control_mode(0);
			intake = 0;
		}

		// L2 is pure outtake
		if (new_press && master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			intake = -127; // Outtake at full speed
		}
		else if (!master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			intake = 0;
		}

		// L1 is blooper (deflector) hold
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
			
			// If blooper is down, set flywheel to 2550 RPM for corner shots
			if (blooper_state != false) {
				blooper.set_value(false);
				flywheel.set_target_RPM(2550);
			}

			blooper_state = false;
		}
		else {
			
			// If blooper is up, set flywheel to 2050 RPM for ripple shots
			if (blooper_state != true) {
				blooper.set_value(true);
				flywheel.set_target_RPM(2050);
			}

			blooper_state = true;
		}

		// B is actuated intake hold
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
			actuated_state = !actuated_state;
			actuated_intake.set_value(actuated_state);
		}

		// Toggle flywheel (Left)
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
			flywheel.set_active(!flywheel.is_active());
			master.clear();
		}

		// Dumbshoot (Y)
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
			flywheel.dumbshoot(3, 3, 950, 950);
		}

		pros::delay(ez::util::DELAY_TIME); // Used for timing calculations and reasonable loop speeds

	}
}