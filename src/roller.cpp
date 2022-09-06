#include "main.h"
#include <cmath>
#include <mutex>

#include "globals.hpp"
#include "roller.hpp"

// Read color from optical sensor
int AutoRoller::read_color() {
	int hue = optical.get_hue();
    if (hue > 300 || hue < 120) {
        return 1;
    }
    return 2;
}
// Set alliance color
void AutoRoller::set_alliance(int side) {
    color_guard.take();
	color = side;
    color_guard.give();
}
// Get alliance color
int AutoRoller::get_alliance() {
    color_guard.take();
    int to_return = color;
    color_guard.give();
    return to_return;
}
// Check if roller is in range
bool AutoRoller::at_roller() {
    if (optical.get_proximity() > 200) {
        solved_guard.take();
        bool to_return = solved;
        solved_guard.give();
        return !to_return;
    }
    else {
        solved_guard.take();
        solved = false;
        solved_guard.give();
        return false;
    }
    return optical.get_proximity() > 200;
}
// Solve a roller in range
void AutoRoller::solve_roller() {
    
    int last_color = read_color();
    intake = 50;

    // t is the last loop timestamp in milliseconds
	// This is used to ensure consistent loop intervals with pros::Task::delay_until
	uint32_t t = pros::millis();

    while (true) {
        if (!at_roller() || master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) != 0 || master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) != 0) {
            break;
        }
        int color = read_color();
        if (last_color != color && color != get_alliance()) {
            solved_guard.take();
            intake = 0;
            solved = true;
            solved_guard.give();
            break;
        }
        last_color = color;

        printf("%d %.2f\n", optical.get_proximity(), optical.get_hue());

        // Delay next loop until 10 ms have passed from the start of this loop
		pros::Task::delay_until(&t, 10);
    }
}