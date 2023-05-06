#include "main.hpp"
#include "control/input/operator.hpp"
#include "debug/logger.hpp"
#include "pros/rtos.hpp"
#include "robot/device/motor.hpp"

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
void pros_task_begin();

/**
 * Called when the robot is first initialized.
 */
void initialize() {
  pros_task_begin();
  section_push("Initialize");
  section_push("Initialize robot");
  Robot &robot = getRobot();
  section_pop();
#ifndef DISABLE_SERIAL
  logger::warn("Initializing serial connection...");
  serial::add_plugin(7, new serial::RobotStatePlugin(robot));
  serial::add_plugin(8, new serial::RobotCommandsPlugin(robot));
  serial::initialize();
#endif // DISABLE_SERIAL
  // Optionally disable autonomous for builds
#ifndef DISABLE_AUTONOMOUS
  // Register the different types of autonomous-es
  control::autonomous::register_autonomous(new control::autonomous::None());
  control::autonomous::register_autonomous(new control::autonomous::LeftWinpoint());
  control::autonomous::register_autonomous(new control::autonomous::RightWinpoint());
  control::autonomous::register_autonomous(new control::autonomous::Skills());
#endif
  // Optionally enable extra screen functionality
#ifndef DISABLE_SCREEN
  section_push("Register Screens");
  // Optionally register the different screens
#if not defined(DISABLE_AUTONOMOUS) and not defined(DISABLE_AUTONOMOUS_SELECTION_SCREEN)
  screen::add_screen(new screen::AutonomousSelect());
#endif
#ifndef DISABLE_CONFIG_SCREEN
  screen::add_screen(new screen::ConfigurationScreen());
#endif
  screen::add_screen(new screen::Information());
#ifndef DISABLE_DRIVETRAIN_DEBUG_SCREEN
  screen::add_screen(new screen::DrivetrainChart());
#endif
  section_swap("Initialize Screen");
  screen::initialize(robot); // initialize the screen
  section_pop();
#endif // DISABLE_SCREEN
  robot.drivetrain.imu.calibrate();
  section_pop();
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 */
void autonomous() {
  pros_task_begin();
#ifndef DISABLE_AUTONOMOUS
  Robot &robot = getRobot();
  section_push("Autonomous Setup");
  //  autonomous::set_active(new std::string("Right Winpoint"));
  section_pop();

  robot.run_autonomous();
#else
  error("Autonomous is disabled");
#endif
}

/**
 * Called when the robot is under driver control.
 * Will delegate to autonomous control if the "Force Autonomous" button is
 * pressed.
 */
void opcontrol() {
  pros_task_begin();
  Robot &robot = getRobot();

  section_push("Opcontrol Setup");
  robot.set_controller(new control::input::Operator()); // set the robot controller to the default operator based one
  section_pop();

  robot.opcontrol();
}

/**
 * Called when the robot is at an official competition.
 */
void competition_initialize() { pros_task_begin(); }

/**
 * Called when the robot should be stopped during a competition
 */
void disabled() { pros_task_begin(); }

void pros_task_begin() {
  logger::flush();
  logger::initialize(pros::c::task_get_name(pros::c::task_get_current()));
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
