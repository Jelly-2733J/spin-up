#include "main.h"
#include <cmath>
#include <mutex>

#include "globals.hpp"
#include "autoaim.hpp"

// Convert degrees to radians
double AutoAim::rads(double deg) {
	return deg * 3.14159265358979323846 / 180;
}
// Set the distance from the goal
void AutoAim::set_distance(double new_distance) {
	goal_distance_guard.take();
	goal_distance = new_distance;
	goal_distance_guard.give();
};
// Read the current goal distance
double AutoAim::distance() {
	goal_distance_guard.take();
	double to_return = goal_distance;
	goal_distance_guard.give();
	return to_return;
};
// Set the target goal color for tracking
void AutoAim::set_tracking_sig(int new_sig) {
	goal_sig_guard.take();
	goal_sig = new_sig;
	goal_sig_guard.give();
}
// Read the current target goal color
int AutoAim::tracking_sig() {
	goal_sig_guard.take();
	int to_return = goal_sig;
	goal_sig_guard.give();
	return to_return;
}
// Goal tracking
void AutoAim::goalSense() {

	// Create color signatures for blob detection
	// These are currently bs values, use vcs_vision utility
  	pros::vision_signature_s_t RED_SIG = pros::Vision::signature_from_utility(1, 3011, 4059, 3535, -4753, -3913, -4333, 1.800, 0);
	pros::vision_signature_s_t BLUE_SIG = pros::Vision::signature_from_utility(2, 8973, 11143, 10058, -2119, -1053, -1586, 5.4, 0);

	// Set signatures on the vision sensors
	vis1.set_signature(1, &RED_SIG);
	vis2.set_signature(1, &RED_SIG);
	vis1.set_signature(2, &BLUE_SIG);
	vis2.set_signature(2, &BLUE_SIG);

	// Inches between the lenses of both vision sensors
	double side_c = 12.5;

	// t is the last loop timestamp in milliseconds
	// This is used to ensure consistent loop intervals with pros::Task::delay_until
	uint32_t t = pros::millis();

	while (true) {

		// Get largest signature blob for each vision sensor
		pros::vision_object_s_t blob1 = vis1.get_by_sig(0, tracking_sig());
		pros::vision_object_s_t blob2 = vis2.get_by_sig(0, tracking_sig());

		if (blob1.signature == 255 || blob2.signature == 255) {
			// Set the AutoAim distance to -1.0 to signify no detection of signature
			set_distance(-1.0);
			continue;
		}

		// Vision Sensor FOV is 61 degrees and horizontal pixels is 316
		// Degrees per pixel
		double angle_constant = 61.0 / 316.0;

		double angle_a = 90 - (blob1.x_middle_coord - 158) * angle_constant;
		double angle_b = 90 + (blob2.x_middle_coord - 158) * angle_constant;

		// pros::lcd::print(0, "Angle A: %.1f    Angle B: %.1f", angle_a, angle_b);

		// Angles of a triangle sum to 180 degrees
		double angle_c = 180 - angle_a - angle_b;

		// Find the sine ratio of the triangle
		double sin_ratio = side_c / sin(rads(angle_c));

		// Solve for side B using the law of sines via the calculated sine ratio
		double side_b = sin(rads(angle_b)) * sin_ratio;

		// Use law of cosines to find distance from midpoint of vision bar to goal
		// distance = sqrt(left_side^2 + (base_side / 2)^2 − 2 * left_size * (base_side / 2) * cos(left_angle))
		double plane_distance = pow(side_b, 2) + pow(side_c / 2, 2) - 2 * side_b * (side_c / 2) * cos(rads(angle_a));

		// pros::lcd::print(2, "Planar: %.2f", plane_distance);

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
		y_average = -(y_average - 106);

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
		double calculated_distance = plane_distance * sin(rads(goal_depression_angle));

		// Set the AutoAim distance to the calculated distance
		set_distance(calculated_distance);

		// pros::lcd::print(3, "Level: %.2f in", distance());

		// Delay next loop until 10 ms have passed from the start of this loop
		pros::Task::delay_until(&t, 10);

	}

}