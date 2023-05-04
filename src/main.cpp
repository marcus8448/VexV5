// CONFIG
#include "robot/device/motor.hpp"
#define RIGHT_FRONT_MOTOR 4
#define LEFT_FRONT_MOTOR 2
#define RIGHT_BACK_MOTOR 9
#define LEFT_BACK_MOTOR 20
#define INERTIAL 18

#define ENABLE_AUTONOMOUS

#define SCREEN
#define SCREEN_CONFIG
#define SCREEN_DRIVETRAIN
#define SCREEN_FLYWHEEL

// #define SERIAL_LINK
// END CONFIG

// #if __has_include("temporary.hpp")
#define ENABLE_TEMPORARY_CODE

// #ifdef ENABLE_TEMPORARY_CODE
#include "temporary.hpp"
// #endif
// #endif

#include "debug/logger.hpp"
#include "main.hpp"
#include "robot/controller/autonomous_recording.hpp"
#include "robot/controller/operator.hpp"

#ifdef ENABLE_AUTONOMOUS
#include "robot/autonomous/autonomous.hpp"
#include "robot/autonomous/left_winpoint.hpp"
#include "robot/autonomous/none.hpp"
#include "robot/autonomous/right_winpoint.hpp"
#include "robot/autonomous/skills.hpp"
#endif

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
#include "screen/information.hpp"
#endif

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
  autonomous::register_autonomous(new autonomous::Skills());
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
  section_swap("Initialize Screen");
  screen::initialize(robot); // initialize the screen
  section_pop();
#endif // SCREEN
  robot.drivetrain.imu.calibrate();
  section_pop();
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 */
void autonomous() {
  pros_task_begin();
#ifdef ENABLE_AUTONOMOUS
  Robot &robot = getRobot();
  section_push("Autonomous Setup");
  //  autonomous::set_active(new std::string("Right Winpoint"));
  section_pop();

  robot.run_autonomous();
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

#ifdef ENABLE_TEMPORARY_CODE
  if (temporary::run(robot))
    return;
#endif

  section_push("Opcontrol Setup");
  robot.set_controller(
      new robot::controller::Operator()); // set the robot controller to the default operator based one.
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
