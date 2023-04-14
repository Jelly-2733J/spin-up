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

void get_roller(double distance, int time) {
	intake = 0;
	chassis.set_drive_pid(distance, DRIVE_SPEED);
	chassis.wait_drive();
	intake = 127;	 
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
	flywheel.set_target_RPM(2850);

	chassis.set_drive_pid(-5, DRIVE_SPEED, true);
	chassis.wait_drive();

    chassis.set_swing_pid(ez::RIGHT_SWING, 92, DRIVE_SPEED);
	chassis.wait_drive();

	get_roller(-4, 300);

	chassis.set_drive_pid(9, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(40, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	chassis.set_drive_pid(21, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(112, TURN_SPEED);
	chassis.wait_drive();

	flywheel.dumbshoot(4, 3);

	flywheel.set_target_RPM(2750);

	intake = -127;

	pros::delay(400);

	intake = 127;

	chassis.set_drive_pid(9, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-8, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(34, 127, true);
	chassis.wait_drive();

	chassis.set_drive_pid(-12, 127, true);
	chassis.wait_drive();

	chassis.set_drive_pid(6, 127);

	chassis.set_turn_pid(124, TURN_SPEED);
	chassis.wait_drive();

	intake = 0;

	flywheel.dumbshoot(4, 3);

	intake = -127;
}



///
// Left Winpoint
///
void left_winpoint() {

	flywheel.set_active(true);
	flywheel.full_voltage(true);
	flywheel.set_target_RPM(3050);

	get_roller(-2, 200);

	chassis.set_swing_pid(ez::RIGHT_SWING, -10, SWING_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(17, DRIVE_SPEED, true);
	chassis.wait_until(2);
	actuated_intake.set_value(true);
	chassis.wait_drive();
	
	pros::delay(1000);
	flywheel.dumbshoot(2, 2, 650, 900);

	actuated_intake.set_value(false);
	intake = 127;

	pros::delay(2000);

	flywheel.dumbshoot(3, 3, 950, 700);

	flywheel.set_target_RPM(3000);

	chassis.set_swing_pid(ez::RIGHT_SWING, 54, SWING_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(23, DRIVE_SPEED, true);
	chassis.wait_until(2);
	actuated_intake.set_value(true);
	intake = 127;
	chassis.wait_drive();

	actuated_intake.set_value(false);
	intake = 127;

	pros::delay(750);

	chassis.set_drive_pid(-4, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(-19, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(6, DRIVE_SPEED);
	chassis.wait_drive();

	flywheel.dumbshoot(3, 3, 950, 700);

	chassis.set_drive_pid(-20, 127, true);
	chassis.wait_drive();

}

///
// Solo Winpoint
///
void solo_winpoint() {
	flywheel.set_active(true);
	flywheel.set_target_RPM(3050);

	get_roller(-2, 200);

	chassis.set_drive_pid(4, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_swing_pid(ez::LEFT_SWING, 45, SWING_SPEED);
	chassis.wait_drive();

	intake = 127;

	chassis.set_drive_pid(40, 60, true);
	chassis.wait_until(2);
	actuated_intake.set_value(true);
	chassis.wait_until(14);
	actuated_intake.set_value(false);
	chassis.wait_drive();

	chassis.set_turn_pid(-33, TURN_SPEED);
	chassis.wait_drive();

	flywheel.dumbshoot(3, 3, 950, 700);

	flywheel.set_target_RPM(3170);

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	chassis.set_drive_pid(67, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_swing_pid(ez::RIGHT_SWING, -90, SWING_SPEED);
	chassis.wait_drive();

	get_roller(-16, 200);

	chassis.set_swing_pid(ez::LEFT_SWING, -80, SWING_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(10, DRIVE_SPEED);
	chassis.wait_drive();

	flywheel.dumbshoot(3, 3, 1100, 1100);

	chassis.set_swing_pid(ez::RIGHT_SWING, 0, 70);
	chassis.wait_drive();

	chassis.set_drive_pid(-43, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-108, TURN_SPEED);
	chassis.wait_drive();

}

void ripple() {
	intake = -127;
	flywheel.full_voltage(true);
	pros::delay(750);
	flywheel.full_voltage(false);
	intake = 0;
}

void triple_tap() {
	intake = -80;
	flywheel.full_voltage(true);
	pros::delay(300);
	intake = 127;
	pros::delay(350);
	intake = -80;
	pros::delay(500);
	flywheel.full_voltage(false);
	intake = 0;
}

///
// Nerfed Skills
///
void nerfed_skills() {
	
	flywheel.set_active(true);
	flywheel.set_target_RPM(2350);

	get_roller(-3, 500);

	chassis.set_drive_pid(4, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(-30, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	chassis.set_drive_pid(27, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(90, TURN_SPEED);
	chassis.wait_drive();

	intake = 0;

	get_roller(-11, 500);

	chassis.set_drive_pid(7, 127);//7 on portable
	chassis.wait_drive();

	intake = 127;

	chassis.set_turn_pid(1, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(43, 127, true); //43 on portable
	chassis.wait_drive();

	// Stack 1
	ripple();

	flywheel.set_target_RPM(2350);
	
	chassis.set_drive_pid(-38, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	// Stack 2
	chassis.set_drive_pid(67, 127, true);
	chassis.wait_until(20);
	chassis.set_max_speed(50);
	chassis.wait_drive();

	chassis.set_drive_pid(-3, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(-48, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(3, 127);
	chassis.wait_drive();

	ripple();

	flywheel.set_target_RPM(2350);

	chassis.set_drive_pid(-4, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	// Stack 3
	chassis.set_drive_pid(55, 127, true);
	chassis.wait_until(10);
	chassis.set_max_speed(35);
	chassis.wait_drive();

	chassis.set_turn_pid(-88, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(34, 127, true);
	chassis.wait_drive();

	ripple();

	chassis.set_drive_pid(-30, 127, true); //-27 on traditional
	chassis.wait_drive();

	chassis.set_swing_pid(ez::LEFT_SWING, -180, SWING_SPEED);
	chassis.wait_drive();

	get_roller(-8, 500);

	chassis.set_drive_pid(14, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(-180, TURN_SPEED);
	chassis.wait_drive();

	chassis.reset_gyro(0);

	// OTHER SIDE

	chassis.set_turn_pid(-20, TURN_SPEED);
	chassis.wait_drive();

	intake = -127;

	// Stack 4
	chassis.set_drive_pid(52, 40, true);
	chassis.wait_drive();

	chassis.set_turn_pid(0, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(6, 127);
	chassis.wait_drive();

	ripple();

	flywheel.set_target_RPM(2350);

	chassis.set_drive_pid(-38, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(90, TURN_SPEED);
	chassis.wait_drive();

	get_roller(-11, 500);

	chassis.set_drive_pid(5, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = -127;

	// Stack 5
	chassis.set_drive_pid(67, 127, true);
	chassis.wait_until(20);
	chassis.set_max_speed(50);
	chassis.wait_drive();

	chassis.set_drive_pid(-4, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(-44, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(3, 127);
	chassis.wait_drive();

	ripple();

	flywheel.set_target_RPM(2330);

	chassis.set_drive_pid(-3, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = -127;

	// Stack 6
	chassis.set_drive_pid(55, 127, true);
	chassis.wait_until(10);
	chassis.set_max_speed(35);
	chassis.wait_drive();

	chassis.set_turn_pid(-88, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(32, 127, true);
	chassis.wait_drive();

	ripple();

	chassis.set_turn_pid(-93, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-36, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-207, TURN_SPEED);
	chassis.wait_drive();

	intake = -127;

	// Stack 7
	chassis.set_drive_pid(63, 35, true);
	chassis.wait_until(32);
	chassis.set_max_speed(70);
	chassis.wait_drive();

	chassis.set_turn_pid(-173, TURN_SPEED);
	chassis.wait_drive();

	ripple();

	intake = -127;

	chassis.set_turn_pid(-188, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-64, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(-135, TURN_SPEED);
	chassis.wait_drive();

	endgame.set_value(true);
}

///
// Skills
///
void auton_skills() {
	
	flywheel.set_active(true);
	flywheel.set_target_RPM(2350);

	get_roller(-3, 450);

	chassis.set_drive_pid(4, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(-30, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	chassis.set_drive_pid(27, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(90, TURN_SPEED);
	chassis.wait_drive();

	intake = 0;

	get_roller(-11, 450);

	chassis.set_drive_pid(7, 127);//7 on portable
	chassis.wait_drive();

	intake = 127;

	chassis.set_turn_pid(1, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(43, 127, true); //43 on portable
	chassis.wait_drive();

	// Stack 1
	ripple();

	flywheel.set_target_RPM(2350);
	
	chassis.set_drive_pid(-38, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	// Stack 2
	chassis.set_drive_pid(67, 127, true);
	chassis.wait_until(20);
	chassis.set_max_speed(50);
	chassis.wait_drive();

	chassis.set_drive_pid(-3, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(-48, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(3, 127);
	chassis.wait_drive();

	ripple();

	flywheel.set_target_RPM(2350);

	chassis.set_drive_pid(-4, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	// Stack 3
	chassis.set_drive_pid(55, 127, true);
	chassis.wait_until(10);
	chassis.set_max_speed(35);
	chassis.wait_drive();

	chassis.set_turn_pid(-88, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(34, 127, true);
	chassis.wait_drive();

	ripple();

	chassis.set_drive_pid(-30, 127, true); //-27 on traditional
	chassis.wait_drive();

	chassis.set_swing_pid(ez::LEFT_SWING, -180, SWING_SPEED);
	chassis.wait_drive();

	get_roller(-8, 450);

	chassis.set_drive_pid(14, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(-180, TURN_SPEED);
	chassis.wait_drive();

	chassis.reset_gyro(0);

	// OTHER SIDE

	chassis.set_turn_pid(-20, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	// Stack 4
	chassis.set_drive_pid(52, 40, true);
	chassis.wait_drive();

	chassis.set_turn_pid(0, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(6, 127);
	chassis.wait_drive();

	ripple();

	flywheel.set_target_RPM(2350);

	chassis.set_drive_pid(-38, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(90, TURN_SPEED);
	chassis.wait_drive();

	get_roller(-11, 450);

	chassis.set_drive_pid(5, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	// Stack 5
	chassis.set_drive_pid(67, 127, true);
	chassis.wait_until(20);
	chassis.set_max_speed(50);
	chassis.wait_drive();

	chassis.set_drive_pid(-4, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(-44, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(3, 127);
	chassis.wait_drive();

	ripple();

	flywheel.set_target_RPM(2330);

	chassis.set_drive_pid(-3, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	// Stack 6
	chassis.set_drive_pid(55, 127, true);
	chassis.wait_until(10);
	chassis.set_max_speed(35);
	chassis.wait_drive();

	chassis.set_turn_pid(-88, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(32, 127, true);
	chassis.wait_drive();

	ripple();

	chassis.set_turn_pid(-93, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-36, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-207, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	// Stack 7
	chassis.set_drive_pid(63, 35, true);
	chassis.wait_until(32);
	chassis.set_max_speed(70);
	chassis.wait_drive();

	chassis.set_turn_pid(-173, TURN_SPEED);
	chassis.wait_drive();

	ripple();

	intake = 127;

	chassis.set_swing_pid(ez::LEFT_SWING, -95, SWING_SPEED);
	chassis.wait_drive();

	// Stack 8
	chassis.set_drive_pid(34, 40);
	chassis.wait_drive();

	chassis.set_turn_pid(-90, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-27, 127);
	chassis.wait_drive();

	chassis.set_swing_pid(ez::LEFT_SWING, -175, SWING_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(4, 127);
	chassis.wait_drive();

	ripple();

	chassis.set_turn_pid(-188, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-64, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(-135, TURN_SPEED);
	chassis.wait_drive();

	endgame.set_value(true);
}

void jerk_test() {

	intake = 127;

	chassis.set_drive_pid(28, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_drive_pid(2733, 70, true);
	chassis.wait_until(39);
	chassis.set_drive_pid(-2733, 127);
	chassis.wait_until(-4);
	intake = -127;
	chassis.wait_until(-8);
	intake = 127;
	chassis.set_drive_pid(4, 127);
	chassis.wait_drive();

}

void three_stack() {
	intake = 127;

	chassis.set_drive_pid(61, 90, true);
	chassis.wait_until(10);
	chassis.set_max_speed(50);
	chassis.wait_drive();

	chassis.set_drive_pid(-2733, 127);
	chassis.wait_until(-10);
	chassis.set_drive_pid(4, 127);
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