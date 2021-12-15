#ifndef _ROBOT_H_
#define _ROBOT_H_

#define WHEEL_SIZE 2.044
#define INCHES_TO_DEGREES (360 / ((2 * 3.14159265358979323846) * (WHEEL_SIZE * WHEEL_SIZE))) * 2
#define DEGREES_TO_ROTATION_DEGREES 4

#include <math.h>
#include <string>
#include "screen.hpp"

Motor motor_rf(10, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor motor_rb(20, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor motor_lf(1, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor motor_lb(11, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor arm(2, E_MOTOR_GEARSET_36, false, E_MOTOR_ENCODER_DEGREES);
Motor arm2(9, E_MOTOR_GEARSET_36, true, E_MOTOR_ENCODER_DEGREES);

Controller controller(E_CONTROLLER_MASTER);

/**
 * Moves the drivetrain a specific distance.
 * @param right_distance the distance to move the motors in degrees.
 * @param left_distance The max rpm for the motors on the right.
 * @param max_rpm The max rpm for the motors on the left.
 */
void move_for(double right_distance, double left_distance, int32_t max_rpm) {
	motor_rf.move_relative(right_distance, max_rpm);
	motor_rb.move_relative(right_distance, max_rpm);
	motor_lf.move_relative(left_distance, max_rpm);
	motor_lb.move_relative(left_distance, max_rpm);

	double prev_rf = 0;
	double prev_lb = 0;
	delay(400);
	while (std::abs(motor_rf.get_actual_velocity()) >= prev_rf - 8 || std::abs(motor_lb.get_actual_velocity() >= prev_lb - 8)) {
		prev_rf = motor_rf.get_actual_velocity();
		prev_lb = motor_lb.get_actual_velocity();
		delay(50);
	}
	while (!motor_rf.is_stopped() || !motor_lb.is_stopped()) {
		delay(20);
	}
	delay(100);
	print_out(motor_rf.get_actual_velocity());

	// motor_rf.move_velocity(0);
	// motor_rb.move_velocity(0);
	// motor_lf.move_velocity(0);
	// motor_lb.move_velocity(0);
}

/**
 * Moves the robot forwards in inches.
 * @param distance The distance to move in inches.
 */
void forwards(double distance, int32_t max_rpm) {
	move_for(distance * INCHES_TO_DEGREES, distance * INCHES_TO_DEGREES, max_rpm);
}

/**
 * Moves the robot backwards in inches.
 * @param distance The distance to move in inches.
 */
void backwards(double distance, int32_t max_rpm) {
	forwards(-distance, max_rpm);
}

/**
 * Turns the robot right.
 * @param angle The amount to turn in degrees.
 */
void turn_right(double angle, int32_t max_rpm) {
	angle *= DEGREES_TO_ROTATION_DEGREES;
	move_for(-angle, angle, max_rpm);
}

/**
 * Turns the robot left.
 * @param angle The amount to turn in degrees.
 */
void turn_left(uint16_t angle, int32_t max_rpm) {
	angle *= DEGREES_TO_ROTATION_DEGREES;
	move_for(angle, -angle, max_rpm);
}

#endif // _ROBOT_H_
