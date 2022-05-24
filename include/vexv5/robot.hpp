#ifndef ROBOT_HPP
#define ROBOT_HPP

/**
 * \return the position of the lift.
 */
double lift_position();

/**
 * \return the average position of the arm.
 */
double arm_position();

/**
 * \return the distance in motor encoder units the motors are from their target position.
 */
double motor_offset_relative();

/**
 * Moves the arm at the specificed rate.
 * \param voltage The maximum allowable 'voltage' for the arm motors.
 */
void move_arm_absolute(double b, double c, int voltage);

/**
 * Moves the arm at the specificed rate.
 * \param voltage The maximum allowable 'voltage' for the arm motors.
 */
void move_arm_absolute(double d, int voltage, bool block);

/**
 * Moves the drivetrain a specific distance.
 * \param right_distance the distance to move the right motors in degrees.
 * \param left_distance the distance to move the left motors in degrees.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void move_for(double right_distance, double left_distance, int max_rpm = 60);

/**
 * Moves the robot forwards in inches.
 * \param distance The distance to move in inches.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void forwards(double distance, int max_rpm = 100);

/**
 * Moves the robot backwards in inches.
 * \param distance The distance to move in inches.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void backwards(double distance, int max_rpm = 100);

/**
 * Turns the robot right.
 * \param angle The amount to turn in degrees.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void turn_right(double angle, int max_rpm = 100);

/**
 * Turns the robot left.
 * \param angle The amount to turn in degrees.
 * \param max_rpm The maximum allowable rpm for the motors.
 */
void turn_left(double angle, int max_rpm = 100);

void lift_move_absolute(double position, int max_rpm = 100, bool block = true);

/**
 * Puts the lift down to pick up or drop off a mobile goal.
 * \param max_rpm The maximum allowable rpm for the lift motors.
 * \param block Whether this operation is blocking and should wait for the lift to move.
 */
void lift_down(int max_rpm = 100, bool block = true);

/**
 * Lifts the lift up slightly to be able to move around with mobile goals.
 * \param max_rpm The maximum allowable rpm for the lift motors.
 * \param block Whether this operation is blocking and should wait for the lift to move.
 */
void lift_lift(int max_rpm = 100, bool block = true);

/**
 * Moves the lift all the way up to its starting position.
 * \param max_rpm The maximum allowable rpm for the lift motors.
 * \param block Whether this operation is blocking and should wait for the lift to move.
 */
void lift_up(int max_rpm = 100, bool block = true);

void arm_move_absolute(double position, int max_rpm = 100, bool block = true);

/**
 * Puts the arm down to pick up or drop off a mobile goal.
 * \param max_rpm The maximum allowable rpm for the arm motors.
 * \param block Whether this operation is blocking and should wait for the arm to move.
 */
void arm_down(int max_rpm = 100, bool block = true);
/**
 * Moves the arm all the way up to its starting position.
 * \param max_rpm The maximum allowable rpm for the arm motors.
 * \param block Whether this operation is blocking and should wait for the arm to move.
 */
void arm_prime(int max_rpm = 100, bool block = true);

/**
 * Moves the arm all the way up to its starting position.
 * \param max_rpm The maximum allowable rpm for the arm motors.
 * \param block Whether this operation is blocking and should wait for the arm to move.
 */
void arm_up(int max_rpm = 100, bool block = true);

void arm_hook_open(int max_rpm = 50, bool block = true);

void arm_hook_close(int max_rpm = 50);

/**
 * Moves the arm at the specificed rate.
 * \param voltage The maximum allowable 'voltage' for the arm motors.
 */
void move_arm(int voltage);

/**
 * Moves the right motors at the specificed rate.
 * \param voltage The maximum allowable 'voltage' for the arm motors.
 */
void move_right_motors(int voltage);

/**
 * Moves the left motors at the specificed rate.
 * \param voltage The maximum allowable 'voltage' for the arm motors.
 */
void move_left_motors(int voltage);

#endif
