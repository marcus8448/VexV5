#include <cfloat>
#include <string>
#include "main.h"
#include "vexv5/autonomous.hpp"
#include "vexv5/debug.hpp"
#include "vexv5/globals.hpp"
#include "vexv5/opcontrol.hpp"
#include "vexv5/robot.hpp"

#ifdef REPLAY_MATCH
#include "replay.hpp"
#endif // REPLAY_MATCH

#ifdef RECORD_MATCH
#include "filesystem.hpp"
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
    // Task::create(controller_update_task, "Controller Update Task");

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

/**
 * Called when the robot is in it's autonomous state in a competition.
 * Or when the "Force Autonomous" button is presssed on the screen.
 */
void autonomous() {
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
    #ifdef FORCE_AUTONOMOUS
    autonomous();
    return;
    #endif // FORCE_AUTONOMOUS

    #ifdef RESET_POSITIONS
    reset_positions();
    return;
    #endif

    #ifdef RECORD_MATCH
    while (!usd::is_installed()) {
        controller.set_text(2, 0, "Missing microSD!");
        delay(500);
    }
    controller.clear_line(2);
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
    std::basic_ofstream<signed int, std::char_traits<signed int>> outf("/usd/record.v5r", std::ios::out | std::ios::binary | std::ios::trunc);
    #endif // RECORD_MATCH

    unsigned int digital_speed = 127;

    while (true) {
        drive(erri(controller.get_digital(DIGITAL_A)),
            erri(controller.get_digital(DIGITAL_B)),
            erri(controller.get_digital(DIGITAL_X)),
            erri(controller.get_digital(DIGITAL_Y)),
            erri(controller.get_digital(DIGITAL_UP)),
            erri(controller.get_digital(DIGITAL_DOWN)),
            erri(controller.get_digital(DIGITAL_LEFT)),
            erri(controller.get_digital(DIGITAL_RIGHT)),
            erri(controller.get_digital(DIGITAL_L1)),
            erri(controller.get_digital(DIGITAL_L2)),
            erri(controller.get_digital(DIGITAL_R1)),
            erri(controller.get_digital(DIGITAL_R2)),
            erri(controller.get_analog(ANALOG_LEFT_X)),
            erri(controller.get_analog(ANALOG_LEFT_Y)),
            erri(controller.get_analog(ANALOG_RIGHT_X)),
            erri(controller.get_analog(ANALOG_RIGHT_Y)),
            &digital_speed
        );

        #ifdef RECORD_MATCH
        serialize_controller_state(outf,
            erri(controller.get_digital(DIGITAL_A)),
            erri(controller.get_digital(DIGITAL_B)),
            erri(controller.get_digital(DIGITAL_X)),
            erri(controller.get_digital(DIGITAL_Y)),
            erri(controller.get_digital(DIGITAL_UP)),
            erri(controller.get_digital(DIGITAL_DOWN)),
            erri(controller.get_digital(DIGITAL_LEFT)),
            erri(controller.get_digital(DIGITAL_RIGHT)),
            erri(controller.get_digital(DIGITAL_L1)),
            erri(controller.get_digital(DIGITAL_L2)),
            erri(controller.get_digital(DIGITAL_R1)),
            erri(controller.get_digital(DIGITAL_R2)),
            erri(controller.get_analog(ANALOG_LEFT_X)),
            erri(controller.get_analog(ANALOG_LEFT_Y)),
            erri(controller.get_analog(ANALOG_RIGHT_X)),
            erri(controller.get_analog(ANALOG_RIGHT_Y))
        );
        #endif // RECORD_MATCH
        delay(20);
    }
}
