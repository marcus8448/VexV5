#include "pros/misc.hpp"
#include "vexv5/autonomous.hpp"
#include "vexv5/debug.hpp"
#include <fstream>
#include <filesystem>

#ifdef RECORD_MATCH
#include "pros/rtos.hpp"
#include "vexv5/debug.hpp"
#include "vexv5/filesystem.hpp"
#include "vexv5/globals.hpp"
#include "vexv5/recording.hpp"
#include <filesystem>

static std::ofstream outf;
#endif // RECORD_MATCH

#ifdef REPLAY_MATCH
#include "vexv5/replay.hpp"
#endif // REPLAY_MATCH

#ifdef RESET_POSITIONS
#include "vexv5/reset.hpp"
#endif // RESET_POSITIONS


void select_autonomous() {
    #ifdef REPLAY_MATCH
    print("RPM");
    replay_match();
    #endif
    
    #ifdef RIGHT_SIDE_WINPOINT
    print("RWP");
    right_side_winpoint();
    #endif // RIGHT_SIDE_WINPOINT
    #ifdef LEFT_SIDE_WINPOINT
    print("LWP");
    left_side_winpoint();
    #endif // LEFT_SIDE_WINPOINT
    #ifdef MIDDLE_RIGHT_GOAL
    print("MRG");
    middle_right_goal();
    #endif // MIDDLE_RIGHT_GOAL
    #ifdef MIDDLE_LEFT_GOAL
    print("MLG");
    middle_left_goal();
    #endif // MIDDLE_LEFT_GOAL
    print("END_AUTO");
}

int call_reset_positions() {
    #ifdef RESET_POSITIONS
    reset_positions();
    return 1;
    #else
    return 0;
    #endif
}

std::ofstream* create_record_stream() {
    #ifdef RECORD_MATCH
    print("GEN_OFSTREAM");
    while (!pros::usd::is_installed()) {
        controller.set_text(2, 0, "Missing microSD!");
        print("Missing microSD!");
        delay(500);
    }
    controller.clear_line(2);
    if (file_exists("/usd/record.v5r")) {
        bool moved = false;
        for (int i = 0; i < 50; i++) {
            std::string filename = std::string("/usd/record_").append(std::to_string(i)).append(".v5r");
            if (!file_exists(filename.c_str())) {
                std::ofstream out(filename, std::ios::out | std::ios::binary | std::ios::trunc);
                std::ifstream in("/usd/record.v5r", std::ios::in | std::ios::binary);
                std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()); // apparently rename/copy is not supported
                out << str;
                out.close(); 
                in.close();
                moved = true;
                break;
            }
        }
        if (!moved) {
            controller.set_text(2, 0, "Too many files!");
            return nullptr;
        }
    }
    controller.clear_line(2);
    outf = std::ofstream("/usd/record.v5r", std::ios::out | std::ios::binary | std::ios::trunc);
    return &outf;
    #else
    return nullptr;
    #endif
}

void call_serialize_controller_state(std::ofstream* outf, pros::Controller controller) {
    #ifdef RECORD_MATCH
    serialize_controller_state(outf,
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)),
            erri(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X)),
            erri(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)),
            erri(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)),
            erri(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)));
    #endif
}
