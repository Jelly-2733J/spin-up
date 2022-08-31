#include "api.h"
#include "pros/apix.h"

#pragma once

// AutonSelector class allows for easy and intutive auton selection at robot initialization.
class LVGLAutonSelector {
	private:
		double selected_auton;
		pros::Mutex selected_auton_guard;
		// Select an auton (0-5)
		void selectAuton(int auton_number);
	public:
		// Get the user selected auton number
		int selectedAuton();
		// Create the auton selector
        void create();
        // End the auton selector and display logo
        void end();
};