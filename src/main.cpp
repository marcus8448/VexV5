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
// #define DEBUG_LOG
// END CONFIG

#include <iostream>

#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"

#include "constants.hpp"
#include "logger.hpp"
#include "robot/controller/operator.hpp"
#include "robot/robot.hpp"

#ifdef AUTONOMOUS
#include "robot/autonomous/autonomous.hpp"
#include "robot/autonomous/left_score.hpp"
#include "robot/autonomous/left_winpoint.hpp"
#include "robot/autonomous/right_score.hpp"
#include "robot/autonomous/right_winpoint.hpp"
#endif

#ifdef SERIAL_LINK
#include "serial/serial.hpp"
#include "serial_plugins.hpp"
#endif

#ifdef SCREEN
// #include "screen/config.hpp"
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

using namespace robot;

Robot *get_or_create_robot();

/**
 * Called when the robot is first initialized.
 */
void initialize() {
  logger::push("Initialize");
  Robot *robot = get_or_create_robot();
    std::cout << robot << std::endl;

  // Optionally disable autonomous for builds
#ifdef AUTONOMOUS
  // Register the different types of autonomous-es
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
  // screen::add_screen(new screen::Configuration());
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
#ifdef SERIAL_LINK
  serial::add_plugin(0, new serial::RobotStatePlugin(robot));
  serial::add_plugin(1, new serial::RobotCommandsPlugin(robot));
  serial::initialize();
#endif // SERIAL_LINK
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
    // create the robot
    robot = new Robot(new Drivetrain(new pros::Motor(RIGHT_FRONT_MOTOR, DRIVETRAIN_GEARSET, false, ENCODER_UNITS),
                                     new pros::Motor(LEFT_FRONT_MOTOR, DRIVETRAIN_GEARSET, true, ENCODER_UNITS),
                                     new pros::Motor(RIGHT_BACK_MOTOR, DRIVETRAIN_GEARSET, false, ENCODER_UNITS),
                                     new pros::Motor(LEFT_BACK_MOTOR, DRIVETRAIN_GEARSET, true, ENCODER_UNITS)),
                      new Intake(new pros::Motor(INTAKE_MOTOR, INTAKE_GEARSET, false, ENCODER_UNITS)),
                      new Indexer(new pros::Motor(INDEXER_MOTOR, INDEXER_GEARSET, true, ENCODER_UNITS)),
                      new Flywheel(new pros::Motor(FLYWHEEL_MOTOR, FLYWHEEL_GEARSET, true, ENCODER_UNITS)),
                      new Roller(new pros::Motor(ROLLER_MOTOR, ROLLER_GEARSET, false, ENCODER_UNITS),
                                 new pros::Optical(ROLLER_OPTICAL)));
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
