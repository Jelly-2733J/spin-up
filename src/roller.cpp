#include "main.h"
#include <cmath>
#include <mutex>

#include "ports.hpp"
#include "roller.hpp"

// Read color from optical sensor
int AutoRoller::read_color() {
	int hue = optical.get_hue();
    if (hue > 300 || hue < 120) {
        return 1;
    }
    return 2;
}
// Move motors
void AutoRoller::move_roller(int speed) {
    ptol.move_velocity(speed);
    ptor.move_velocity(speed);
}
// Set alliance color
void AutoRoller::set_alliance(int side) {
    std::lock_guard<pros::Mutex> lock(color_guard);
	color = side;
}
// Get alliance color
int AutoRoller::get_alliance() {
    std::lock_guard<pros::Mutex> lock(color_guard);
	return color;
}
// Check if roller is in range
bool AutoRoller::at_roller() {
    return optical.get_proximity() > 200;
}
// Solve a roller in range
void AutoRoller::solve_roller() {
    
    int last_color = read_color();
    move_roller(100);

    // t is the last loop timestamp in milliseconds
	// This is used to ensure consistent loop intervals with pros::Task::delay_until
	uint32_t t = pros::millis();

    while (true) {
        if (!at_roller() || master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) != 0 || master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) != 0) {
            break;
        }
        int color = read_color();
        if (last_color != color && color != get_alliance()) {
            move_roller(0);
            break;
        }
        last_color = color;
        // Delay next loop until 10 ms have passed from the start of this loop
		pros::Task::delay_until(&t, 500);
    }
}