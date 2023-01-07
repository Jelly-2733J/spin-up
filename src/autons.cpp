#include "EZ-Template/drive/drive.hpp"
#include "EZ-Template/util.hpp"
#include "autons.hpp"
#include "globals.hpp"
#include "main.h"
#include "pros/rtos.hpp"


/////
// For installation, upgrading, documentation, and tutorials, check out website!
// https://ez-robotics.github.io/EZ-Template/
/////


const int DRIVE_SPEED = 110; // This is 110/127 (around 87% of max speed).  We don't suggest making this 127.
                             // If this is 127 and the robot tries to heading correct, it's only correcting by
                             // making one side slower.  When this is 87%, it's correcting by making one side
                             // faster and one side slower, giving better heading correction.
const int TURN_SPEED  = 110;
const int SWING_SPEED = 110;



///
// Constants
///

// It's best practice to tune constants when the robot is empty and with heavier game objects, or with lifts up vs down.
// If the objects are light or the cog doesn't change much, then there isn't a concern here.

void default_constants() {
	chassis.set_slew_min_power(80, 80);
	chassis.set_slew_distance(7, 7);
	chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
	chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
	chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
	chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
	chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void one_mogo_constants() {
	chassis.set_slew_min_power(80, 80);
	chassis.set_slew_distance(7, 7);
	chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
	chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
	chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
	chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
	chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void two_mogo_constants() {
	chassis.set_slew_min_power(80, 80);
	chassis.set_slew_distance(7, 7);
	chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
	chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
	chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
	chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
	chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void exit_condition_defaults() {
	chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
	chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
	chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}

void modified_exit_condition() {
	chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
	chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
	chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}

void getRoller(int distance, int time) {
	chassis.set_drive_pid(distance, DRIVE_SPEED);
	chassis.wait_drive();
	intake = 100;	 
	pros::delay(time);
	intake = 0;
}


///
// No Auton
///
void no_auton() {
	// Do nothing.
}

///
// Right Winpoint
///
void right_winpoint() {

	flywheel.set_active(true);
	flywheel.set_target_RPM(2800);

	chassis.set_drive_pid(-24, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(90, TURN_SPEED);
	chassis.wait_drive();

	getRoller(-10, 190);

	chassis.set_drive_pid(6, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(40, TURN_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_drive_pid(33, DRIVE_SPEED, true);
	chassis.wait_until(20);
	chassis.set_max_speed(50);
	chassis.wait_drive();

	chassis.set_turn_pid(112, TURN_SPEED);
	chassis.wait_drive();

	pros::delay(750);

	flywheel.shoot(3, 3500, 15);

	flywheel.set_target_RPM(2680);

	intake = 100;

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(40, DRIVE_SPEED, true);
	chassis.wait_until(5);
	chassis.set_max_speed(50);
	chassis.wait_drive();

	chassis.set_drive_pid(-20, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(122, TURN_SPEED);
	chassis.wait_drive();

	flywheel.shoot(3, 10000);

}



///
// Left Winpoint
///
void left_winpoint() {

	flywheel.set_active(true);
	flywheel.set_target_RPM(2800);

	getRoller(-3, 190);

	chassis.set_drive_pid(4, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-40, TURN_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_drive_pid(6, 50);
	chassis.wait_drive();

	pros::delay(250);

	chassis.set_drive_pid(-3, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(53, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(14, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_swing_pid(ez::RIGHT_SWING, -17, DRIVE_SPEED);
	chassis.wait_drive();

	flywheel.shoot(3, 1000, 10); // Remove this one

	flywheel.set_active(false);

	/*flywheel.shoot(3, 2500, 10);

	intake = -100;

	chassis.set_swing_pid(ez::RIGHT_SWING, 40, DRIVE_SPEED);
	chassis.wait_drive();

	flywheel.set_target_RPM(2700);

	intake = 100;

	chassis.set_drive_pid(34, DRIVE_SPEED, true);
	chassis.wait_until(5);
	chassis.set_max_speed(30);
	chassis.wait_until(12);
	chassis.set_max_speed(50);
	chassis.wait_drive();
	
	chassis.set_swing_pid(ez::LEFT_SWING, -28, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(8, DRIVE_SPEED);
	chassis.wait_drive();

	pros::delay(250);

	flywheel.shoot(3, 5000, 30);*/
}

///
// Solo Winpoint
///
void solo_winpoint() {
	// make this mattias
	flywheel.set_active(true);
	flywheel.set_target_RPM(2660);

	getRoller(-3, 190);

	chassis.set_drive_pid(5, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(75, 127);
	chassis.wait_drive();

	chassis.set_drive_pid(35, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(26, 127);
	chassis.wait_drive();

	intake = 100;

	chassis.set_drive_pid(36, 127, true);
	chassis.wait_drive();

	chassis.set_swing_pid(ez::RIGHT_SWING, -47, 127);
	chassis.wait_drive();

	pros::delay(500);

	flywheel.shoot(3, 3000);

	// chassis.set_drive_pid(-7, 127);
	// chassis.wait_drive();

	flywheel.set_target_RPM(3070);

	intake = 100;

	chassis.set_turn_pid(45, 127);
	chassis.wait_drive();

	chassis.set_drive_pid(59, 110, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-90, 127);
	chassis.wait_drive();

	intake = 0;

	getRoller(-14, 190);

	chassis.set_drive_pid(5, 50);
	chassis.wait_drive();

	chassis.set_turn_pid(-79, 127);
	chassis.wait_drive();

	flywheel.shoot(3, 10000);

}

void ripple() {
	intake = -100;
	flywheel.full_voltage(true);
	pros::delay(1500);
	flywheel.full_voltage(false);
	intake = 0;
}

void triple_tap() {
	intake = -80;
	flywheel.full_voltage(true);
	pros::delay(300);
	intake = 100;
	pros::delay(350);
	intake = -80;
	pros::delay(500);
	flywheel.full_voltage(false);
	intake = 0;
}

///
// Skills
///
void auton_skills() {
	
	flywheel.set_active(true);
	flywheel.set_target_RPM(2250);
	blooper.set_value(true);

	getRoller(-3, 380);

	chassis.set_drive_pid(4, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-30, TURN_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_drive_pid(27, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(90, TURN_SPEED);
	chassis.wait_drive();

	pros::delay(300);

	intake = 0;

	getRoller(-13, 380);

	chassis.set_drive_pid(10, DRIVE_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_turn_pid(0, TURN_SPEED);
	chassis.wait_drive();

	pressure_bar.set_value(true);

	chassis.set_drive_pid(44, DRIVE_SPEED, true);
	chassis.wait_drive();

	flywheel.shoot(3, 2000);

	intake = -100;

	flywheel.set_target_RPM(2550);

	chassis.set_drive_pid(-39, DRIVE_SPEED, true);
	chassis.wait_drive();

	pressure_bar.set_value(false);
	blooper.set_value(false);

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_drive_pid(64, 70, true);
	chassis.wait_drive();

	chassis.set_drive_pid(-3, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-45, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-8, DRIVE_SPEED);
	chassis.wait_drive();

	pros::delay(300);

	flywheel.shoot(3, 2000);

	intake = -100;

	flywheel.set_target_RPM(2250);

	chassis.set_drive_pid(-8, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_drive_pid(35, 60);
	chassis.wait_drive();

	chassis.set_turn_pid(0, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(30, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-90, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_swing_pid(ez::LEFT_SWING, -180, SWING_SPEED);
	chassis.wait_drive();

	intake = 0;

	getRoller(-7, 380);

	// OTHER SIDE
	chassis.set_angle(0);

	chassis.set_drive_pid(4, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-45, TURN_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_drive_pid(30, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(90, TURN_SPEED);
	chassis.wait_drive();

	intake = 0;

	getRoller(-9, 380);

	chassis.set_drive_pid(10, DRIVE_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_turn_pid(0, TURN_SPEED);
	chassis.wait_drive();

	pressure_bar.set_value(true);
	blooper.set_value(true);

	chassis.set_drive_pid(44, DRIVE_SPEED, true);
	chassis.wait_drive();
	
	flywheel.shoot(3, 2000);

	intake = -100;

	flywheel.set_target_RPM(2550);

	chassis.set_drive_pid(-39, DRIVE_SPEED, true);
	chassis.wait_drive();

	pressure_bar.set_value(false);
	blooper.set_value(false);

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_drive_pid(72, 70, true);
	chassis.wait_drive();

	chassis.set_drive_pid(-6, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-48, TURN_SPEED);
	chassis.wait_drive();

	pros::delay(300);

	flywheel.shoot(3, 2000);

	intake = -100;

	flywheel.set_target_RPM(2250);

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_drive_pid(66, 90, true);
	chassis.wait_until(30);
	chassis.set_max_speed(25);
	chassis.wait_until(42);
	chassis.set_max_speed(35);
	chassis.wait_drive();

	chassis.set_drive_pid(-8, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-90, TURN_SPEED);
	chassis.wait_drive();

	pressure_bar.set_value(true);
	blooper.set_value(true);

	chassis.set_drive_pid(32, DRIVE_SPEED, true);
	chassis.wait_drive();

	flywheel.shoot(3, 2000);

	intake = -100;

	chassis.set_turn_pid(-247, TURN_SPEED);
	chassis.wait_drive();

	pressure_bar.set_value(false);

	intake = 100;

	chassis.set_drive_pid(60, DRIVE_SPEED, true);
	chassis.wait_until(18);
	chassis.set_max_speed(30);
	chassis.wait_until(40);
	chassis.set_max_speed(40);
	chassis.wait_drive();

	chassis.set_turn_pid(-182, TURN_SPEED);
	chassis.wait_drive();

	pressure_bar.set_value(true);

	chassis.set_drive_pid(36, DRIVE_SPEED, true);
	chassis.wait_drive();

	flywheel.shoot(3, 2000);

	chassis.set_turn_pid(-188, TURN_SPEED);
	chassis.wait_drive();

	intake = -100;

	flywheel.set_active(false);

	chassis.set_drive_pid(-59, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-135, TURN_SPEED);
	chassis.wait_drive();

	endgame.set_value(true);

}



///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed() {
	// wait_until will wait until the robot gets to a desired position


	// When the robot gets to 6 inches, the robot will travel the remaining distance at a max speed of 40
	chassis.set_drive_pid(24, DRIVE_SPEED, true);
	chassis.wait_until(6);
	chassis.set_max_speed(40); // After driving 6 inches at DRIVE_SPEED, the robot will go the remaining distance at 40 speed
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-45, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(0, TURN_SPEED);
	chassis.wait_drive();

	// When the robot gets to -6 inches, the robot will travel the remaining distance at a max speed of 40
	chassis.set_drive_pid(-24, DRIVE_SPEED, true);
	chassis.wait_until(-6);
	chassis.set_max_speed(40); // After driving 6 inches at DRIVE_SPEED, the robot will go the remaining distance at 40 speed
	chassis.wait_drive();
}



///
// Swing Example
///
void swing_example() {
	// The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
	// The second parameter is target degrees
	// The third parameter is speed of the moving side of the drive


	chassis.set_swing_pid(ez::LEFT_SWING, 45, SWING_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(24, DRIVE_SPEED, true);
	chassis.wait_until(12);

	chassis.set_swing_pid(ez::RIGHT_SWING, 0, SWING_SPEED);
	chassis.wait_drive();
}



///
// Auto that tests everything
///
void combining_movements() {
	chassis.set_drive_pid(24, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_swing_pid(ez::RIGHT_SWING, -45, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(0, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-24, DRIVE_SPEED, true);
	chassis.wait_drive();
}



///
// Interference example
///
void tug (int attempts) {
	for (int i=0; i<attempts-1; i++) {
		// Attempt to drive backwards
		printf("i - %i", i);
		chassis.set_drive_pid(-12, 127);
		chassis.wait_drive();

		// If failsafed...
		if (chassis.interfered) {
			chassis.reset_drive_sensor();
			chassis.set_drive_pid(-2, 20);
			pros::delay(1000);
		}
		// If robot successfully drove back, return
		else {
			return;
		}
	}
}

// If there is no interference, robot will drive forward and turn 90 degrees. 
// If interfered, robot will drive forward and then attempt to drive backwards. 
void interfered_example() {
	chassis.set_drive_pid(24, DRIVE_SPEED, true);
	chassis.wait_drive();

	if (chassis.interfered) {
		tug(3);
		return;
	}

	chassis.set_turn_pid(90, TURN_SPEED);
	chassis.wait_drive();
}



// . . .
// Make your own autonomous functions here!
// . . .