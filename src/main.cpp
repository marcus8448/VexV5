// CONFIG
#define FLYWHEEL_MOTOR 3
#define FLYWHEEL_SECONDARY_MOTOR 5
#define INDEXER_MOTOR 19
#define INTAKE_MOTOR 1
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
#include "robot/controller/autonomous_recording.hpp"

#ifdef ENABLE_AUTONOMOUS
#include "robot/autonomous/autonomous.hpp"
#include "robot/autonomous/left_skills.hpp"
#include "robot/autonomous/left_winpoint.hpp"
#include "robot/autonomous/none.hpp"
#include "robot/autonomous/right_skills.hpp"
#include "robot/autonomous/right_winpoint.hpp"
#endif

#include "pros/apix.h"
#include "pros/rtos.hpp"

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
  section_push("Initialize");
  section_push("Initialize robot");
  Robot &robot = get_or_create_robot();
  section_pop();
#ifdef SERIAL_LINK
  logger::warn("Initializing serial connection...");
  serial::add_plugin(7, new serial::RobotStatePlugin(robot));
  serial::add_plugin(8, new serial::RobotCommandsPlugin(robot));
  serial::initialize();
#endif // SERIAL_LINK
  // Optionally disable autonomous for builds
#ifdef ENABLE_AUTONOMOUS
  // Register the different types of autonomous-es
  autonomous::register_autonomous(new autonomous::None());
  autonomous::register_autonomous(new autonomous::LeftWinpoint());
  autonomous::register_autonomous(new autonomous::RightWinpoint());
//  autonomous::register_autonomous(new autonomous::LeftSkills());
//  autonomous::register_autonomous(new autonomous::RightSkills());
#endif
  // Optionally enable extra screen functionality
#ifdef SCREEN
  section_push("Register Screens");
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
  section_swap("Initialize Screen");
  screen::initialize(robot); // initialize the screen
  section_pop();
#endif // SCREEN
  section_pop();
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 */
void autonomous() {
#ifdef ENABLE_AUTONOMOUS
  Robot &robot = get_or_create_robot();
  section_push("Autonomous Setup");
  autonomous::set_active(new std::string("Right Winpoint"));
  autonomous::Autonomous *autonomous = autonomous::get_autonomous();
  section_pop();
  if (autonomous != nullptr) {
    // #ifdef SCREEN
    // if (screen::autonomous_select_instance != nullptr) {
    //   screen::remove_screen(screen::autonomous_select_instance);
    //   screen::autonomous_select_instance = nullptr;
    // }
    // #endif
    autonomous->run(robot); // run the autonomous code
  } else {
    error("Missing autonomous run!");
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

  section_push("Opcontrol Setup");
  robot.controller = new controller::OpController(); // set the robot controller to the default operator based one.
  section_pop();

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
                             new Intake(INTAKE_MOTOR), new Indexer(INDEXER_MOTOR),
                             new Flywheel(FLYWHEEL_MOTOR, FLYWHEEL_SECONDARY_MOTOR), new Expansion(EXPANSION_PISTON));
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
void disabled() { logger::flush(); }
