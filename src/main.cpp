#include "main.hpp"
#include "control/input/operator.hpp"
#include "debug/logger.hpp"
#include "robot/robot.hpp"
#include "tasks.hpp"

#ifndef DISABLE_AUTONOMOUS
#include "control/autonomous/autonomous.hpp"
#include "control/autonomous/score.hpp"
#include "control/autonomous/skills.hpp"
#include "control/autonomous/winpoint.hpp"
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
#include "screen/chart.hpp"
#include "screen/colour.hpp"
#endif
#include "screen/information.hpp"
#include "screen/pid_tuning.hpp"
#endif

// CONFIG
#define DRIVETRAIN_MOTOR_L1 2
#define DRIVETRAIN_MOTOR_L2 12
#define DRIVETRAIN_MOTOR_L3 13
#define DRIVETRAIN_MOTOR_R1 10
#define DRIVETRAIN_MOTOR_R2 20
#define DRIVETRAIN_MOTOR_R3 19
#define DRIVETRAIN_IMU 7
#define INTAKE_RIGHT_MOTOR 12
#define INTAKE_LEFT_MOTOR 11
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
  rtos::onRootTaskStart();
  logger::scope("Initialize");
  logger::scope("Initialize robot");
  Robot &robot = getRobot();
  logger::endScope();
  // Optionally disable autonomous for builds
#ifndef DISABLE_AUTONOMOUS
  // Register the different types of autonomous-es
  control::autonomous::registerRun("Right Winpoint", control::autonomous::rightWinpoint);
  control::autonomous::registerRun("Left Winpoint", control::autonomous::leftWinpoint);
  control::autonomous::registerRun("Right Score", control::autonomous::rightScore);
  control::autonomous::registerRun("Left Score", control::autonomous::leftScore);
  control::autonomous::registerRun("Skills", control::autonomous::skills);
  control::autonomous::registerRun("!PidTuning", [](robot::Robot &robot) {
    robot.drivetrain.tare();
    robot.drivetrain.forwards(24.0, true);
    robot.drivetrain.backwards(24.0, true);
  });
  control::autonomous::initialize();
#endif
  // Optionally enable extra screen functionality
#ifndef DISABLE_SCREEN
  logger::scope("Register Screens");
  // Optionally register the different screens
#if not defined(DISABLE_AUTONOMOUS) and not defined(DISABLE_AUTONOMOUS_SELECTION_SCREEN)
  screen::addScreen(std::make_unique<screen::AutonomousSelect, robot::Robot &, lv_obj_t *, lv_coord_t, lv_coord_t>);
#endif
#ifndef DISABLE_CONFIG_SCREEN
  screen::addScreen(std::make_unique<screen::ConfigurationScreen, robot::Robot &, lv_obj_t *, lv_coord_t, lv_coord_t>);
#endif
  screen::addScreen(std::make_unique<screen::Information, robot::Robot &, lv_obj_t *, lv_coord_t, lv_coord_t>);
#ifndef DISABLE_DRIVETRAIN_DEBUG_SCREEN
  screen::addScreen([](robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
    return std::make_unique<screen::Chart<2, 100>>(
        robot, screen, width, height, "Drivetrain Velocity",
        std::array<screen::DataSet, 2>{
            screen::DataSet("Left", screen::colour::RED,
                            [](robot::Robot &robot) { return static_cast<float>(robot.drivetrain.getLeftVelocity()); }),
            screen::DataSet("Right", screen::colour::GREEN, [](robot::Robot &robot) {
              return static_cast<float>(robot.drivetrain.getRightVelocity());
            })});
  });
  screen::addScreen([](robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
    return std::make_unique<screen::Chart<2, 100>>(
        robot, screen, width, height, "Drivetrain PID Error",
        std::array<screen::DataSet, 2>{screen::DataSet("Left", screen::colour::LIGHT_BLUE,
                                                       [](robot::Robot &robot) {
                                                         return static_cast<float>(robot.drivetrain.leftPID.getError());
                                                       }),
                                       screen::DataSet("Right", screen::colour::PINK, [](robot::Robot &robot) {
                                         return static_cast<float>(robot.drivetrain.rightPID.getError());
                                       })});
  });
#endif
  screen::addScreen([](robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
    return std::make_unique<screen::PidTuning>(robot, screen, width, height, robot.drivetrain.rightPID,
                                               std::string("!PidTuning"));
  });
  logger::endScope();
  logger::scope("Initialize Screen");
  screen::initialize(robot); // initialize the screen
  logger::endScope();
#endif // DISABLE_SCREEN
  logger::endScope();
  rtos::onRootTaskEnd();
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 */
void autonomous() {
  rtos::onRootTaskStart();
#ifndef DISABLE_AUTONOMOUS
  Robot &robot = getRobot();
  robot.runAutonomous();
#else
  logger::error("Autonomous is disabled");
#endif
  rtos::onRootTaskEnd();
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

  rtos::onRootTaskStart();
  Robot &robot = getRobot();

  logger::scope("Opcontrol Setup");
  robot.setController(new control::input::Operator()); // set the robot controller to the default operator based one
  logger::endScope();

  robot.opcontrol();
  rtos::onRootTaskEnd();
}

/**
 * Called when the robot is at an official competition.
 */
void competition_initialize() {
  rtos::onRootTaskStart();
  rtos::onRootTaskEnd();
}

/**
 * Called when the robot should be stopped during a competition
 */
void disabled() {
  rtos::onRootTaskStart();
  rtos::onRootTaskEnd();
}

/**
 * Returns the current robot instance, or creates one if it has not been made yet.
 * @return the robot instance
 */
Robot &getRobot() {
  static Robot robot =
      Robot(DRIVETRAIN_MOTOR_L1, DRIVETRAIN_MOTOR_L2, DRIVETRAIN_MOTOR_L3, DRIVETRAIN_MOTOR_R1, DRIVETRAIN_MOTOR_R2,
            DRIVETRAIN_MOTOR_R3, INTAKE_RIGHT_MOTOR, INTAKE_LEFT_MOTOR, DRIVETRAIN_IMU, ARM_1, ARM_2);
  device::initialize();
  return robot;
}
