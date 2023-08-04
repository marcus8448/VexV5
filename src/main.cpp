#include "main.hpp"
#include "control/input/operator.hpp"
#include "debug/logger.hpp"
#include "pros/apix.h"
#include "robot/robot.hpp"
#include "tasks.hpp"

#ifndef DISABLE_AUTONOMOUS
#include "control/autonomous/autonomous.hpp"
#include "control/autonomous/score.hpp"
#include "control/autonomous/skills.hpp"
#include "control/autonomous/winpoint.hpp"
#endif

#ifndef DISABLE_SERIAL
#include "serial/robot_command.hpp"
#include "serial/robot_state.hpp"
#include "serial/serial.hpp"
#endif

#ifndef DISABLE_SCREEN
#include "screen/screen.hpp"
#ifndef DISABLE_CONFIG_SCREEN
#include "screen/config_screen.hpp"
#endif
#if not defined(DISABLE_AUTONOMOUS) and not defined(DISABLE_AUTONOMOUS_SELECTION_SCREEN)
#include "screen/autonomous_select.hpp"
#endif
#ifndef DISABLE_DRIVETRAIN_DEBUG_SCREEN
#include "screen/drivetrain_chart.hpp"
#endif
#include "screen/information.hpp"
#endif

// CONFIG
#define DRIVETRAIN_LEFT_FRONT_MOTOR 9
#define DRIVETRAIN_RIGHT_FRONT_MOTOR 2
#define DRIVETRAIN_LEFT_BACK_MOTOR 10
#define DRIVETRAIN_RIGHT_BACK_MOTOR 1
#define INTAKE_RIGHT_MOTOR 12
#define INTAKE_LEFT_MOTOR 11
#define INERTIAL 7
#define ARM_1 19
#define ARM_2 20
// END CONFIG

using namespace robot;

robot::Robot &getRobot();

/**
 * Called when the robot is first initialized.
 */
void initialize() {
//  pros::c::serctl(SERCTL_DISABLE_COBS, nullptr);
  onRootTaskStart();
  scopePush("Initialize");
  scopePush("Initialize robot");
  Robot &robot = getRobot();
  scopePop();
#ifndef DISABLE_SERIAL
  logger::warn("Initializing serial connection...");
  serial::add_plugin(7, new serial::RobotStatePlugin(robot));
  serial::add_plugin(8, new serial::RobotCommandsPlugin(robot));
  serial::initialize();
#endif // DISABLE_SERIAL
  // Optionally disable autonomous for builds
#ifndef DISABLE_AUTONOMOUS
  // Register the different types of autonomous-es
  control::autonomous::initialize();
  control::autonomous::registerRun("Right Winpoint", control::autonomous::rightWinpoint);
  control::autonomous::registerRun("Left Winpoint", control::autonomous::leftWinpoint);
  control::autonomous::registerRun("Right Score", control::autonomous::rightScore);
  control::autonomous::registerRun("Left Score", control::autonomous::leftScore);
  control::autonomous::registerRun("Skills", control::autonomous::skills);
#endif
  // Optionally enable extra screen functionality
#ifndef DISABLE_SCREEN
  scopePush("Register Screens");
  // Optionally register the different screens
//#if not defined(DISABLE_AUTONOMOUS) and not defined(DISABLE_AUTONOMOUS_SELECTION_SCREEN)
//  screen::addScreen(new screen::AutonomousSelect(robot));
//#endif
//#ifndef DISABLE_CONFIG_SCREEN
//  screen::addScreen(new screen::ConfigurationScreen(robot));
//#endif
//  screen::addScreen(new screen::Information(robot));
//#ifndef DISABLE_DRIVETRAIN_DEBUG_SCREEN
//  screen::addScreen(new screen::DrivetrainChart(robot));
//#endif
  section_swap("Initialize Screen");
  screen::initialize(); // initialize the screen
  scopePop();
#endif // DISABLE_SCREEN
  robot.drivetrain.imu.calibrate();
  scopePop();
  onRootTaskEnd();
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 */
void autonomous() {
  onRootTaskStart();
#ifndef DISABLE_AUTONOMOUS
  Robot &robot = getRobot();
  scopePush("Autonomous Setup");
  //  autonomous::set_active(new std::string("Right Winpoint"));
  scopePop();

  robot.runAutonomous();
#else
  error("Autonomous is disabled");
#endif
  onRootTaskEnd();
}

/**
 * Called when the robot is under driver control.
 * Will delegate to autonomous control if the "Force Autonomous" button is
 * pressed.
 */
void opcontrol() {
#ifdef ENABLE_TEMPORARY_CODE
  autonomous();
#endif

  onRootTaskStart();
  Robot &robot = getRobot();

  scopePush("Opcontrol Setup");
  robot.setController(new control::input::Operator()); // set the robot controller to the default operator based one
  scopePop();

  robot.opcontrol();
  onRootTaskEnd();
}

/**
 * Called when the robot is at an official competition.
 */
void competition_initialize() {
  onRootTaskStart();
  onRootTaskEnd();
}

/**
 * Called when the robot should be stopped during a competition
 */
void disabled() {
  onRootTaskStart();
  onRootTaskEnd();
}

/**
 * Returns the current robot instance, or creates one if it has not been made yet.
 * @return the robot instance
 */
Robot &getRobot() {
  static Robot robot =
      Robot(DRIVETRAIN_LEFT_FRONT_MOTOR, DRIVETRAIN_RIGHT_FRONT_MOTOR, DRIVETRAIN_LEFT_BACK_MOTOR,
            DRIVETRAIN_RIGHT_BACK_MOTOR, INTAKE_RIGHT_MOTOR, INTAKE_LEFT_MOTOR, INERTIAL, ARM_1, ARM_2);
  device::initialize();
  return robot;
}
