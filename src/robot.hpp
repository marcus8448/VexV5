#ifndef _ROBOT_H_
#define _ROBOT_H_
#define WHEEL_SIZE 2.0625 /* 4 1/8 d (2 1/16 r) */
#define INCHES_TO_DEGREES (360.0 / ((2.0 * 3.14159265358979323846) * (WHEEL_SIZE * WHEEL_SIZE))) * 2.0
#define DEGREES_TO_ROTATION_DEGREES 4
// 12.9590697in = 360 degrees
#include "screen.hpp"
#include <cmath>
#include <string>

Motor motor_rf(10, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor motor_rb(20, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor motor_lf(1, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor motor_lb(11, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor lift(8, E_MOTOR_GEARSET_36, false, E_MOTOR_ENCODER_DEGREES);
Motor arm_1(9, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor arm_2(2, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor arm_hook(16, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);

Controller controller(E_CONTROLLER_MASTER);

/**
 * \return the position of the lift.
 */
double lift_position() {
    return p_err(lift.get_position());
}

/**
 * \return the distance in motor encoder units the motors are from their target position.
 */
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
void move_for(double right_distance, double left_distance, int32_t max_rpm = 120) {
    p_err(motor_rf.move_relative(right_distance, max_rpm));
    p_err(motor_rb.move_relative(right_distance, max_rpm));
    p_err(motor_lf.move_relative(left_distance, max_rpm));
    p_err(motor_lb.move_relative(left_distance, max_rpm));
    double d = motor_offset_relative();
    while (motor_offset_relative() > 8) {
        double e = max_rpm * std::sin(std::min(fabs((motor_offset_relative() / 333.357263) * 0.2) + 0.8, 1.0) * 90); //about 1ft away it should slow down.
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
void forwards(double distance, int32_t max_rpm = 120) {
    print("Move forwards: " + std::to_string(distance));
    move_for(distance * INCHES_TO_DEGREES, distance * INCHES_TO_DEGREES, max_rpm);
}

/**
 * Moves the robot backwards in inches.
 * \param distance The distance to move in inches.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void backwards(double distance, int32_t max_rpm = 120) {
    print("Move back: " + std::to_string(distance));
    move_for(-distance * INCHES_TO_DEGREES, -distance * INCHES_TO_DEGREES, max_rpm);
}

/**
 * Turns the robot right.
 * \param angle The amount to turn in degrees.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void turn_right(double angle, int32_t max_rpm = 120) {
    print("Turn right: " + std::to_string(angle));
    angle *= DEGREES_TO_ROTATION_DEGREES;
    move_for(-angle, angle, max_rpm);
}

/**
 * Turns the robot left.
 * \param angle The amount to turn in degrees.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void turn_left(uint16_t angle, int32_t max_rpm = 120) {
    print("Turn left: " + std::to_string(angle));
    angle *= DEGREES_TO_ROTATION_DEGREES;
    move_for(angle, -angle, max_rpm);
}

/**
 * Puts the lift down to pick up or drop off a mobile goal.
 * \param max_rpm The maximum allowable rpm for the lift motors.
 * \param block Whether this operation is blocking and should wait for the lift to move.
 */
void lift_down(int32_t max_rpm = 100, bool block = true) {
    print("Lift Down");
    p_err(lift.move_absolute(135, max_rpm));
    if (block) {
        while (fabs(lift_position() - 135) > 8.0) {
            delay(50);
        }
    }
}

/**
 * Lifts the lift up slightly to be able to move around with mobile goals.
 * \param max_rpm The maximum allowable rpm for the lift motors.
 * \param block Whether this operation is blocking and should wait for the lift to move.
 */
void lift_lift(int32_t max_rpm = 100, bool block = true) {
    print("Lift Lift");
    p_err(lift.move_absolute(110, max_rpm));
    if (block) {
        while (fabs(lift_position() - 110) > 8.0) {
            delay(50);
        }
    }
}

/**
 * Moves the lift all the way up to its starting position.
 * \param max_rpm The maximum allowable rpm for the lift motors.
 * \param block Whether this operation is blocking and should wait for the lift to move.
 */
void lift_up(int32_t max_rpm = 100, bool block = true) {
    print("Lift Up");
    p_err(lift.move_absolute(0, max_rpm));
    if (block) {
        while (fabs(lift_position() - 0.0) > 8.0) {
            delay(50);
        }
    }
}

/**
 * Moves the arm at the specificed rate.
 * \param voltage The maximum allowable 'voltage' for the arm motors.
 */
void move_arm(int32_t voltage) {
    p_err(arm_1.move(voltage));
    p_err(arm_2.move(voltage));
}

/**
 * Moves the right motors at the specificed rate.
 * \param voltage The maximum allowable 'voltage' for the arm motors.
 */
void move_right_motors(int32_t voltage) {
    p_err(motor_rf.move(voltage));
    p_err(motor_rb.move(voltage));
}

/**
 * Moves the left motors at the specificed rate.
 * \param voltage The maximum allowable 'voltage' for the arm motors.
 */
void move_left_motors(int32_t voltage) {
    p_err(motor_lf.move(voltage));
    p_err(motor_lb.move(voltage));
}
#endif // _ROBOT_H_
