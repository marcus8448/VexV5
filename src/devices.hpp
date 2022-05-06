#ifndef _ROBOT_DEVICES_HPP_
#define _ROBOT_DEVICES_HPP_

#include "pros/motors.hpp"
#include "pros/misc.hpp"

using namespace pros;

Motor motor_rf(10, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor motor_rb(20, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor motor_lf(1, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor motor_lb(11, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor lift(6, E_MOTOR_GEARSET_36, true, E_MOTOR_ENCODER_DEGREES);
Motor arm_1(15, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES); 
Motor arm_2(16, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor arm_hook(13, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);

Controller controller(E_CONTROLLER_MASTER);

#endif //_ROBOT_DEVICES_HPP_