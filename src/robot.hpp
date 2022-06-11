#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef VEXV5_ROBOT_HPP
#define VEXV5_ROBOT_HPP

#include "../include/pros/misc.hpp"
#include "../include/pros/motors.hpp"

class Resettable {
public:
    virtual void stop(){};
    virtual void reset(){};
};

class Targeting {
public:
    virtual bool is_offset_within(double distance) = 0;
};

class Positioned {
public:
    virtual double get_position() = 0;
    virtual double get_target_position() = 0;
};

class DualPositioned : public Positioned {
public:
    virtual double get_left_position() = 0;
    virtual double get_right_position() = 0;
    virtual double get_left_target_position() = 0;
    virtual double get_right_target_position() = 0;
    double get_position() override;
    double get_target_position() override;
};

class Describable {
public:
    virtual std::string describe() = 0;
};

class Controller : public Resettable, public Describable {
public:
    virtual unsigned short int a_pressed() = 0;
    virtual unsigned short int b_pressed() = 0;
    virtual unsigned short int x_pressed() = 0;
    virtual unsigned short int y_pressed() = 0;

    virtual unsigned short int up_pressed() = 0;
    virtual unsigned short int down_pressed() = 0;
    virtual unsigned short int left_pressed() = 0;
    virtual unsigned short int right_pressed() = 0;

    virtual unsigned short int l1_pressed() = 0;
    virtual unsigned short int l2_pressed() = 0;
    virtual unsigned short int r1_pressed() = 0;
    virtual unsigned short int r2_pressed() = 0;

    virtual unsigned char get_digital_speed() = 0;
    virtual void set_digital_speed(unsigned char digitalSpeed) = 0;

    virtual void set_line(unsigned char line, unsigned char col, const char* str) = 0;
    virtual void clear_line(unsigned char line) = 0;

    virtual void rumble(const char* str) = 0;

    virtual double left_stick_x() = 0;
    virtual double left_stick_y() = 0;
    virtual double right_stick_x() = 0;
    virtual double right_stick_y() = 0;

    virtual double prev_left_stick_x() = 0;
    virtual double prev_left_stick_y() = 0;
    virtual double prev_right_stick_x() = 0;
    virtual double prev_right_stick_y() = 0;

    virtual void update() = 0;

    void reset() override = 0;
    void stop() override = 0;
    std::string describe() override = 0;
};

class Updatable {
public:
    virtual void update(Controller* controller){};
};

class Drivetrain : public Resettable, public Targeting, public Updatable, public Describable {
private:
    pros::Motor rightFront;
    pros::Motor leftFront;
    pros::Motor rightBack;
    pros::Motor leftBack;

public:
    Drivetrain(pros::Motor rightFront, pros::Motor leftFront, pros::Motor rightBack, pros::Motor leftBack);

    bool is_offset_within(double distance) override;
    void move_for(double right_distance, double left_distance, int max_rpm = 60, bool block = true);
    void forwards(double distance, int max_rpm = 100, bool block = true);
    void backwards(double distance, int max_rpm = 100, bool block = true);
    void turn_right(double angle, int max_rpm = 100, bool block = true);
    void turn_left(double angle, int max_rpm = 100, bool block = true);
    void move_right(int voltage);
    void move_left(int voltage);

    void update(Controller* controller) override;
    void reset() override;
    void stop() override;
    std::string describe() override;
};

class OpController : public Controller {
public:
    pros::Controller controller;

private:
    unsigned short int a = 0;
    unsigned short int b = 0;
    unsigned short int x = 0;
    unsigned short int y = 0;

    unsigned short int up = 0;
    unsigned short int down = 0;
    unsigned short int left = 0;
    unsigned short int right = 0;

    unsigned short int l1 = 0;
    unsigned short int l2 = 0;
    unsigned short int r1 = 0;
    unsigned short int r2 = 0;

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
    explicit OpController(pros::Controller controller = pros::Controller(pros::E_CONTROLLER_MASTER));

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

    double left_stick_x() override;
    double left_stick_y() override;
    double right_stick_x() override;
    double right_stick_y() override;

    double prev_left_stick_x() override;
    double prev_left_stick_y() override;
    double prev_right_stick_x() override;
    double prev_right_stick_y() override;

    unsigned char get_digital_speed() override;
    void set_digital_speed(unsigned char speed) override;

    void set_line(unsigned char line, unsigned char col, const char* str) override;
    void clear_line(unsigned char line) override;

    void rumble(const char* str) override;

    void update() override;
    void reset() override;
    void stop() override;
    std::string describe() override;
};

class Robot : public Resettable, public Describable {
public:
    Drivetrain drivetrain;
    Controller* controller;

public:
    explicit Robot(Drivetrain drivetrain);

    void forwards(double distance, int max_rpm = 100, bool block = true);
    void backwards(double distance, int max_rpm = 100, bool block = true);
    void turn_right(double angle, int max_rpm = 100, bool block = true);
    void turn_left(double angle, int max_rpm = 100, bool block = true);

    void update();
    void reset() override;
    void stop() override;
    std::string describe() override;
    ~Robot();
};

#endif//VEXV5_ROBOT_HPP

#pragma clang diagnostic pop