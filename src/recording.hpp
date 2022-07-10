#ifndef VEXV5_RECORDING_HPP
#define VEXV5_RECORDING_HPP

#include "robot.hpp"
#include <fstream>

class RecordingController : public Controller {
    std::ofstream outf;
    pros::Controller controller;
    unsigned short int a = false;
    unsigned short int b = false;
    unsigned short int x = false;
    unsigned short int y = false;
    unsigned short int up = false;
    unsigned short int down = false;
    unsigned short int left = false;
    unsigned short int right = false;
    unsigned short int l1 = false;
    unsigned short int l2 = false;
    unsigned short int r1 = false;
    unsigned short int r2 = false;
    double leftStickX = 0.0;
    double leftStickY = 0.0;
    double rightStickX = 0.0;
    double rightStickY = 0.0;
    double prevLeftStickX = 0.0;
    double prevLeftStickY = 0.0;
    double prevRightStickX = 0.0;
    double prevRightStickY = 0.0;
    unsigned char digitalSpeed = 127;

public:
    explicit RecordingController(pros::Controller controller = pros::Controller(pros::E_CONTROLLER_MASTER), const char* filename = "record");

    unsigned short int a_pressed() override;
    unsigned short int b_pressed() override;
    unsigned short int x_pressed() override;
    unsigned short int y_pressed() override;

    unsigned short int up_pressed() override;
    unsigned short int down_pressed() override;
    unsigned short int left_pressed() override;
    unsigned short int right_pressed() override;

    unsigned short int l1_pressed() override;
    unsigned short int l2_pressed() override;
    unsigned short int r1_pressed() override;
    unsigned short int r2_pressed() override;

    unsigned char get_digital_speed() override;
    void set_digital_speed(unsigned char digitalSpeed) override;

    void set_line(unsigned char line, unsigned char col, const char* str) override;
    void clear_line(unsigned char line) override;

    void rumble(const char* str) override;

    double left_stick_x() override;
    double left_stick_y() override;
    double right_stick_x() override;
    double right_stick_y() override;

    double prev_left_stick_x() override;
    double prev_left_stick_y() override;
    double prev_right_stick_x() override;
    double prev_right_stick_y() override;

    void update() override;

    void reset() override;
    void stop() override;
};

#endif//VEXV5_RECORDING_HPP
