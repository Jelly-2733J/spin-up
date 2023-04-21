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
	// Do nothing
}

///
// Right Max Discs
///
void right_max() {

	flywheel.set_active(true);
	flywheel.set_target_RPM(3070);

	chassis.set_drive_pid(-16, DRIVE_SPEED, true);
	chassis.wait_drive();

    chassis.set_turn_pid(90, DRIVE_SPEED);
	chassis.wait_drive();

	get_roller(-10, 200);

	chassis.set_drive_pid(7, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(97, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(9, DRIVE_SPEED, true);
	chassis.wait_until(2);
	actuated_intake.set_value(true);
	chassis.wait_drive();

	flywheel.dumbshoot(3, 3, 400, 400);

	intake = 127;

	chassis.set_drive_pid(4, DRIVE_SPEED);
	chassis.wait_until(3);
	actuated_intake.set_value(false);
	chassis.wait_drive();

	pros::delay(2000);

	flywheel.dumbshoot(3, 3, 400, 400);

	flywheel.set_target_RPM(2950);

	chassis.set_drive_pid(-11, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	chassis.set_drive_pid(60, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_drive_pid(-20, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_turn_pid(115, TURN_SPEED);
	chassis.wait_drive();

	flywheel.dumbshoot(1, 3, 0, 0);

	chassis.set_drive_pid(10, DRIVE_SPEED);
	chassis.wait_drive();

	flywheel.dumbshoot(3, 3, 400, 400);

}

///
// Right Safe
///
void right_safe() {

}

///
// Right Rush
///
void right_rush() {

	flywheel.set_active(true);
	flywheel.set_control_mode(2);
	flywheel.set_target_RPM(3050);

	intake = 127;

	chassis.set_drive_pid(2733, 127, true);
	chassis.wait_until(18.5);
	chassis.set_drive_pid(-8, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(-90, TURN_SPEED);
	chassis.wait_drive();

	get_roller(-22, 200);

	chassis.set_drive_pid(21, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-31, TURN_SPEED);
	chassis.wait_drive();

	flywheel.dumbshoot(3, 3, 500, 500);

	chassis.set_turn_pid(-90, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(44, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_drive_pid(-12, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-12, TURN_SPEED);
	chassis.wait_drive();

	flywheel.dumbshoot(1, 3, 0, 0);

	intake = 127;

	chassis.set_drive_pid(10, DRIVE_SPEED);

	pros::delay(500);

	flywheel.dumbshoot(3, 3, 500, 500);
}

///
// Left Max Discs
///
void left_max() {

	flywheel.set_active(true);
	flywheel.set_target_RPM(3100);

	get_roller(-2, 250);

	chassis.set_drive_pid(4, DRIVE_SPEED);

	chassis.set_swing_pid(ez::RIGHT_SWING, -60, 70);
	chassis.wait_drive();

	intake = 127;

	chassis.set_drive_pid(8, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-5, DRIVE_SPEED);
	chassis.wait_drive();

	chassis.set_swing_pid(ez::RIGHT_SWING, -9, 70);
	chassis.wait_drive();

	chassis.set_drive_pid(10, DRIVE_SPEED, true);
	chassis.wait_until(2);
	actuated_intake.set_value(true);
	chassis.wait_until(10);
	chassis.set_max_speed(40);
	chassis.wait_drive();

	flywheel.dumbshoot(3, 3, 400, 400);

	intake = 127;

	chassis.set_drive_pid(4, DRIVE_SPEED);
	chassis.wait_until(3);
	actuated_intake.set_value(false);
	chassis.wait_drive();

	pros::delay(2000);

	flywheel.dumbshoot(3, 3, 400, 400);

	flywheel.set_target_RPM(3000);

	chassis.set_swing_pid(ez::RIGHT_SWING, 54, SWING_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(23, DRIVE_SPEED, true);
	chassis.wait_until(2);
	actuated_intake.set_value(true);
	intake = 127;
	chassis.wait_until(22);
	actuated_intake.set_value(false);
	intake = 127;
	chassis.wait_drive();

	pros::delay(750);

	chassis.set_turn_pid(-20, TURN_SPEED);
	chassis.wait_drive();

	flywheel.dumbshoot(1, 3, 0, 0);

	intake = 127;

	chassis.set_drive_pid(10, DRIVE_SPEED);
	chassis.wait_drive();

	pros::delay(1000);

	flywheel.dumbshoot(3, 3, 400, 400);

}

///
// Left Safe
///
void left_safe() {

}

///
// Left Rush
///
void left_rush() {

}

///
// Solo Winpoint
///
void solo_wp() {
	flywheel.set_active(true);
	flywheel.set_control_mode(2);
	flywheel.set_target_RPM(3150);

	get_roller(-2, 200);

	chassis.set_drive_pid(8, DRIVE_SPEED);
	chassis.wait_until(2);
	intake = 127;
	chassis.wait_drive();

	chassis.set_turn_pid(-11, SWING_SPEED);
	chassis.wait_drive();

	flywheel.wait_for_target_RPM(500);

	flywheel.dumbshoot(3, 3, 450, 450);

	flywheel.set_target_RPM(3000);

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(40, 80, true);
	chassis.wait_until(2);
	intake = 127;
	actuated_intake.set_value(true);
	chassis.wait_until(15);
	actuated_intake.set_value(false);
	chassis.wait_drive();

	chassis.set_turn_pid(-30, TURN_SPEED);
	chassis.wait_drive();

	flywheel.dumbshoot(3, 3, 400, 400);

	flywheel.set_target_RPM(3150);

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	chassis.set_drive_pid(69, DRIVE_SPEED, true);
	chassis.wait_drive();

	chassis.set_swing_pid(ez::RIGHT_SWING, -90, SWING_SPEED);
	chassis.wait_drive();

	get_roller(-16, 200);

	chassis.set_swing_pid(ez::LEFT_SWING, -81, SWING_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(10, DRIVE_SPEED);
	chassis.wait_drive();

	flywheel.dumbshoot(3, 3, 450, 450);

}

void ripple() {
	intake = -110;
	flywheel.set_control_mode(2);
	pros::delay(850);
	flywheel.set_control_mode(0);
	intake = 0;
}

void triple_tap() {
	intake = -80;
	flywheel.set_control_mode(2);
	pros::delay(300);
	intake = 127;
	pros::delay(350);
	intake = -80;
	pros::delay(500);
	flywheel.set_control_mode(0);
	intake = 0;
}

///
// Skills
///
void auton_skills() {
	
	flywheel.set_active(true);
	blooper.set_value(true);
	flywheel.set_target_RPM(2450);

	get_roller(-3, 300);

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

	get_roller(-11, 300);

	chassis.set_drive_pid(7, 127);
	chassis.wait_drive();

	intake = 127;

	chassis.set_turn_pid(2, TURN_SPEED);
	chassis.wait_drive();

	intake = 0;

	chassis.set_drive_pid(43, 127, true);
	chassis.wait_drive();

	// Stack 1
	ripple();

	flywheel.set_target_RPM(2460);
	
	chassis.set_drive_pid(-38, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	// Stack 2
	chassis.set_drive_pid(61, 100, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-45, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(7, 127);
	chassis.wait_drive();

	ripple();

	chassis.set_turn_pid(-80, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	// Stack 3
	chassis.set_drive_pid(35, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-90, TURN_SPEED);

	chassis.set_drive_pid(-36, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-45, TURN_SPEED);
	chassis.wait_drive();

	ripple();

	flywheel.set_target_RPM(2350);

	chassis.set_turn_pid(-10, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	// Stack 4
	chassis.set_drive_pid(35, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(0, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(7, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(-96, TURN_SPEED);
	chassis.wait_drive();

	ripple();

	chassis.set_turn_pid(-231, TURN_SPEED);
	chassis.wait_drive();

	// Stack 5
	chassis.set_drive_pid(29, 127, true);
	chassis.wait_until(2);
	intake = 127;
	actuated_intake.set_value(true);
	chassis.wait_until(25);
	chassis.set_max_speed(50);
	chassis.wait_until(28);
	actuated_intake.set_value(false);
	chassis.wait_drive();

	pros::delay(750);

	chassis.set_drive_pid(-31, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-96, TURN_SPEED);
	chassis.wait_drive();

	ripple();

	flywheel.set_target_RPM(2450);
	
	chassis.set_turn_pid(-90, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-53, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-180, TURN_SPEED);
	chassis.wait_drive();

	get_roller(-6, 300);

	chassis.reset_gyro(0);

	chassis.set_turn_pid(0, TURN_SPEED);
	chassis.wait_drive();

	// Stack 6
	chassis.set_drive_pid(29, 127);
	chassis.wait_until(2);
	actuated_intake.set_value(true);
	chassis.wait_until(4);
	intake = 127;
	chassis.wait_until(10);
	chassis.set_max_speed(50);
	chassis.wait_until(17);
	actuated_intake.set_value(false);
	chassis.wait_drive();

	pros::delay(250);

	chassis.set_turn_pid(90, TURN_SPEED);
	chassis.wait_drive();

	get_roller(-29, 300);

	chassis.set_drive_pid(7, 127);
	chassis.wait_drive();

	intake = 127;

	chassis.set_turn_pid(2, TURN_SPEED);
	chassis.wait_drive();

	intake = 0;

	chassis.set_drive_pid(41, 127, true);
	chassis.wait_drive();

	ripple();

	flywheel.set_target_RPM(2460);
	
	chassis.set_drive_pid(-38, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(45, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	// Stack 7
	chassis.set_drive_pid(61, 100, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-45, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(7, 127);
	chassis.wait_drive();

	ripple();

	chassis.set_turn_pid(-80, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	// Stack 8
	chassis.set_drive_pid(35, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-90, TURN_SPEED);

	chassis.set_drive_pid(-36, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-45, TURN_SPEED);
	chassis.wait_drive();

	ripple();

	flywheel.set_target_RPM(2350);

	chassis.set_turn_pid(-10, TURN_SPEED);
	chassis.wait_drive();

	intake = 127;

	// Stack 9
	chassis.set_drive_pid(35, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(0, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(7, 127);
	chassis.wait_drive();

	chassis.set_turn_pid(-99, TURN_SPEED);
	chassis.wait_drive();

	ripple();

	chassis.set_turn_pid(-231, TURN_SPEED);
	chassis.wait_drive();

	// Stack 10
	chassis.set_drive_pid(29, 127, true);
	chassis.wait_until(2);
	intake = 127;
	actuated_intake.set_value(true);
	chassis.wait_until(25);
	chassis.set_max_speed(50);
	chassis.wait_until(28);
	actuated_intake.set_value(false);
	chassis.wait_drive();

	pros::delay(750);

	chassis.set_drive_pid(-31, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-99, TURN_SPEED);
	chassis.wait_drive();

	ripple();

	chassis.set_turn_pid(-75, TURN_SPEED);
	chassis.wait_drive();

	chassis.set_drive_pid(-67, 127, true);
	chassis.wait_drive();

	chassis.set_turn_pid(-135, TURN_SPEED);
	chassis.wait_drive();

	endgame();

}