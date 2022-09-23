#include "EZ-Template/drive/drive.hpp"
#include "globals.hpp"
#include "main.h"


/////
// For installation, upgrading, documentation, and tutorials, check out website!
// https://ez-robotics.github.io/EZ-Template/
/////


const int DRIVE_SPEED = 120; // This is 110/127 (around 87% of max speed).  We don't suggest making this 127.
                             // If this is 127 and the robot tries to heading correct, it's only correcting by
                             // making one side slower.  When this is 87%, it's correcting by making one side
                             // faster and one side slower, giving better heading correction.
const int TURN_SPEED  = 110;
const int SWING_SPEED = 90;



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
	flywheel.set_target_RPM(2910);

	chassis.set_drive_pid(-22, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(90, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-11, DRIVE_SPEED);
	chassis.wait_drive();

	intake = 100;
	pros::delay(150);
	intake = 0;

	chassis.set_drive_pid(6, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(40, TURN_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_drive_pid(33, DRIVE_SPEED, true);
	chassis.wait_until(20);
	chassis.set_max_speed(50);
	chassis.wait_drive();

	chassis.set_turn_pid(105, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(7, DRIVE_SPEED);
	chassis.wait_drive();

	pros::delay(200);

	flywheel.shoot(3, 4000);

	pros::delay(300);

	intake = 100;

	chassis.set_drive_pid(-7, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(40, DRIVE_SPEED, true);
	chassis.wait_until(5);
	chassis.set_max_speed(50);
	chassis.wait_drive();

	chassis.set_turn_pid(130, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(5, DRIVE_SPEED);
	chassis.wait_drive();

	flywheel.set_target_RPM(2680);

	flywheel.shoot(2, 3000);

	intake = 0;
	flywheel.set_active(false);

}



///
// Left Winpoint
///
void left_winpoint() {

	flywheel.set_active(true);
	flywheel.set_target_RPM(2910);

	chassis.set_drive_pid(-2, DRIVE_SPEED);
	chassis.wait_drive();

	intake = 100;
	pros::delay(150);
	intake = 0;

	chassis.set_drive_pid(3, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-45, TURN_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_drive_pid(9, 50);
	chassis.wait_drive();

	pros::delay(250);

	chassis.set_drive_pid(-9, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-127, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-51, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-40, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(12, DRIVE_SPEED);
	chassis.wait_drive();

	pros::delay(200);

	flywheel.shoot(2, 5000);

	pros::delay(300);

	intake = 100;

	chassis.set_drive_pid(4, DRIVE_SPEED);
	chassis.wait_drive();

	pros::delay(2000);

	flywheel.shoot(2, 10000);

}



///
// Skills
///
void skills() {
	
	flywheel.set_active(true);
	flywheel.set_target_RPM(2700);

	chassis.set_drive_pid(-3, DRIVE_SPEED);
	chassis.wait_drive();

	intake = 100;
	pros::delay(270);
	intake = 0;

	chassis.set_drive_pid(3, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-45, TURN_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_drive_pid(27, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(90, TURN_SPEED);
	chassis.wait_drive();

	intake = 0;

	chassis.set_drive_pid(-11, DRIVE_SPEED);
	chassis.wait_drive();

	intake = 100;
	pros::delay(270);
	intake = 0;

	chassis.set_drive_pid(5, DRIVE_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_turn_pid(0, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(50, DRIVE_SPEED, true);
	chassis.wait_drive();

	intake = -80;
	flywheel.full_voltage(true);
	pros::delay(500);
	intake = 0;
	pros::delay(400);
	intake = -80;
	pros::delay(500);
	intake = 100;
	flywheel.full_voltage(false);

	flywheel.set_target_RPM(2600);

	chassis.set_drive_pid(-45, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(70, DRIVE_SPEED, true);
	chassis.wait_until(12);
	chassis.set_max_speed(45);
	chassis.wait_drive();

	chassis.set_drive_pid(-6, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-51, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(6, DRIVE_SPEED);
	chassis.wait_drive();

	pros::delay(300);

	flywheel.shoot(3, 3000);

	pros::delay(300);

	flywheel.set_target_RPM(2700);

	chassis.set_drive_pid(-22, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_drive_pid(32, 60);
	chassis.wait_drive();

	chassis.set_turn_pid(0, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(38, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-90, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-15, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-180, TURN_SPEED);
	chassis.wait_drive();

	intake = 0;

	chassis.set_drive_pid(-6.5, DRIVE_SPEED);
	chassis.wait_drive();

	intake = 100;
	pros::delay(270);
	intake = 0;

	// OTHER SIDE
	chassis.set_angle(0);

	chassis.set_drive_pid(3, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-45, TURN_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_drive_pid(27, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(90, TURN_SPEED);
	chassis.wait_drive();

	intake = 0;

	chassis.set_drive_pid(-10, DRIVE_SPEED);
	chassis.wait_drive();

	intake = 100;
	pros::delay(270);
	intake = 0;

	chassis.set_drive_pid(6, DRIVE_SPEED);
	chassis.wait_drive();

	intake = 100;

	chassis.set_turn_pid(0, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(51, DRIVE_SPEED, true);
	chassis.wait_drive();
	
	intake = -80;
	flywheel.full_voltage(true);
	pros::delay(500);
	intake = 0;
	pros::delay(400);
	intake = -80;
	pros::delay(500);
	intake = 100;
	flywheel.full_voltage(false);

	flywheel.set_target_RPM(2600);

	chassis.set_drive_pid(-45, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(70, DRIVE_SPEED, true);
	chassis.wait_until(12);
	chassis.set_max_speed(45);
	chassis.wait_drive();

	chassis.set_drive_pid(-6, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-49, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(7, DRIVE_SPEED);
	chassis.wait_drive();

	pros::delay(300);

	flywheel.shoot(3, 3000);

	pros::delay(300);

	flywheel.set_target_RPM(2700);

	chassis.set_drive_pid(-5, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(68, 90, true);
	chassis.wait_until(30);
	chassis.set_max_speed(25);
	chassis.wait_until(42);
	chassis.set_max_speed(45);
	chassis.wait_drive();

	chassis.set_drive_pid(-11, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-90, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(36, DRIVE_SPEED, true);
	chassis.wait_drive();

	intake = -80;
	flywheel.full_voltage(true);
	pros::delay(500);
	intake = 0;
	pros::delay(400);
	intake = -80;
	pros::delay(500);
	intake = 100;
	flywheel.full_voltage(false);

	flywheel.set_active(false);

	chassis.set_turn_pid(-82, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-57, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-135, TURN_SPEED);
	chassis.wait_drive();

	endgame.set_value(true);

	chassis.set_drive_pid(-6, DRIVE_SPEED);
	chassis.wait_drive();
	
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