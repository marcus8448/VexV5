#include "main.h"
#include "autonomous.hpp"
#include "debug.hpp"
#include "filesystem.hpp"
#include "robot.hpp"
#include <cfloat>

#ifdef REPLAY_MATCH
#include "replay.hpp"
#endif // REPLAY_MATCH

#ifdef RECORD_MATCH 
#include "recording.hpp"
#include <fstream>
#include <filesystem>
#endif // RECORD_MATCH

#ifdef RESET_POSITIONS
#include "reset.hpp"
#endif // RESET_POSITIONS

/**
 * Called when the robot is first initialized.
 */
void initialize() {
    Task::create(debug_input_task, "Debug Input Task");
    Task::create(controller_update_task, "Controller Update Task");

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
    driver_control = false;
    #ifdef REPLAY_MATCH
    replay_match();
    #endif
    
    #ifdef RIGHT_SIDE_WINPOINT
    right_side_winpoint();
    #endif // RIGHT_SIDE_WINPOINT
    #ifdef LEFT_SIDE_WINPOINT
    left_side_winpoint();
    #endif // LEFT_SIDE_WINPOINT
    #ifdef MIDDLE_RIGHT_GOAL
    middle_right_goal();
    #endif // MIDDLE_RIGHT_GOAL
    #ifdef MIDDLE_LEFT_GOAL
    middle_left_goal();
    #endif // MIDDLE_LEFT_GOAL
}

/**
 * Called when the robot is under driver control.
 * Will delegate to autonomous control if the "Force Autonomous" button is pressed.
 */
