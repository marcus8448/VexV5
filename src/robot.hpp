#ifndef _ROBOT_H_
#define _ROBOT_H_
// 4 1/8 d (2 1/16 r)
#define WHEEL_SIZE 2.0625
#define INCHES_TO_DEGREES (360.0 / ((2.0 * 3.14159265358979323846) * (WHEEL_SIZE * WHEEL_SIZE))) * 2.0
#define DEGREES_TO_ROTATION_DEGREES 4

#include "screen.hpp"
#include <cmath>
#include <string>

Motor motor_rf(10, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor motor_rb(20, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor motor_lf(1, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor motor_lb(11, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor arm(2, E_MOTOR_GEARSET_36, true, E_MOTOR_ENCODER_DEGREES);
Motor arm2(9, E_MOTOR_GEARSET_36, false, E_MOTOR_ENCODER_DEGREES);

Controller controller(E_CONTROLLER_MASTER);

double arm_position() {
    return (p_err(arm.get_position()) + p_err(arm2.get_position())) / 2.0;
}

double motor_offset_relative() {
    return (fabs(p_err(motor_rf.get_position()) - p_err(motor_rf.get_target_position()))
        + fabs(p_err(motor_rb.get_position()) - p_err(motor_rb.get_target_position()))
        + fabs(p_err(motor_lf.get_position()) - p_err(motor_lf.get_target_position()))
        + fabs(p_err(motor_lb.get_position()) - p_err(motor_lb.get_target_position()))) / 4.0;
}

/**
 * Moves the drivetrain a specific distance.
 * \param right_distance the distance to move the right motors in degrees.
 * \param left_distance the distance to move the left motors in degrees.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void move_for(double right_distance, double left_distance, int32_t max_rpm) {
    p_err(motor_rf.move_relative(right_distance, max_rpm));
    p_err(motor_rb.move_relative(right_distance, max_rpm));
    p_err(motor_lf.move_relative(left_distance, max_rpm));
    p_err(motor_lb.move_relative(left_distance, max_rpm));
    double d = motor_offset_relative();
    while (motor_offset_relative() > 8) {
        double e = max_rpm * std::min((motor_offset_relative() / d) + 0.8, 1.0);
        if (e < max_rpm && e > 0) {
            max_rpm = (int32_t) e;
            p_err(motor_rf.modify_profiled_velocity(max_rpm));
            p_err(motor_rb.modify_profiled_velocity(max_rpm));
            p_err(motor_lf.modify_profiled_velocity(max_rpm));
            p_err(motor_lb.modify_profiled_velocity(max_rpm));
        }
        delay(50);
    }
    delay(100);
}

/**
 * Moves the robot forwards in inches.
 * \param distance The distance to move in inches.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void forwards(double distance, int32_t max_rpm) {
    print("Move forwards: " + std::to_string(distance));
    move_for(distance * INCHES_TO_DEGREES, distance * INCHES_TO_DEGREES, max_rpm);
}

/**
 * Moves the robot backwards in inches.
 * \param distance The distance to move in inches.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void backwards(double distance, int32_t max_rpm) {
    print("Move back: " + std::to_string(distance));
    move_for(-distance * INCHES_TO_DEGREES, -distance * INCHES_TO_DEGREES, max_rpm);
}

/**
 * Turns the robot right.
 * \param angle The amount to turn in degrees.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void turn_right(double angle, int32_t max_rpm) {
    print("Turn right: " + std::to_string(angle));
    angle *= DEGREES_TO_ROTATION_DEGREES;
    move_for(-angle, angle, max_rpm);
}

/**
 * Turns the robot left.
 * \param angle The amount to turn in degrees.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void turn_left(uint16_t angle, int32_t max_rpm) {
    print("Turn left: " + std::to_string(angle));
    angle *= DEGREES_TO_ROTATION_DEGREES;
    move_for(angle, -angle, max_rpm);
}

/**
 * Puts the arm down to pick up or drop off a mobile goal.
 * \param max_rpm The maximum allowable rpm for the arm motors.
 * \param block Whether this operation is blocking and should wait for the arm to move.
 */
void arm_down(int32_t max_rpm = 100, bool block = true) {
    print("Arm Down");
    p_err(arm.move_absolute(135, max_rpm));
    p_err(arm2.move_absolute(135, max_rpm));
    if (block) {
        while (fabs(arm_position() - 135) > 8.0) {
            delay(50);
        }
    }
}

/**
 * Lifts the arm up slightly to be able to move around with mobile goals.
 * \param max_rpm The maximum allowable rpm for the arm motors.
 * \param block Whether this operation is blocking and should wait for the arm to move.
 */
void arm_lift(int32_t max_rpm = 100, bool block = true) {
    print("Arm Lift");
    p_err(arm.move_absolute(110, max_rpm));
    p_err(arm2.move_absolute(110, max_rpm));
    if (block) {
        while (fabs(arm_position() - 110) > 8.0) {
            delay(50);
        }
    }
}

/**
 * Moves the arm all the way up to its starting position.
 * \param max_rpm The maximum allowable rpm for the arm motors.
 * \param block Whether this operation is blocking and should wait for the arm to move.
 */
void arm_up(int32_t max_rpm = 100, bool block = true) {
    print("Arm Up");
    p_err(arm.move_absolute(0, max_rpm));
    p_err(arm2.move_absolute(0, max_rpm));
    if (block) {
        while (fabs(arm_position() - 0.0) > 8.0) {
            delay(50);
        }
    }
}

#endif// _ROBOT_H_
