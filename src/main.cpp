#include "robot/intake.hpp"
#include "screen/config.hpp"
extern "C" {
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
}

// CONFIG
#define AUTONOMOUS
#define SCREEN
#define SCREEN_LOGGING
#define SCREEN_DRIVETRAIN
#define SCREEN_FLYWHEEL
#define DEBUG_LOG
// END CONFIG

#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"

#include "constants.hpp"
#include "logger.hpp"
#include "robot/controller/operator.hpp"
#include "robot/robot.hpp"

#ifdef AUTONOMOUS
#include "robot/autonomous/autonomous.hpp"
#include "robot/autonomous/left_winpoint.hpp"
#include "robot/autonomous/right_winpoint.hpp"
#include "robot/autonomous/left_score.hpp"
#include "robot/autonomous/right_score.hpp"
#endif

#ifdef SERIAL_LINK
#include "serial/serial.hpp"
#include "serial_plugins.hpp"
#endif

#ifdef SCREEN
#include "screen/screen.hpp"
#ifdef AUTONOMOUS
#include "screen/autonomous_select.hpp"
#endif
#ifdef SCREEN_DRIVETRAIN
#include "screen/drivetrain_chart.hpp"
#endif
#ifdef SCREEN_FLYWHEEL
#include "screen/flywheel_chart.hpp"
#endif
#include "screen/info.hpp"
#ifdef SCREEN_LOGGING
#include "screen/logging.hpp"
#endif
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
  robot::Robot *robot = get_robot();
#ifdef AUTONOMOUS
robot::autonomous::register_autonomous(new robot::autonomous::LeftWinpoint());
robot::autonomous::register_autonomous(new robot::autonomous::RightWinpoint());
robot::autonomous::register_autonomous(new robot::autonomous::LeftScore());
robot::autonomous::register_autonomous(new robot::autonomous::RightScore());
#endif
#ifdef SCREEN
  logger::push_section("Add Screens");
#ifdef AUTONOMOUS
  screen::add_screen(new screen::AutonomousSelect());
#endif
  screen::add_screen(new screen::Configuration());
  screen::add_screen(new screen::Information());
#ifdef SCREEN_DRIVETRAIN
  screen::add_screen(new screen::DrivetrainChart());
#endif
#ifdef SCREEN_FLYWHEEL
  screen::add_screen(new screen::FlywheelChart());
#endif
#ifdef SCREEN_LOGGING
  screen::add_screen(new screen::Logging());
#endif
  logger::swap_section("Initialize Screen");
  screen::initialize(robot);
  logger::pop_section();
#endif // SCREEN
#ifdef SERIAL_LINK
  serial::add_plugin(new serial::RobotStatePlugin(robot));
  serial::add_plugin(new serial::RobotCommandsPlugin(robot));
  serial::initialize();
#endif // SERIAL_LINK
  logger::pop_section();
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 */
void autonomous() {
#ifdef AUTONOMOUS
  logger::push_section("Autonomous Setup");
  robot::Robot *robot = get_robot();
  logger::pop_section();
  robot::autonomous::run(robot);
  main_loop(robot);
#endif
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
        new robot::Intake(new pros::Motor(INTAKE_MOTOR, INTAKE_GEARSET, true, ENCODER_UNITS)),
        new robot::Flywheel(new pros::Motor(FLYWHEEL_MOTOR, FLYWHEEL_GEARSET, true, ENCODER_UNITS)));
  }
  return robot;
}

void competition_initialize() {}
void disabled() {}
