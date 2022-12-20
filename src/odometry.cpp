#include "main.h"
#include <cmath>
#include <mutex>

#include "globals.hpp"
#include "odometry.hpp"

// Convert degrees to radians
double Odometry::rads(double deg) {
    return deg * PI / 180;
}
// Convert radians to degrees
double Odometry::degs(double rad) {
    return rad * 180 / PI;
}
// Convert ticks to inches
double Odometry::ticks_to_inches(int ticks) {
    // 360 ticks per revolution
    return (ticks / 360.0) * d * PI;
}
// Set the x position
void Odometry::set_x(double value) {
    x_guard.take();
    x = value;
    x_guard.give();
}
// Set the y position
void Odometry::set_y(double value) {
    y_guard.take();
    y = value;
    y_guard.give();
}
// Set the heading
void Odometry::set_h(double value) {
    if (value >= 2 * PI) {
        value -= 2 * PI;
    } else if (value < 0) {
        value += 2 * PI;
    }
    h_guard.take();
    h = value;
    h_guard.give();
}
// Set the previous x position
void Odometry::set_prev_x(double value) {
    prev_x_guard.take();
    prev_x = value;
    prev_x_guard.give();
}
// Set the previous y position
void Odometry::set_prev_y(double value) {
    prev_y_guard.take();
    prev_y = value;
    prev_y_guard.give();
}
// Set the previous left encoder value
void Odometry::set_prev_L(int value) {
    prev_L_guard.take();
    prev_L = value;
    prev_L_guard.give();
}
// Set the previous right encoder value
void Odometry::set_prev_R(int value) {
    prev_R_guard.take();
    prev_R = value;
    prev_R_guard.give();
}
// Set the previous back encoder value
void Odometry::set_prev_B(int value) {
    prev_B_guard.take();
    prev_B = value;
    prev_B_guard.give();
}
// Get the current x position
double Odometry::get_x() {
    x_guard.take();
    double to_return = x;
    x_guard.give();
    return to_return;
}
// Get the current y position
double Odometry::get_y() {
    y_guard.take();
    double to_return = y;
    y_guard.give();
    return to_return;
}
// Get the current heading
double Odometry::get_h() {
    h_guard.take();
    double to_return = h;
    h_guard.give();
    return to_return;
}
// Reset the position and heading
void Odometry::reset(double reset_x, double reset_y, double reset_heading) {
    set_x(reset_x);
    set_y(reset_y);
    set_h(rads(reset_heading));
    set_prev_x(reset_x);
    set_prev_y(reset_y);
    set_prev_L(0);
    set_prev_R(0);
    set_prev_B(0);
}
void Odometry::odometry() {
    while (true) {
        // Get the current encoder values
        int current_L = encoder_L.get_value();
        int current_R = encoder_R.get_value();
        int current_B = encoder_B.get_value();

        // Get the change in encoder values
        int delta_L = current_L - get_prev_L();
        int delta_R = current_R - get_prev_R();
        int delta_B = current_B - get_prev_B();

        // Set the previous encoder values
        set_prev_L(current_L);
        set_prev_R(current_R);
        set_prev_B(current_B);

        // Calculate and set the heading
        double heading = (ticks_to_inches(current_L) - ticks_to_inches(current_R)) / (s_L + s_R);
        set_h(heading);


        
    }
}