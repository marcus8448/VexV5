#include "debug.hpp"
#include "error.hpp"
#include "recording.hpp"
#include "replay.hpp"
#include "reset.hpp"
#include "robot.hpp"
#include "pros/rtos.hpp"
#include <string>

static Robot* robot = static_cast<Robot*>(malloc(sizeof(Robot)));

[[noreturn]] void main_loop(Robot* ptr) {
    while (true) {
        ptr->update();
        pros::delay(20);
    }
}

/**
 * Called when the robot is first initialized.
 */
[[maybe_unused]] void initialize() {
    robot = new Robot(Drivetrain(
            pros::Motor(10, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES),
            pros::Motor(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES),
            pros::Motor(20, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES),
            pros::Motor(11, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES)
                    ));
    pros::Task(debug_input_task, robot, "Debug Input Task");
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 */
[[maybe_unused]] void autonomous() {
    printf("BEGIN AUTO");
#ifdef REPLAY_MATCH
    printf("REPLAY_MATCH");
    robot->controller = new ReplayController();
#elif defined(TODO)
    printf("TODO");
    robot->controller = new ReplayController("test");
#endif
    printf("END AUTO");
    main_loop(robot);
}

/**
 * Called when the robot is under driver control.
 * Will delegate to autonomous control if the "Force Autonomous" button is pressed.
 */
[[maybe_unused]] void opcontrol() {
#ifdef RECORD_MATCH
    robot->controller = new RecordingController();
#else
    robot->controller = new OpController();
#endif

#ifdef RESET_POSITIONS
    reset_positions(robot);
#else
    main_loop(robot);
#endif
}
