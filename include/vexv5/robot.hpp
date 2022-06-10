#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "pros/motors.hpp"
#include "pros/misc.hpp"

class Resettable {
    public:
        virtual void stop();
        virtual void reset();
};

class Targeting {
    public:
        virtual bool is_offset_within(double distance);
};

class Positioned {
    public:
        virtual double get_position();
        virtual double get_target_position();
};

class Controller: public Resettable {
    public:
        virtual bool a_pressed();
        virtual bool b_pressed();
        virtual bool x_pressed();
        virtual bool y_pressed();

        virtual bool up_pressed();
        virtual bool down_pressed();
        virtual bool left_pressed();
        virtual bool right_pressed();

        virtual bool l1_pressed();
        virtual bool l2_pressed();
        virtual bool r1_pressed();
        virtual bool r2_pressed();

        virtual unsigned short int a_press_length();
        virtual unsigned short int b_press_length();
        virtual unsigned short int x_press_length();
        virtual unsigned short int y_press_length();

        virtual unsigned short int up_press_length();
        virtual unsigned short int down_press_length();
        virtual unsigned short int left_press_length();
        virtual unsigned short int right_press_length();

        virtual unsigned short int l1_press_length();
        virtual unsigned short int l2_press_length();
        virtual unsigned short int r1_press_length();
        virtual unsigned short int r2_press_length();

        virtual unsigned char get_digital_speed();
        virtual void set_digital_speed(unsigned char digitalSpeed);

        virtual void set_line(unsigned char line, unsigned char col, const char* str);
        virtual void clear_line(unsigned char line);

        virtual void rumble(const char* str);

        virtual double left_stick_x();
        virtual double left_stick_y();
        virtual double right_stick_x();
        virtual double right_stick_y();

        virtual double prev_left_stick_x();
        virtual double prev_left_stick_y();
        virtual double prev_right_stick_x();
        virtual double prev_right_stick_y();

        virtual void update();

        virtual void reset();
        virtual void stop();
};

class Updatable {
    public:
        virtual void update(Controller controller);
};

class DualPositioned: public Positioned {
    public:
        virtual double get_left_position();
        virtual double get_right_position();
        virtual double get_left_target_position();
        virtual double get_right_target_position();
        double get_position();
        double get_target_position();
};

class Drivetrain: public Resettable, public Targeting, public Updatable {
    private:
        pros::Motor rightFront;
        pros::Motor leftFront;
        pros::Motor rightBack;
        pros::Motor leftBack;

    public:
        Drivetrain(pros::Motor rightFront, pros::Motor leftFront, pros::Motor rightBack, pros::Motor leftBack);

        bool is_offset_within(double distance);
        void move_for(double right_distance, double left_distance, int max_rpm = 60, bool block = true);
        void forwards(double distance, int max_rpm = 100, bool block = true);
        void backwards(double distance, int max_rpm = 100, bool block = true);
        void turn_right(double angle, int max_rpm = 100, bool block = true);
        void turn_left(double angle, int max_rpm = 100, bool block = true);
        void move_right(int voltage);
        void move_left(int voltage);

        void update(Controller controller);
        void reset();
        void stop();
};

class OpController: public Resettable, public Controller {
    public:
        pros::Controller controller;
    private:
        unsigned short int a;
        unsigned short int b;
        unsigned short int x;
        unsigned short int y;

        unsigned short int up;
        unsigned short int down;
        unsigned short int left;
        unsigned short int right;

        unsigned short int l1;
        unsigned short int l2;
        unsigned short int r1;
        unsigned short int r2;

        double leftStickX;
        double leftStickY;
        double rightStickX;
        double rightStickY;

        double prevLeftStickX;
        double prevLeftStickY;
        double prevRightStickX;
        double prevRightStickY;

        unsigned char digitalSpeed;
    
    public:
        OpController(pros::Controller controller);

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

        double left_stick_x();
        double left_stick_y();
        double right_stick_x();
        double right_stick_y();

        double prev_left_stick_x();
        double prev_left_stick_y();
        double prev_right_stick_x();
        double prev_right_stick_y();

        unsigned char get_digital_speed();
        void set_digital_speed(unsigned char digitalSpeed);

        void set_line(unsigned char line, unsigned char col, const char* str);
        void clear_line(unsigned char line);

        void rumble(const char* str);

        void update();
        void reset();
        void stop();
};

class Robot: public Resettable {
    public:
        Drivetrain drivetrain;
        Controller controller;

    public:
        Robot();
        Robot(Drivetrain drivetrain, Controller controller);

        void forwards(double distance, int max_rpm = 100, bool block = true);
        void backwards(double distance, int max_rpm = 100, bool block = true);
        void turn_right(double angle, int max_rpm = 100, bool block = true);
        void turn_left(double angle, int max_rpm = 100, bool block = true);

        void update();
        void reset();
        void stop();
};

#endif
