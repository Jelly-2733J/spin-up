#include "main.h"

#pragma once

// AutoAim class allows for interfacing with the stereo vision task and calculating flywheel RPM.
// There are methods to read and set distances, receive RPM calculations, and set the target color.
class AutoAim {
	private:
        double goal_distance;
        pros::Mutex goal_distance_guard;
        int goal_sig;
        pros::Mutex goal_sig_guard;
        // Convert degrees to radians
        static double rads(double deg);
        // Set the distance from the goal
		void set_distance(double new_distance);
	public:
		// Read the current goal distance
		double distance();
		// Set the target goal color for tracking
		void set_tracking_sig(int new_sig);
		// Read the current target goal color
		int tracking_sig();
        // GoalSense task
        void goalSense();
};