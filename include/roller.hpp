#include "api.h"

#pragma once

// AutoAim class allows for interfacing with the stereo vision task and calculating flywheel RPM.
// There are methods to read and set distances, receive RPM calculations, and set the target color.
class AutoRoller {
	private:
		int color;
		pros::Mutex color_guard;
		int solved = false;
		pros::Mutex solved_guard;
		// Read color from optical sensor
		int read_color();
		// Get alliance color
		int get_alliance();
	public:
		// Set alliance color
		void set_alliance(int side);
		// Check if roller is in range
		bool at_roller();
		// Solve a roller in range
		void solve_roller();
};