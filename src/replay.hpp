#ifndef VEXV5_REPLAY_HPP
#define VEXV5_REPLAY_HPP

#include "robot.hpp"
#include <fstream>

class ReplayController : public Controller {
    std::ifstream inf;
    char type = 0;
    uint16_t a = false;
    uint16_t b = false;
    uint16_t x = false;
    uint16_t y = false;
    uint16_t up = false;
    uint16_t down = false;
    uint16_t left = false;
    uint16_t right = false;
    uint16_t l1 = false;
    uint16_t l2 = false;
    uint16_t r1 = false;
    uint16_t r2 = false;
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
    explicit ReplayController(const char* filename = "/usd/record.v5r");

    uint16_t a_pressed() override;
    uint16_t b_pressed() override;
    uint16_t x_pressed() override;
    uint16_t y_pressed() override;

    uint16_t up_pressed() override;
    uint16_t down_pressed() override;
    uint16_t left_pressed() override;
    uint16_t right_pressed() override;

    uint16_t l1_pressed() override;
    uint16_t l2_pressed() override;
    uint16_t r1_pressed() override;
    uint16_t r2_pressed() override;

    unsigned char digital_speed() override;
    void digital_speed(unsigned char digitalSpeed) override;

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

#endif//VEXV5_REPLAY_HPP
