#ifndef GLOBALS_HPP    
#define GLOBALS_HPP

#define PI 3.14159265358979323846
#define WHEEL_SIZE 2.0625
#define WHEEL_CIRCUMFRENCE (WHEEL_SIZE * 2 * PI)

#define LIFT_DOWN_POSITION -475.0
#define LIFT_UP_POSITION 0.0
#define LIFT_LIFTED_POSITION -410.0

#define ARM_DOWN_POSITION 0.0
#define ARM_UP_POSITION 700.0
#define ARM_LIFTED_POSITION 100.0

#include "pros/motors.hpp"
#include "pros/misc.hpp"

using namespace pros;

/**
 * The motor on the right front of the robot for a tank drive
 */
static Motor motor_rf(10, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);

/**
 * The motor on the right back of the robot for a tank drive
 */
static Motor motor_rb(20, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);

/**
 * The motor on the left front of the robot for a tank drive
 */
static Motor motor_lf(1, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);

/**
 * The motor on the left back of the robot for a tank drive
 */
static Motor motor_lb(11, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);

/**
 * The motor that controlls the lift
 */
static Motor lift(6, E_MOTOR_GEARSET_36, true, E_MOTOR_ENCODER_DEGREES);

/**
 * The first motor that controlls the arm
 */
static Motor arm_1(12, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES); 

/**
 * The second motor that controlls the arm
 */
static Motor arm_2(19, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);

/**
 * The motor that controlls the hook on the arm
 */
static Motor arm_hook(13, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);

/**
 * The connected controller
 */
static Controller controller(E_CONTROLLER_MASTER);

#endif