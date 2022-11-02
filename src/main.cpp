// CONFIG
#define AUTONOMOUS

#define SCREEN
#define SCREEN_CONFIG
#define SCREEN_LOGGING
#define SCREEN_DRIVETRAIN
#define SCREEN_FLYWHEEL

// #define SERIAL_LINK
// END CONFIG

#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"

#include "constants.hpp"
#include "logger.hpp"
#include "main.hpp"
#include "robot/controller/operator.hpp"

#ifdef AUTONOMOUS
#include "robot/autonomous/autonomous.hpp"
#include "robot/autonomous/left_score.hpp"
#include "robot/autonomous/left_winpoint.hpp"
#include "robot/autonomous/none.hpp"
#include "robot/autonomous/right_score.hpp"
#include "robot/autonomous/right_winpoint.hpp"
#endif

#include "pros/apix.h"

#ifdef SERIAL_LINK
#include "serial/robot_command.hpp"
#include "serial/robot_state.hpp"
#include "serial/serial.hpp"
#endif

#ifdef SCREEN
#include "screen/screen.hpp"
#ifdef SCREEN_CONFIG
#include "screen/config_screen.hpp"
#endif
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

using namespace robot;

/**
 * Called when the robot is first initialized.
 */
void initialize() {
  logger::push("Initialize");
  Robot *robot = get_or_create_robot();
#ifdef SERIAL_LINK
  logger::warn("Initializing serial connection...");
  serial::add_plugin(6, new serial::RobotStatePlugin(robot));
  serial::add_plugin(7, new serial::RobotCommandsPlugin(robot));
  serial::initialize();
#endif // SERIAL_LINK
  // Optionally disable autonomous for builds
#ifdef AUTONOMOUS
  // Register the different types of autonomous-es
  autonomous::register_autonomous("None", new autonomous::None());
  autonomous::register_autonomous("Left Winpoint", new autonomous::LeftWinpoint());
  autonomous::register_autonomous("Right Winpoint", new autonomous::RightWinpoint());
  autonomous::register_autonomous("Left Score", new autonomous::LeftScore());
  autonomous::register_autonomous("Right Score", new autonomous::RightScore());
#endif
  // Optionally enable extra screen functionality
#ifdef SCREEN
  logger::push("Add Screens");
  // Optionally register the different screens
#ifdef AUTONOMOUS
  screen::add_screen(new screen::AutonomousSelect());
#endif
#ifdef SCREEN_CONFIG
  screen::add_screen(new screen::ConfigurationScreen());
#endif
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
  logger::pop_push("Initialize Screen");
  screen::initialize(robot); // initialize the screen
  logger::pop();
#endif // SCREEN
  logger::pop();
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 */
void autonomous() {
#ifdef AUTONOMOUS
  logger::push("Autonomous Setup");
  Robot *robot = get_or_create_robot();
  logger::pop();
  autonomous::set_active("Left Winpoint");
  autonomous::Autonomous *autonomous = autonomous::get_autonomous();
  if (autonomous != nullptr) {
    autonomous->run(robot); // run the autonomous code
  } else {
    logger::error("Missing autonomous run!");
  }
#endif
}

/**
 * Called when the robot is under driver control.
 * Will delegate to autonomous control if the "Force Autonomous" button is
 * pressed.
 */
void opcontrol() {
  logger::push("Opcontrol Setup");
  Robot *robot = get_or_create_robot();
  robot->controller = new controller::OpController(); // set the robot controller to the default operator based one.
  logger::pop();

  robot->expansion->charge();
  // infinitely run opcontrol - pros will kill the task when it's over.
  while (true) {
    robot->update();
    pros::delay(20);
  }
}

/**
 * Returns the current robot instance, or creates one if it has not been made yet.
 * @return the robot instance
 */
Robot *get_or_create_robot() {
  static Robot *robot = nullptr; // stored forever
  if (robot == nullptr) {        // check if robot exists
    // otherwise, create the robot
    robot = new Robot(new Drivetrain(RIGHT_FRONT_MOTOR, LEFT_FRONT_MOTOR, RIGHT_BACK_MOTOR, LEFT_BACK_MOTOR),
                      new Intake(INTAKE_MOTOR),
                      new Indexer(INDEXER_MOTOR),
                      new Flywheel(new pros::Motor(FLYWHEEL_MOTOR, FLYWHEEL_GEARSET, true, ENCODER_UNITS)),
                      new Expansion(EXPANSION_MOTOR));
  }
  return robot;
}

/**
 * Called when the robot is at an official competition.
 */
void competition_initialize() {}

/**
 * Called when the robot should be stopped during a competition
 */
void disabled() {}
