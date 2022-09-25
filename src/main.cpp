extern "C" {
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
}

#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"

#include "config.hpp"
#include "constants.hpp"
#include "logger.hpp"
#include "robot.hpp"

#ifdef SERIAL_LINK
#include "seriallink/seriallink.hpp"
#include "seriallink/plugins.hpp"
#endif

#ifdef RECORD_MATCH
#include "recording.hpp"
#endif

#if defined REPLAY_MATCH || defined TODO
#include "replay.hpp"
#endif

#ifdef RESET_POSITIONS
#include "reset.hpp"
#endif

#ifdef SCREEN
#include "screen.hpp"
#endif

static Robot *robot = nullptr;

[[noreturn]] void main_loop(Robot *rbt) {
  while (true) {
    rbt->update();
    pros::delay(20);
  }
}

/**
 * Called when the robot is first initialized.
 */
void initialize() {
  logger::push_section("Initialize");
  robot = new Robot(
      new Drivetrain(
          new pros::Motor(RIGHT_FRONT_MOTOR, DRIVETRAIN_GEARSET, false, ENCODER_UNITS),
          new pros::Motor(LEFT_FRONT_MOTOR, DRIVETRAIN_GEARSET, true, ENCODER_UNITS),
          new pros::Motor(RIGHT_BACK_MOTOR, DRIVETRAIN_GEARSET, false, ENCODER_UNITS),
          new pros::Motor(LEFT_BACK_MOTOR, DRIVETRAIN_GEARSET, true, ENCODER_UNITS)
          ),
          new Flywheel(
              new pros::Motor(FLYWHEEL_MOTOR, FLYWHEEL_GEARSET, false, ENCODER_UNITS)
              ));
#ifdef SERIAL_LINK
  seriallink::add_plugin(new seriallink::RobotStatePlugin(robot));
  seriallink::add_plugin(new seriallink::RobotCommandsPlugin(robot));
  seriallink::create_debug_task();
#endif //SERIAL_LINK
#ifdef SCREEN
  screen::initialize(robot);
#endif //SCREEN
  logger::pop_section();
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 */
void autonomous() {
  logger::push_section("Autonomous Setup");
#ifdef REPLAY_MATCH
  println("Replay match");
  robot->controller = new ReplayController();
#elif defined TODO
  println("Autonomous: TODO");
  robot->controller = new ReplayController("test");
#endif
  logger::pop_section();
  main_loop(robot);
}

/**
 * Called when the robot is under driver control.
 * Will delegate to autonomous control if the "Force Autonomous" button is pressed.
 */
void opcontrol() {
  if (robot == nullptr) {
    initialize();
  }
  logger::push_section("Opcontrol Setup");
#ifdef RECORD_MATCH
  logger::info("Recording controller");
  robot->controller = new RecordingController();
#else
  logger::info("Normal controller");
  robot->controller = new OpController();
#endif
  logger::pop_section();

#ifdef RESET_POSITIONS
  reset_positions(robot);
#else
  main_loop(robot);
#endif
}

void competition_initialize() {}
void disabled() {}
