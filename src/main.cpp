#include "main.hpp"
#include "control/input/operator.hpp"
#include "debug/logger.hpp"
#include "robot/device/motor.hpp"
#include "pros/rtos.h"

#ifndef DISABLE_AUTONOMOUS
#include "control/autonomous/autonomous.hpp"
#include "control/autonomous/left_winpoint.hpp"
#include "control/autonomous/none.hpp"
#include "control/autonomous/right_winpoint.hpp"
#include "control/autonomous/skills.hpp"
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
#define RIGHT_FRONT_MOTOR 4
#define LEFT_FRONT_MOTOR 2
#define RIGHT_BACK_MOTOR 9
#define LEFT_BACK_MOTOR 20
#define INERTIAL 18
// END CONFIG

using namespace robot;

robot::Robot &getRobot();
void onRootTaskStart();
void onRootTaskEnd();

/**
 * Called when the robot is first initialized.
 */
void initialize() {
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
  control::autonomous::registerRun(new control::autonomous::None());
  control::autonomous::registerRun(new control::autonomous::LeftWinpoint());
  control::autonomous::registerRun(new control::autonomous::RightWinpoint());
  control::autonomous::registerRun(new control::autonomous::Skills());
#endif
  // Optionally enable extra screen functionality
#ifndef DISABLE_SCREEN
  scopePush("Register Screens");
  // Optionally register the different screens
#if not defined(DISABLE_AUTONOMOUS) and not defined(DISABLE_AUTONOMOUS_SELECTION_SCREEN)
  screen::addScreen(new screen::AutonomousSelect());
#endif
#ifndef DISABLE_CONFIG_SCREEN
  screen::addScreen(new screen::ConfigurationScreen());
#endif
  screen::addScreen(new screen::Information());
#ifndef DISABLE_DRIVETRAIN_DEBUG_SCREEN
  screen::addScreen(new screen::DrivetrainChart());
#endif
  section_swap("Initialize Screen");
  screen::initialize(robot); // initialize the screen
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

void onRootTaskStart() {
  logger::flush();
  logger::initialize(pros::c::task_get_name(pros::c::task_get_current()));
}

void onRootTaskEnd() {
  logger::clearRoot(pros::c::task_get_name(pros::c::task_get_current()));
  logger::flush();
}

/**
 * Returns the current robot instance, or creates one if it has not been made yet.
 * @return the robot instance
 */
Robot &getRobot() {
  static Robot robot = Robot(RIGHT_FRONT_MOTOR, LEFT_FRONT_MOTOR, RIGHT_BACK_MOTOR, LEFT_BACK_MOTOR, INERTIAL);
  device::initialize();
  return robot;
}
