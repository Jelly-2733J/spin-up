#include "main.h"
#include <cmath>
#include <mutex>

// RobotPosition class holds the x, y, and heading values of the robot.
// There are methods to read and set all positional and orientational values.
class AutoAim {
	double goal_distance;
	pros::Mutex goal_distance_guard;
	int goal_sig;
	pros::Mutex goal_sig_guard;
public:
	// Set the distance from the goal
	void set_distance(double new_distance) {
		std::lock_guard<pros::Mutex> lock(goal_distance_guard);
        goal_distance = new_distance;
	};
	// Read the current goal distance
	double distance() {
		std::lock_guard<pros::Mutex> lock(goal_distance_guard);
        return goal_distance;
	};
	// Calculate optimal RPM
	double calculate_RPM() {
		std::lock_guard<pros::Mutex> lock(goal_distance_guard);

		// TODO: Experimentally find best fit curve for different distances
		// Graph should be RPM (dependent) vs. Distance (independent)
		return goal_distance; // Add equation here

	}
	void set_tracking_sig(int new_sig) {
		std::lock_guard<pros::Mutex> lock(goal_sig_guard);
        goal_sig = new_sig;
	}
	int tracking_sig() {
		std::lock_guard<pros::Mutex> lock(goal_sig_guard);
        return goal_sig;
	}
};

double rads(double deg) {
	return deg * 3.14159265358979323846 / 180;
}

AutoAim robot_aim;

// Goal tracking
void goalSense() {

	pros::lcd::initialize();

	// Define vision sensors
	pros::Vision vis1 (11);
	pros::Vision vis2 (12);

	// Create color signatures for blob detection
	// These are currently bs values, use vcs_vision utility
  	pros::vision_signature_s_t RED_SIG = pros::Vision::signature_from_utility(1, 8973, 11143, 10058, -2119, -1053, -1586, 5.4, 0);
	pros::vision_signature_s_t BLUE_SIG = pros::Vision::signature_from_utility(2, 8973, 11143, 10058, -2119, -1053, -1586, 5.4, 0);

	// Set signatures on the vision sensors
	vis1.set_signature(1, &RED_SIG);
	vis2.set_signature(1, &RED_SIG);
	vis1.set_signature(2, &BLUE_SIG);
	vis2.set_signature(2, &BLUE_SIG);

	// Inches between the lenses of both vision sensors
	double side_c = 16;

	// t is the last loop timestamp in milliseconds
	// This is used to ensure consistent loop intervals with pros::Task::delay_until
	uint32_t t = pros::millis();

	while (true) {

		// Get largest signature blob for each vision sensor
		pros::vision_object_s_t blob1 = vis1.get_by_sig(0, robot_aim.tracking_sig());
		pros::vision_object_s_t blob2 = vis2.get_by_sig(0, robot_aim.tracking_sig());

		if (blob1.signature == 255 || blob2.signature == 255) {
			// Set the AutoAim distance to -1.0 to signify no detection of signature
			robot_aim.set_distance(-1.0);
			continue;
		}

		// Vision Sensor FOV is 61 degrees and horizontal pixels is 316
		// Degrees per pixel
		double angle_constant = 61.0 / 316.0;

		double angle_a = (316 - blob1.x_middle_coord) * angle_constant;
		double angle_b = blob2.x_middle_coord * angle_constant;

		// Angles of a triangle sum to 180 degrees
		double angle_c = 180 - angle_a - angle_b;

		// Find the sine ratio of the triangle
		double sin_ratio = side_c / sin(rads(angle_c));

		// Solve for sides A and B using the law of sines via the calculated sine ratio
		double side_a = sin(rads(angle_a)) * sin_ratio;
		double side_b = sin(rads(angle_b)) * sin_ratio;

		// Find area of the triangle
		double area = side_a * side_b * sin(rads(angle_c)) / 2;

		// Find the height of the C side of the triangle
		// This is the plane distance
		double plane_distance = 2 * area / side_c;

		pros::lcd::print(2, "Planar: %.2f", plane_distance);

		/* ------------------------------------------------------------------------------------------
		| plane_distance is along the plane containing the sensors and the center of the goal blob. |
		| We need to project this distance to a level plane for uniform readings.					|
		------------------------------------------------------------------------------------------ */

		// Set the angle of elevation of the vision sensors
		double vision_elevation_angle_offset = 0.0;

		// Find the average y reading for the blob
		double y_average = (blob1.y_middle_coord + blob2.y_middle_coord) / 2.0;

		// Vision Sensor vertical pixels is 212
		// Offsetting 0 to be in the center allows us to calculate angle of elevation
		y_average -= 106;

		// Find angle of elevation using angle constant
		double elevation_angle = y_average * angle_constant;

		// Apply physical vision angle of elevation offset
		elevation_angle += vision_elevation_angle_offset;

		// We know that a level plane will form a right angle with the goal's height from that plane
		// We can find the goal depression angle by, once again, subtracting our other known angles from 180
		// However, as we know that the angle between the level plane and the goal's height is 90, we can simplify it to 90 - elevation angle
		double goal_depression_angle = 90 - elevation_angle;
		
		// Using all of these calculated values and the law of sines again, we can find true goal distance
		// As we know that sin(90) = 1, we can ommit the sine calculation and division as dividing by one does not affect our result
		double distance = plane_distance * sin(rads(goal_depression_angle));

		// Set the AutoAim distance to the calculated distance
		robot_aim.set_distance(distance);

		pros::lcd::print(3, "Level: %.2f in", robot_aim.distance());

		// Delay next loop until 10 ms have passed from the start of this loop
		pros::Task::delay_until(&t, 10);

	}

}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

	pros::Task vision(goalSense);
	robot_aim.set_tracking_sig(1);

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

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
void autonomous() {}

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
void opcontrol() {}
