// CONFIG
#define FLYWHEEL_MOTOR 3
#define FLYWHEEL_SECONDARY_MOTOR 5
#define INDEXER_MOTOR 19
#define INTAKE_MOTOR 1
#define ROLLER_COLOUR_SENSOR_UPPER 7
#define ROLLER_COLOUR_SENSOR_LOWER 6
#define EXPANSION_PISTON 'A'

#define RIGHT_FRONT_MOTOR 4
#define LEFT_FRONT_MOTOR 2
#define RIGHT_BACK_MOTOR 9
#define LEFT_BACK_MOTOR 20

#define ENABLE_AUTONOMOUS

#define SCREEN
#define SCREEN_CONFIG
#define SCREEN_DRIVETRAIN
#define SCREEN_FLYWHEEL

// #define SERIAL_LINK
// END CONFIG

#if __has_include("temporary.hpp")
// #define ENABLE_TEMPORARY_CODE

#ifdef ENABLE_TEMPORARY_CODE
#include "temporary.hpp"
#endif
#endif

#include "logger.hpp"
#include "main.hpp"
#include "robot/controller/operator.hpp"

#ifdef ENABLE_AUTONOMOUS
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
#ifdef ENABLE_AUTONOMOUS
#include "screen/autonomous_select.hpp"
#endif
#ifdef SCREEN_DRIVETRAIN
#include "screen/drivetrain_chart.hpp"
#endif
#ifdef SCREEN_FLYWHEEL
#include "screen/flywheel_chart.hpp"
#endif
#include "screen/information.hpp"
#endif

using namespace robot;

/**
 * Called when the robot is first initialized.
 */
void initialize() {
  logger::push("Initialize");
  logger::push("Initialize robot");
  Robot &robot = get_or_create_robot();
  logger::pop();
#ifdef SERIAL_LINK
  logger::warn("Initializing serial connection...");
  serial::add_plugin(7, new serial::RobotStatePlugin(robot));
  serial::add_plugin(8, new serial::RobotCommandsPlugin(robot));
  serial::initialize();
#endif // SERIAL_LINK
  // Optionally disable autonomous for builds
#ifdef ENABLE_AUTONOMOUS
  // Register the different types of autonomous-es
  autonomous::register_autonomous("None", new autonomous::None());
  autonomous::register_autonomous("Left Winpoint", new autonomous::LeftWinpoint());
  autonomous::register_autonomous("Right Winpoint", new autonomous::RightWinpoint());
//  autonomous::register_autonomous("Left Score", new autonomous::LeftScore());
//  autonomous::register_autonomous("Right Score", new autonomous::RightScore());
#endif
  // Optionally enable extra screen functionality
#ifdef SCREEN
  logger::push("Register Screens");
  // Optionally register the different screens
#ifdef ENABLE_AUTONOMOUS
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
#ifdef ENABLE_AUTONOMOUS
  Robot &robot = get_or_create_robot();
  logger::push("Autonomous Setup");
  //  autonomous::set_active(new std::string("Left Winpoint"));
  autonomous::Autonomous *autonomous = autonomous::get_autonomous();
  logger::pop();
  if (autonomous != nullptr) {
    // #ifdef SCREEN
    // if (screen::autonomous_select_instance != nullptr) {
    //   screen::remove_screen(screen::autonomous_select_instance);
    //   screen::autonomous_select_instance = nullptr;
    // }
    // #endif
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
  Robot &robot = get_or_create_robot();
  // #if defined(ENABLE_AUTONOMOUS) && defined(SCREEN) // todo: fix race condition
  //   if (screen::autonomous_select_instance != nullptr) {
  //     screen::remove_screen(screen::autonomous_select_instance);
  //     screen::autonomous_select_instance = nullptr;
  //   }
  // #endif

  logger::push("Opcontrol Setup");
  robot.controller = new controller::OpController(); // set the robot controller to the default operator based one.
  logger::pop();

#ifdef ENABLE_TEMPORARY_CODE
  if (temporary::run(robot))
    return;
#endif

  // infinitely run opcontrol - pros will kill the task when it's over.
  while (true) {
    robot.update();
    pros::delay(20);
  }
}

/**
 * Returns the current robot instance, or creates one if it has not been made yet.
 * @return the robot instance
 */
Robot &get_or_create_robot() {
  static Robot robot = Robot(new Drivetrain(RIGHT_FRONT_MOTOR, LEFT_FRONT_MOTOR, RIGHT_BACK_MOTOR, LEFT_BACK_MOTOR),
                             new Intake(INTAKE_MOTOR, ROLLER_COLOUR_SENSOR_UPPER, ROLLER_COLOUR_SENSOR_LOWER),
                             new Indexer(INDEXER_MOTOR), new Flywheel(FLYWHEEL_MOTOR, FLYWHEEL_SECONDARY_MOTOR),
                             new Expansion(EXPANSION_PISTON));
  device::initialize();
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
