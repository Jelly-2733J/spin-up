#include "api.h"

#pragma once

// Odometry class allows for positional and rotational tracking of the robot using 3-wheel odometry
class Odometry {
    private:
        // Encoder objects
        pros::ADIEncoder encoder_L;
        pros::ADIEncoder encoder_R;
        pros::ADIEncoder encoder_B;

        // Chassis constants
        double s_L;
        double s_R;
        double s_B;
        double d;

        // Positional variables
        double x = 0.0;
        double y = 0.0;
        double h = 0.0;
        double prev_x = 0.0;
        double prev_y = 0.0;
        double prev_h = 0.0;

        // Encoder variables
        int prev_L = 0;
        int prev_R = 0;
        int prev_B = 0;

        // Mutex
        pros::Mutex lock;

        // Convert degrees to radians
        static double rads(double deg);
        // Convert radians to degrees
        static double degs(double rad);
        // Convert ticks to inches
        double ticks_to_inches(int ticks);
        // Set the x position
        void set_x(double value);
        // Set the y position
        void set_y(double value);
        // Set the heading
        void set_h(double value);
        // Set the previous x position
        void set_prev_x(double value);
        // Set the previous y position
        void set_prev_y(double value);
        // Set the previous heading
        void set_prev_h(double value);
        // Set the previous left encoder value
        void set_prev_L(int value);
        // Set the previous right encoder value
        void set_prev_R(int value);
        // Set the previous back encoder value
        void set_prev_B(int value);
        // Get the previous x position
        double get_prev_x();
        // Get the previous y position
        double get_prev_y();
        // Get the previous heading
        double get_prev_h();
        // Get the previous left encoder value
        int get_prev_L();
        // Get the previous right encoder value
        int get_prev_R();
        // Get the previous back encoder value
        int get_prev_B();

    public:
        // Constructor
        Odometry(double distance_L, double distance_R, double distance_B, double wheel_diameter, char port_L1, char port_L2, char port_R1, char port_R2, char port_B1, char port_B2) : encoder_L(port_L1, port_L2, false), encoder_R(port_R1, port_R2, false), encoder_B(port_B1, port_B2, false) {
            // Set chassis constants
            s_L = distance_L;
            s_R = distance_R;
            s_B = distance_B;
            d = wheel_diameter;

            // Create encoder objects
            encoder_L = pros::ADIEncoder(port_L1, port_L2, false);
            encoder_R = pros::ADIEncoder(port_R1, port_R2, false);
            encoder_B = pros::ADIEncoder(port_B1, port_B2, false);
        }

        // Odometry methods

        // Get the current x position
        double get_x();
        // Get the current y position
        double get_y();
        // Get the current heading in radians
        double get_h();
        // Get the current heading in degrees
        double get_h_deg();
        // Reset the position and heading
        void reset(double set_x = 0.0, double set_y = 0.0, double set_heading = 0.0);
        // Odometry task
        void odometry();
};