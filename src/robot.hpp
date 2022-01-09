#ifndef _ROBOT_H_
#define _ROBOT_H_
// 4 1/8 d (2 1/16 r)
#define WHEEL_SIZE 2.0625
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
 * @param velocity The max rpm for the motors on the left.
 */
void move_for(double right_distance, double left_distance, int32_t velocity) {
	motor_rf.move_relative(right_distance, velocity);
	motor_rb.move_relative(right_distance, velocity);
	motor_lf.move_relative(left_distance, velocity);
	motor_lb.move_relative(left_distance, velocity);
	double d = abs(motor_rf.get_position() - motor_rf.get_target_position());
	double c = velocity;
	while (abs(motor_rf.get_position() - motor_rf.get_target_position()) > 8 || abs(motor_lb.get_position() - motor_lb.get_target_position()) > 8) {
		double e = velocity * std::min((abs((motor_rf.get_position() - motor_rf.get_target_position())) / d) + 0.7, 1.0);
		if (e < c && e > 0) {
			c = e;
		motor_rf.modify_profiled_velocity(c);
		motor_rb.modify_profiled_velocity(c);
		motor_lf.modify_profiled_velocity(c);
		motor_lb.modify_profiled_velocity(c);
		}
		delay(50);
	}
	delay(100);
}

/**
 * Moves the robot forwards in inches.
 * @param distance The distance to move in inches.
 */
void forwards(double distance, int32_t velocity) {
	print_out("Move forwards: " + std::to_string(distance));
	move_for(distance * INCHES_TO_DEGREES, distance * INCHES_TO_DEGREES, velocity);
}

/**
 * Moves the robot backwards in inches.
 * @param distance The distance to move in inches.
 */
void backwards(double distance, int32_t velocity) {
	print_out("Move back: " + std::to_string(distance));
	move_for(-distance * INCHES_TO_DEGREES, -distance * INCHES_TO_DEGREES, velocity);
}

/**
 * Turns the robot right.
 * @param angle The amount to turn in degrees.
 */
void turn_right(double angle, int32_t velocity) {
	print_out("Turn right: " + std::to_string(angle));
	angle *= DEGREES_TO_ROTATION_DEGREES;
	move_for(-angle, angle, velocity);
}

/**
 * Turns the robot left.
 * @param angle The amount to turn in degrees.
 */
void turn_left(uint16_t angle, int32_t velocity) {
	print_out("Turn left: " + std::to_string(angle));
	angle *= DEGREES_TO_ROTATION_DEGREES;
	move_for(angle, -angle, velocity);
}

void arm_down(int32_t velocity) {
	print_out("Arm Down");
	arm.move_absolute(135, velocity);
	arm2.move_absolute(135, velocity);
	delay(400);
	while (abs(arm.get_position() - arm.get_target_position()) > 8) {
		delay(50);
	}
}

void arm_lift() {
	print_out("Arm Lift");
	arm.move_absolute(110, 200);
	arm2.move_absolute(110, 200);
	while (abs(arm.get_position() - arm.get_target_position()) > 8) {
		delay(50);
	}
}

void arm_up(int32_t velocity) {
	print_out("Arm Up");
	arm.move_absolute(0, velocity);
	arm2.move_absolute(0, velocity);
	delay(400);
	while (abs(arm.get_position() - arm.get_target_position()) > 8) {
		delay(50);
	}
}

#endif // _ROBOT_H_
