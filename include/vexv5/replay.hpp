#ifndef REPLAY_HPP
#define REPLAY_HPP

#include <fstream>
#include "vexv5/robot.hpp"

class ReplayController: public Controller {
    std::ifstream inf;
    bool a = false, b = false, x = false, y = false, up = false, down = false, left = false, right = false, l1 = false, l2 = false, r1 = false, r2 = false;
    double lx = 0.0, ly = 0.0, rx = 0.0, ry = 0.0;

    public:
        ReplayController();

        bool a_pressed();
        bool b_pressed();
        bool x_pressed();
        bool y_pressed();

        bool up_pressed();
        bool down_pressed();
        bool left_pressed();
        bool right_pressed();

        bool l1_pressed();
        bool l2_pressed();
        bool r1_pressed();
        bool r2_pressed();

        unsigned short int a_press_length();
        unsigned short int b_press_length();
        unsigned short int x_press_length();
        unsigned short int y_press_length();

        unsigned short int up_press_length();
        unsigned short int down_press_length();
        unsigned short int left_press_length();
        unsigned short int right_press_length();

        unsigned short int l1_press_length();
        unsigned short int l2_press_length();
        unsigned short int r1_press_length();
        unsigned short int r2_press_length();

        unsigned char get_digital_speed();
        void set_digital_speed(unsigned char digitalSpeed);

        void set_line(unsigned char line, unsigned char col, const char* str);
        void clear_line(unsigned char line);

        void rumble(const char* str);

        double left_stick_x();
        double left_stick_y();
        double right_stick_x();
        double right_stick_y();

        double prev_left_stick_x();
        double prev_left_stick_y();
        double prev_right_stick_x();
        double prev_right_stick_y();

        void update();

        void reset();
        void stop();
};

/**
 * Replays the match stored in `/usd/record.v5r`
 */
void replay_match();

#endif