void opcontrol() {
    #ifdef RESET_POSITIONS
    Task::create(reset_positions, "Reset Task");
    return;
    #endif

    driver_control = true;
    #ifdef RECORD_MATCH
    while (!usd::is_installed()) {
        controller.set_text(2, 0, "Missing microSD!");
        delay(250);
    }
    if (file_exists("/usd/record.v5r")) {
        bool moved = false;
        for (int i = 0; i < 50; i++) {
            std::string filename = std::string("/usd/record_").append(to_string(i)).append(".v5r");
            if (!file_exists(filename)) {
                std::filesystem::rename("/usd/record.v5r", filename);
                moved = true;
                break;
            }
        }
        if (!moved) {
            controller.set_text(2, 0, "Too many files!");
            return;
        }
    }
    controller.clear_line(2);
    std::basic_ofstream<signed int, std::char_traits<signed int>> outf("/usd/record.v5r", std::ios::out | std::ios::binary | std::ios::app | std::ios::trunc);
    #endif // RECORD_MATCH

    #ifdef REPLAY_MATCH
    while (!usd::is_installed()) {
        controller.set_text(2, 0, "Missing microSD!");
        delay(250);
    }
    controller.clear_line(2);
    replay_match();
    return;
    #endif // REPLAY_MATCH

    #ifdef FORCE_AUTONOMOUS
    autonomous();
    return;
    #endif // FORCE_AUTONOMOUS

    unsigned int digital_speed = 127;
    unsigned int prev_digital_speed = 0;
    int cooldown = 0;
    double lift_lock_pos = DBL_MAX;
    double arm_lock_pos_1 = DBL_MAX;
    double arm_lock_pos_2 = DBL_MAX;
    double arm_hook_lock_pos = DBL_MAX;

    while (true) {
        if (cooldown > 0) {
            cooldown--;
            if (cooldown == 0) {
                controller.clear_line(1);
            }
        }
        if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_R1)) && p_err(controller.get_digital(E_CONTROLLER_DIGITAL_R2))) {
            lift.move_absolute(lift.get_position(), digital_speed);
            lift_lock_pos = lift.get_position();
            cooldown = 30;
            controller.set_text(1, 0, "Locked");
        } else {
            if (cooldown == 0) {
                if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_R1))) {
                    p_err(lift.move(digital_speed)); // UP
                    lift_lock_pos = DBL_MAX;
                } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_R2))) {
                    if (lift.get_position() < LIFT_DOWN_POSITION) {
                        lift.move(0);
                    } else {
                        p_err(lift.move(-digital_speed)); // DOWN
                    }
                    lift_lock_pos = DBL_MAX;
                } else {
                    if (lift_lock_pos == DBL_MAX) {
                        p_err(lift.move(0)); // STOP
                    } else {
                        lift.move_absolute(lift_lock_pos, 20);
                    }
                }
            }
        }
        if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_L1)) && p_err(controller.get_digital(E_CONTROLLER_DIGITAL_L2))) {
            move_arm_absolute(arm_1.get_position(), arm_2.get_position(), digital_speed);
            arm_lock_pos_1 = arm_1.get_position();
            arm_lock_pos_2 = arm_2.get_position();
            cooldown = 30;
        } else {
            if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_L1))) {
                move_arm(digital_speed); // UP
                arm_lock_pos_1 = DBL_MAX;
                arm_lock_pos_2 = DBL_MAX;
            } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_L2))) {
                move_arm(digital_speed / -1.4); // DOWN
                arm_lock_pos_1 = DBL_MAX;
                arm_lock_pos_2 = DBL_MAX;
            } else {
                if (arm_lock_pos_1 == DBL_MAX) {
                    move_arm(0); // STOP
                } else {
                    move_arm_absolute(arm_lock_pos_1, arm_lock_pos_2, digital_speed);
                }
            }
        }


        if (cooldown == 0) {
            if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_LEFT)) && p_err(controller.get_digital(E_CONTROLLER_DIGITAL_RIGHT))) {
                p_err(arm_hook.move_relative(0, digital_speed));
                arm_hook_lock_pos = p_err(arm_hook.get_target_position());
                cooldown = 30;
                controller.set_text(1, 0, "Locked");
            } else if (cooldown == 0) { 
                if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_LEFT))) {
                    p_err(arm_hook.move(digital_speed / 1.5)); // OPEN
                    arm_hook_lock_pos = DBL_MAX;
                } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_RIGHT))) {
                    p_err(arm_hook.move(digital_speed / -1.5)); // SHUT
                    arm_hook_lock_pos = DBL_MAX;
                } else if (arm_hook_lock_pos == DBL_MAX) {
                    p_err(arm_hook.move(0)); // STOP
                }
            }
        }

        if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_UP))) {
            if (digital_speed + 1 <= 127) {
                digital_speed += 1;
            }
        } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_DOWN))) {
            if (digital_speed - 1 >= 10) {
                digital_speed -= 1;
            }
        }

        if (cooldown == 0) {
            if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_A))) {
                arm_up(digital_speed, false);
                arm_lock_pos_1 = arm_1.get_target_position();
                arm_lock_pos_2 = arm_2.get_target_position();
            } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_Y))) {
                arm_prime(digital_speed, false);
                arm_lock_pos_1 = arm_1.get_target_position();
                arm_lock_pos_2 = arm_2.get_target_position();
            }
        }

        if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_X)) && p_err(controller.get_digital(E_CONTROLLER_DIGITAL_B))) {
            lift_lift(digital_speed, false);
            lift_lock_pos = lift.get_target_position();
            cooldown = 30;
            controller.set_text(1, 0, "Locked");
        } else if (cooldown == 0) {
            if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_X))) {
                lift_up(digital_speed, false);
                lift_lock_pos = lift.get_target_position();
            } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_B))) {
                lift_down(digital_speed, false);
                lift_lock_pos = lift.get_target_position();
            }
        }

        move_right_motors(p_err(controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y)));
        move_left_motors(p_err(controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));

        if (digital_speed != prev_digital_speed) {
            prev_digital_speed = digital_speed;
            controller.set_text(0, 0, "Digital Speed: " + std::to_string((unsigned int)digital_speed).append(" "));
        }

        #ifdef RECORD_MATCH
        if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_UP)) && p_err(controller.get_digital(E_CONTROLLER_DIGITAL_DOWN))) {
            outf.flush();
            outf.close();
            controller.set_text(0, 0, "Recording Stopped");
            return;
        }
        serialize_controller_state(outf);
        #endif // RECORD_MATCH
        delay(20);
    }
}
