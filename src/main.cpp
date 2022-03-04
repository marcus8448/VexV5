#include "main.h"
#include "debug.hpp"
#include "robot.hpp"
#include "debug.hpp"

#ifdef REPLAY_MATCH
#include "replay.hpp"
#endif
#ifdef RECORD_MATCH 
#include "recording.hpp"
#include <fstream>
#endif

/**
 * Called when the robot is first initialized.
 */
void initialize() {
    Task::create(debug_input_task, "Debug Input Task");

    motor_rf.set_brake_mode(MOTOR_BRAKE_BRAKE);
    motor_rb.set_brake_mode(MOTOR_BRAKE_BRAKE);
    motor_lf.set_brake_mode(MOTOR_BRAKE_BRAKE);
    motor_lb.set_brake_mode(MOTOR_BRAKE_BRAKE);
    lift.set_brake_mode(MOTOR_BRAKE_HOLD);
    lift.set_zero_position(0);
    arm_1.set_brake_mode(MOTOR_BRAKE_HOLD);
    arm_2.set_brake_mode(MOTOR_BRAKE_HOLD);
    arm_hook.set_brake_mode(MOTOR_BRAKE_HOLD);
    arm_1.set_zero_position(0);
    arm_2.set_zero_position(0);
}

void disabled() {
}

void competition_initialize() {
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 * Or when the "Force Autonomous" button is presssed on the screen.
 */
void autonomous() {
    #ifdef REPLAY_MATCH
    replay_match();
    #endif
}

/**
 * Called when the robot is under driver control.
 * Will delegate to autonomous control if the "Force Autonomous" button is pressed.
 */
void opcontrol() {
    #ifdef RECORD_MATCH
    std::basic_ofstream<unsigned char, std::char_traits<unsigned char>> outf("/usd/record.v5r", std::ios::out | std::ios::binary | std::ios::app | std::ios::trunc);
    #endif // RECORD_MATCH

    #ifdef REPLAY_MATCH
    replay_match();
    return;
    #endif

    #ifdef FORCE_AUTONOMOUS
    autonomous();
    return;
    #endif

    unsigned int digital_speed = 127;
    unsigned int prev_digital_speed = 0;
    int cooldown = 0;
    bool lift_lock = false;
    bool arm_lock = false;
    
    while (true) {
        if (cooldown > 0) {
            cooldown--;
        } else if (cooldown < 0) {
            cooldown = 0;
        }
        if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_R1))) {
            p_err(lift.move(digital_speed)); // UP
            lift_lock = false;
        } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_R2))) {
            p_err(lift.move(-digital_speed)); // DOWN
            lift_lock = false;
        } else {
            if (!lift_lock) {
                p_err(lift.move(0)); // STOP
            }
        }
        if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_L1))) {
            move_arm(-digital_speed); // UP
            arm_lock = false;
        } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_L2))) {
            move_arm(digital_speed); // DOWN
            arm_lock = false;
        } else {
            if (!arm_lock) {
                move_arm(0); // STOP
            }
        }

        if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_LEFT))) {
            p_err(arm_hook.move(-100)); // OPEN
        } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_RIGHT))) {
            p_err(arm_hook.move(100)); // SHUT
        } else {
            p_err(arm_hook.move(0)); // STOP
        }

        if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_UP))) {
            if (digital_speed + 1 <= 127) {
                digital_speed += 1;
            }
        } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_DOWN))) {
            if (digital_speed - 1 > 20) {
                digital_speed -= 1;
            }
        }

        if (cooldown == 0) {
            if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_A))) {
                arm_up(200, false);
                arm_lock = true;
            } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_Y))) {
                arm_prime(200, false);
                arm_lock = true;
            }
        }

        if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_X)) && p_err(controller.get_digital(E_CONTROLLER_DIGITAL_B))) {
            lift_lift(200, false);
            lift_lock = true;
            cooldown = 20;
        } else if (cooldown == 0) {
            if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_X))) {
                lift_up(200, false);
                lift_lock = true;
            } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_B))) {
                lift_down(200, false);
                lift_lock = true;
            }
        }

        move_right_motors(p_err(controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y)));
        move_left_motors(p_err(controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));

        if (digital_speed != prev_digital_speed) {
            prev_digital_speed = digital_speed;
            controller.set_text(0, 0, std::string("Digital Speed: ").append(std::to_string((int)digital_speed)));
        }

        #ifdef RECORD_MATCH
        if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_A)) && p_err(controller.get_digital(E_CONTROLLER_DIGITAL_Y))) {
            outf.flush();
            outf.close();
            break;
        }
        serialize_controller_state(outf);
        #endif // RECORD_MATCH
        delay(20);
    }
}
