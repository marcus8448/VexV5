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
#include "robot/controller/operator.hpp"
#include "robot/robot.hpp"

#ifdef SERIAL_LINK
#include "serial/serial.hpp"
#include "serial_plugins.hpp"
#endif

#ifdef RECORD_MATCH
#include "robot/controller/recording.hpp"
#endif

#if defined REPLAY_MATCH || defined TODO
#include "robot/controller/replay.hpp"
#endif

#ifdef RESET_POSITIONS
#include "reset.hpp"
#endif

#ifdef SCREEN
#include "screen/autonomous_select.hpp"
#include "screen/drivetrain_chart.hpp"
#include "screen/flywheel_chart.hpp"
#include "screen/info.hpp"
#include "screen/logs.hpp"
#include "screen/screen.hpp"
#endif

robot::Robot *get_robot();

[[noreturn]] void main_loop(robot::Robot *rbt) {
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
  robot::Robot *bot = get_robot();
#ifdef SCREEN
  screen::add_screen(new screen::Information());
  screen::add_screen(new screen::DrivetrainChart());
  screen::add_screen(new screen::FlywheelChart());
  screen::add_screen(new screen::Logging());
  screen::initialize(bot);
#endif // SCREEN
#ifdef SERIAL_LINK
  serial::add_plugin(new serial::RobotStatePlugin(bot));
  serial::add_plugin(new serial::RobotCommandsPlugin(bot));
  serial::initialize();
#endif // SERIAL_LINK
  logger::pop_section();
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 */
void autonomous() {
  logger::push_section("Autonomous Setup");
  robot::Robot *bot = get_robot();
#ifdef REPLAY_MATCH
  println("Replay match");
  bot->controller = new ReplayController();
#elif defined TODO
  println("Autonomous: TODO");
  bot->controller = new ReplayController("test");
#endif
  logger::pop_section();
  main_loop(bot);
}

/**
 * Called when the robot is under driver control.
 * Will delegate to autonomous control if the "Force Autonomous" button is
 * pressed.
 */
void opcontrol() {
  logger::push_section("Opcontrol Setup");
  robot::Robot *bot = get_robot();
#ifdef RECORD_MATCH
  logger::info("Recording controller");
  bot->controller = new RecordingController();
#else
  logger::info("Normal controller");
  bot->controller = new robot::controller::OpController();
#endif
  logger::pop_section();

#ifdef RESET_POSITIONS
  reset_positions(bot);
#else
  main_loop(bot);
#endif
}

robot::Robot *get_robot() {
  static robot::Robot *robot = nullptr;
  if (robot == nullptr) {
    robot = new robot::Robot(
        new robot::Drivetrain(new pros::Motor(RIGHT_FRONT_MOTOR, DRIVETRAIN_GEARSET, false, ENCODER_UNITS),
                              new pros::Motor(LEFT_FRONT_MOTOR, DRIVETRAIN_GEARSET, true, ENCODER_UNITS),
                              new pros::Motor(RIGHT_BACK_MOTOR, DRIVETRAIN_GEARSET, false, ENCODER_UNITS),
                              new pros::Motor(LEFT_BACK_MOTOR, DRIVETRAIN_GEARSET, true, ENCODER_UNITS)),
        new robot::Flywheel(new pros::Motor(FLYWHEEL_MOTOR, FLYWHEEL_GEARSET, false, ENCODER_UNITS)));
  }
  return robot;
}

void competition_initialize() {}
void disabled() {}
