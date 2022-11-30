#include "screen/info.hpp"
#include "configuration.hpp"
#include "screen/lvgl_util.hpp"
#include "screen/screen.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#pragma GCC diagnostic pop

namespace screen {
extern lv_coord_t halfWidth;
void update_motor_digital(lv_obj_t *label, const pros::Motor &motor, bool engaged, const char *name);
void update_motor(lv_obj_t *label, const char *name, double position);

Information::Information() = default;

void Information::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
  create_info_label(screen, false, 0, "Build: " __DATE__ " " __TIME__);
  this->uptimeLabel = create_info_label(screen, true, 0);

  this->controlSchemeLabel = create_info_label(screen, false, 1);
  this->teamColourLabel = create_info_label(screen, true, 1);

  // Port Statuses
  this->motorLFLabel = create_info_label(screen, false, 2);
  this->motorRFLabel = create_info_label(screen, true, 2);

  this->motorLBLabel = create_info_label(screen, false, 3);
  this->motorRBLabel = create_info_label(screen, true, 3);

  this->intakeLabel = create_info_label(screen, false, 4);
  this->indexerLabel = create_info_label(screen, true, 4);

  this->flywheelLabel = create_info_label(screen, false, 5);
  this->flywheelSecondaryLabel = create_info_label(screen, false, 5);

  this->flywheelSpeedLabel = create_info_label(screen, true, 6);
  this->flywheelSecondarySpeedLabel = create_info_label(screen, true, 6);

  this->flywheelTempLabel = create_info_label(screen, false, 7);
  this->flywheelSecondaryTempLabel = create_info_label(screen, true, 7);
}

void Information::update(robot::Robot &robot) {
  set_label_text(this->uptimeLabel, "Uptime: %i", pros::millis());
  set_label_text(this->controlSchemeLabel, "Control Scheme: %s",
                 config::get_drive_scheme_name(config::get_instance()->get_drivetrain_control_scheme()));
  set_label_text(this->teamColourLabel, "Team Colour: %s",
                 config::get_alliance_colour_name(config::get_instance()->get_alliance_colour()));
  update_motor(this->motorLFLabel, "DT-LF", robot.drivetrain->leftFront.get_position());
  update_motor(this->motorRFLabel, "DT-RF", robot.drivetrain->rightFront.get_position());
  update_motor(this->motorLBLabel, "DT-LB", robot.drivetrain->leftBack.get_position());
  update_motor(this->motorRBLabel, "DT-RB", robot.drivetrain->rightBack.get_position());
  update_motor_digital(this->intakeLabel, robot.intake->get_motor().get_raw_motor(), false, "Intake/Roller");
  update_motor_digital(this->indexerLabel, robot.indexer->get_motor().get_raw_motor(), false, "Indexer");
  update_motor(this->flywheelLabel, "Flywheel", robot.flywheel->get_first_motor().get_velocity());
  update_motor(this->flywheelSecondaryLabel, "Flywheel2", robot.flywheel->get_first_motor().get_velocity());
  update_motor(this->flywheelTempLabel, "Flywheel Temp", robot.flywheel->get_first_motor().get_temperature());
  update_motor(this->flywheelSecondaryTempLabel, "Flywheel2 Temp", robot.flywheel->get_first_motor().get_temperature());
  set_label_text(this->flywheelSpeedLabel, "Flywheel Speed: %i", (int32_t)robot.controller->flywheel_speed());
  set_label_text(this->flywheelSecondarySpeedLabel, "Flywheel2 Speed: %i", (int32_t)robot.controller->flywheel_speed());
}

void update_motor_digital(lv_obj_t *label, const pros::Motor &motor, bool engaged, const char *name) {
  if (motor.get_voltage() == INT32_MAX) {
    set_label_text(label, "%s: Disconnected", name);
  } else {
    if (engaged) {
      set_label_text(label, "%s: Enabled", name);
    } else {
      set_label_text(label, "%s: Disabled", name);
    }
  }
}

void update_motor(lv_obj_t *label, const char *name, double position) {
  if (position == INT32_MAX) {
    set_label_text(label, "%s: Disconnected", name);
  } else {
    set_label_text(label, "%s: %f", name, position);
  }
}
} // namespace screen
