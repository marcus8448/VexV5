#include <cmath>
#include <string>
#include "pros/rtos.hpp"
#include "vexv5/debug.hpp"
#include "vexv5/globals.hpp"

double inches_to_wheel_rotation(double inches) {
    return inches / WHEEL_CIRCUMFRENCE * 360.0;
}

double deg_rot_to_deg_wheel_rot(double degrees) {
    return degrees * 4.0;
}

/**
 * \return the position of the lift.
 */
double lift_position() {
    return errd(lift.get_position());
}

/**
 * \return the average position of the arm.
 */
double arm_position() {
    return errd((arm_1.get_position() + arm_2.get_position()) / 2.0);
}

/**
 * \return the distance in motor encoder units the motors are from their target position.
 */
double motor_offset_relative() {
    return (fabs(errd(motor_rf.get_position()) - errd(motor_rf.get_target_position()))
        + fabs(errd(motor_rb.get_position()) - errd(motor_rb.get_target_position()))
        + fabs(errd(motor_lf.get_position()) - errd(motor_lf.get_target_position()))
        + fabs(errd(motor_lb.get_position()) - errd(motor_lb.get_target_position()))) / 4.0;
}

/**
 * Moves the arm at the specificed rate.
 * \param voltage The maximum allowable 'voltage' for the arm motors.
 */
void move_arm_absolute(double b, double c, int voltage) {
    erri(arm_1.move_absolute(b, voltage));
    erri(arm_2.move_absolute(c, voltage));
}

/**
 * Moves the arm at the specificed rate.
 * \param voltage The maximum allowable 'voltage' for the arm motors.
 */
void move_arm_absolute(double d, int voltage, bool block) {
    erri(arm_1.move_absolute(d, voltage));
    erri(arm_2.move_absolute(d, voltage));
    while (std::abs(arm_position() - d) > 8.0) {
        delay(50);
    }
    delay(100);
}

/**
 * Moves the drivetrain a specific distance.
 * \param right_distance the distance to move the right motors in degrees.
 * \param left_distance the distance to move the left motors in degrees.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void move_for(double right_distance, double left_distance, int max_rpm = 60) {
    erri(motor_rf.move_relative(right_distance, max_rpm));
    erri(motor_rb.move_relative(right_distance, max_rpm));
    erri(motor_lf.move_relative(left_distance, max_rpm));
    erri(motor_lb.move_relative(left_distance, max_rpm));
    double d = motor_offset_relative();
    double inv_progress = 1.0;
    while ((inv_progress = (motor_offset_relative() / d)) > 0.10) {
        delay(50);
    }
    do {
        delay(25);
    } while  ((inv_progress = (motor_offset_relative() / d)) > 0.05);
}

/**
 * Moves the robot forwards in inches.
 * \param distance The distance to move in inches.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void forwards(double distance, int max_rpm = 100) {
    print("Move forwards: " + std::to_string(distance));
    move_for(inches_to_wheel_rotation(distance), inches_to_wheel_rotation(distance), max_rpm);
}

/**
 * Moves the robot backwards in inches.
 * \param distance The distance to move in inches.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void backwards(double distance, int max_rpm = 100) {
    print("Move back: " + std::to_string(distance));
    move_for(inches_to_wheel_rotation(-distance), inches_to_wheel_rotation(-distance), max_rpm);
}

/**
 * Turns the robot right.
 * \param angle The amount to turn in degrees.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void turn_right(double angle, int max_rpm = 100) {
    print("Turn right: " + std::to_string(angle));
    move_for(-deg_rot_to_deg_wheel_rot(angle), deg_rot_to_deg_wheel_rot(angle), max_rpm);
}

/**
 * Turns the robot left.
 * \param angle The amount to turn in degrees.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void turn_left(double angle, int max_rpm = 100) {
    print("Turn left: " + std::to_string(angle));
    move_for(deg_rot_to_deg_wheel_rot(angle), -deg_rot_to_deg_wheel_rot(angle), max_rpm);
}

void lift_move_absolute(double position, int max_rpm = 100, bool block = true) {
    erri(lift.move_absolute(position, max_rpm));

    if (block) {
        while (fabs(arm_position() - position) > 8.0) {
            delay(50);
        }
    }
}

/**
 * Puts the lift down to pick up or drop off a mobile goal.
 * \param max_rpm The maximum allowable rpm for the lift motors.
 * \param block Whether this operation is blocking and should wait for the lift to move.
 */
