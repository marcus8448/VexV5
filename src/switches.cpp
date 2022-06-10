#include "pros/misc.hpp"
#include "vexv5/autonomous.hpp"
#include "vexv5/debug.hpp"
#include "vexv5/robot.hpp"
#include <fstream>
#include <filesystem>

#ifdef RECORD_MATCH
#include "pros/rtos.hpp"
#include "vexv5/debug.hpp"
#include "vexv5/filesystem.hpp"
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
    print("BEGIN AUTO");
    Robot robot = Robot();
    
    #ifdef REPLAY_MATCH
    print("REPLAY_MATCH");
    replay_match(&robot);
    #endif

    print("END AUTO");
}

int call_reset_positions() {
    #ifdef RESET_POSITIONS
    reset_positions(Robot());
    return 1;
    #else
    return 0;
    #endif
}

std::ofstream* create_record_stream() {
    #ifdef RECORD_MATCH
    print("GENERATE OFSTREAM");
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
    print("GENERATED OFSTREAM");
    return &outf;
    #else
    return nullptr;
    #endif
}

void call_serialize_controller_state(std::ofstream* outf, pros::Controller controller) {
    #ifdef RECORD_MATCH
    serialize_controller_state(outf, controller);
    #endif
}
