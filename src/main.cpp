#include <cfloat>
#include <string>
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "vexv5/debug.hpp"
#include "vexv5/robot.hpp"
#include "vexv5/switches.hpp"

extern "C" {
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
}

/**
 * Called when the robot is first initialized.
 */
void initialize() {
    pros::Task::create(debug_input_task, "Debug Input Task");
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 * Or when the "Force Autonomous" button is presssed on the screen.
 */
void autonomous() {
    select_autonomous();
}

/**
 * Called when the robot is under driver control.
 * Will delegate to autonomous control if the "Force Autonomous" button is pressed.
 */
void opcontrol() {
    Robot robot = Robot();
    if (call_reset_positions()) {
        return;
    }

    std::ofstream* outf = create_record_stream();

    while (true) {
        robot.update();

        call_serialize_controller_state(outf, robot.controller);
        pros::delay(20);
    }
}
