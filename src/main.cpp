#include "debug.hpp"
#include "robot.hpp"
#include "util.hpp"
#include "pros/rtos.hpp"
#include <iostream>

#ifdef RECORD_MATCH
#include "recording.hpp"
#endif

#if defined REPLAY_MATCH || defined TODO
#include "replay.hpp"
#endif

#ifdef RESET_POSITIONS
#include "reset.hpp"
#endif

extern "C" {
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
}

static Robot* robot = nullptr;

void main_loop(Robot* robot) {
    println("Main loop");
    while (true) {
        robot->update();
        pros::delay(20);
    }
}

/**
 * Called when the robot is first initialized.
 */
void initialize() {
    print_section("Initialize");
    robot = new Robot(new Drivetrain(
            new pros::Motor(10, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES),
            new pros::Motor(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES),
            new pros::Motor(20, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES),
            new pros::Motor(11, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES)));
    pros::Task(debug_input_task, static_cast<void*>(robot), "Debug Input Task");
    print_section("End Initialize");
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 */
void autonomous() {
    print_section("Autonomous setup");
#ifdef REPLAY_MATCH
    println("Replay match");
    robot->controller = new ReplayController();
#elif defined(TODO)
    println("Autonomous: TODO");
    robot->controller = new ReplayController("test");
#endif
    print_section("End autonomous setup");
    main_loop(robot);
}

/**
 * Called when the robot is under driver control.
 * Will delegate to autonomous control if the "Force Autonomous" button is pressed.
 */
void opcontrol() {
    print_section("Opcontrol setup");
#ifdef RECORD_MATCH
    println("Recording");
    robot->controller = new RecordingController();
#else
    println("Normal controller");
    robot->controller = new OpController();
#endif

#ifdef RESET_POSITIONS
    print_section("Resetting positions");
    reset_positions(robot);
#else
    print_section("End opcontrol setup");
    main_loop(robot);
#endif
}
