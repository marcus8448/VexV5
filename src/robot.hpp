#ifndef _ROBOT_H_
#define _ROBOT_H_

#define WHEEL_SIZE 2.0625 /* 4 1/8 d (2 1/16 r) */
#define INCHES_TO_DEGREES (360.0 / ((2.0 * 3.14159265358979323846) * (WHEEL_SIZE * WHEEL_SIZE))) * 2.0
// todo: 12.9590697in = 360 degrees
#define DEGREES_TO_ROTATION_DEGREES 4

#include <cmath>
#include <string>
#include "debug.hpp"
#include "devices.hpp"

/**
 * \return the position of the lift.
 */
double lift_position() {
    return p_err(lift.get_position());
}

/**
 * \return the average position of the arm.
 */
double arm_position() {
    return p_err((arm_1.get_position() + arm_2.get_position()) / 2.0);
}

void move_arm_absolute(double position, int32_t velocity) {
    p_err(arm_1.move_absolute(position, velocity));
    p_err(arm_2.move_absolute(position, velocity));
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
    print("Lift down");
    p_err(lift.move_absolute(-470.0, max_rpm));
    if (block) {
        while (fabs(lift_position() - -470.0) > 8.0) {
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
    print("Lift lift");
    p_err(lift.move_absolute(-120.0, max_rpm));
    if (block) {
        while (fabs(lift_position() - -120.0) > 8.0) {
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
    print("Lift up");
    p_err(lift.move_absolute(0, max_rpm));
    if (block) {
        while (fabs(lift_position() - 0.0) > 8.0) {
            delay(50);
        }
    }
}

/**
 * Puts the arm down to pick up or drop off a mobile goal.
 * \param max_rpm The maximum allowable rpm for the arm motors.
 * \param block Whether this operation is blocking and should wait for the arm to move.
 */
void arm_down(int32_t max_rpm = 100, bool block = true) {
    print("Arm down");
    move_arm_absolute(0, max_rpm);
    if (block) {
        while (fabs(arm_position()) > 8.0) {
            delay(50);
        }
    }
}

/**
 * Moves the arm all the way up to its starting position.
 * \param max_rpm The maximum allowable rpm for the arm motors.
 * \param block Whether this operation is blocking and should wait for the arm to move.
 */
void arm_prime(int32_t max_rpm = 100, bool block = true) {
    print("Arm prime");
    move_arm_absolute(-200.0, max_rpm);
    if (block) {
        while (fabs(arm_position() - -200.0) > 8.0) {
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
    print("Arm up");
    move_arm_absolute(-460.0, max_rpm);
    if (block) {
        while (fabs(arm_position() - -460.0) > 8.0) {
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
 * Moves the arm at the specificed rate.
 * \param voltage The maximum allowable 'voltage' for the arm motors.
 */
void move_arm_absolute(double b, double c, int32_t voltage) {
    p_err(arm_1.move_absolute(b, voltage));
    p_err(arm_2.move_absolute(c, voltage));
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