void lift_down(int max_rpm = 100, bool block = true) {
    print("Lift down");
    lift_move_absolute(LIFT_DOWN_POSITION, max_rpm, block);
}

/**
 * Lifts the lift up slightly to be able to move around with mobile goals.
 * \param max_rpm The maximum allowable rpm for the lift motors.
 * \param block Whether this operation is blocking and should wait for the lift to move.
 */
void lift_lift(int max_rpm = 100, bool block = true) {
    print("Lift lift");
    lift_move_absolute(LIFT_LIFTED_POSITION, max_rpm, block);
}

/**
 * Moves the lift all the way up to its starting position.
 * \param max_rpm The maximum allowable rpm for the lift motors.
 * \param block Whether this operation is blocking and should wait for the lift to move.
 */
void lift_up(int max_rpm = 100, bool block = true) {
    print("Lift up");
    lift_move_absolute(LIFT_UP_POSITION, max_rpm, block);
}

void arm_move_absolute(double position, int max_rpm = 100, bool block = true) {
    erri(arm_1.move_absolute(position, max_rpm));
    erri(arm_2.move_absolute(position, max_rpm));

    if (block) {
        while (fabs(arm_position() - -position) > 8.0) {
            delay(50);
        }
    }
}

/**
 * Puts the arm down to pick up or drop off a mobile goal.
 * \param max_rpm The maximum allowable rpm for the arm motors.
 * \param block Whether this operation is blocking and should wait for the arm to move.
 */
void arm_down(int max_rpm = 100, bool block = true) {
    print("Arm down");
    move_arm_absolute(ARM_DOWN_POSITION, max_rpm, block);
}

/**
 * Moves the arm all the way up to its starting position.
 * \param max_rpm The maximum allowable rpm for the arm motors.
 * \param block Whether this operation is blocking and should wait for the arm to move.
 */
void arm_prime(int max_rpm = 100, bool block = true) {
    print("Arm prime");
    move_arm_absolute(ARM_LIFTED_POSITION, max_rpm, block);
}

/**
 * Moves the arm all the way up to its starting position.
 * \param max_rpm The maximum allowable rpm for the arm motors.
 * \param block Whether this operation is blocking and should wait for the arm to move.
 */
void arm_up(int max_rpm = 100, bool block = true) {
    print("Arm up");
    move_arm_absolute(ARM_UP_POSITION, max_rpm, block);
}

void arm_hook_open(int max_rpm = 50, bool block = true) {
    print("Arm open");
    arm_hook.move_absolute(0.0, max_rpm);
    if (block) {
        while (fabs(arm_hook.get_position()) > 8.0) {
            delay(50);
        }
    }
}

void arm_hook_close(int max_rpm = 50) {
    print("Arm close");
    arm_hook.move_velocity(-max_rpm);
    delay(100);
    while (arm_hook.get_efficiency() > 2.0) {
        delay(40);
    }
    arm_hook.move_velocity(-max_rpm / 8.0);
}

/**
 * Moves the arm at the specificed rate.
 * \param voltage The maximum allowable 'voltage' for the arm motors.
 */
void move_arm(int voltage) {
    erri(arm_1.move(voltage));
    erri(arm_2.move(voltage));
}

/**
 * Moves the right motors at the specificed rate.
 * \param voltage The maximum allowable 'voltage' for the arm motors.
 */
void move_right_motors(int voltage) {
    erri(motor_rf.move(voltage));
    erri(motor_rb.move(voltage));
}

/**
 * Moves the left motors at the specificed rate.
 * \param voltage The maximum allowable 'voltage' for the arm motors.
 */
void move_left_motors(int voltage) {
    erri(motor_lf.move(voltage));
    erri(motor_lb.move(voltage));
}
