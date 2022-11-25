#include "screen/info.hpp"
#include "configuration.hpp"
#include "screen/lvgl_util.hpp"
#include "screen/screen.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#pragma GCC diagnostic pop

namespace screen {
static lv_coord_t halfWidth;
lv_obj_t *create_info_label(lv_obj_t *screen, bool right, lv_coord_t index);
void update_motor_digital(lv_obj_t *label, const pros::Motor &motor, bool engaged, const char *name);
void update_motor(lv_obj_t *label, const char *name, double position);

Information::Information() = default;

void Information::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
  halfWidth = static_cast<lv_coord_t>(width / 2);

  create_label(screen, 0, 0, halfWidth, 16, "Build: " __DATE__ " " __TIME__);
  this->uptimeLabel = create_info_label(screen, halfWidth, 0);
  this->controlSchemeLabel = create_info_label(screen, 0, 1);

  // Port Statuses
  this->motorLFLabel = create_info_label(screen, false, 2);
  this->motorRFLabel = create_info_label(screen, true, 2);

  this->motorLBLabel = create_info_label(screen, false, 3);
  this->motorRBLabel = create_info_label(screen, true, 3);

  this->flywheelLabel = create_info_label(screen, false, 4);
  this->indexerLabel = create_info_label(screen, true, 4);

  this->intakeLabel = create_info_label(screen, false, 5);
  this->flywheelSpeedLabel = create_info_label(screen, true, 5);

  this->flywheelTempLabel = create_info_label(screen, true, 6);
}

void Information::update(robot::Robot &robot) {
  set_label_text(this->uptimeLabel, "Uptime: %i", pros::millis());
  set_label_text(this->controlSchemeLabel, "Control Scheme: %s",
                 config::get_drive_scheme_name(config::get_instance()->get_drivetrain_control_scheme()));
  update_motor(this->motorLFLabel, "DT-LF", robot.drivetrain->leftFront.get_position());
  update_motor(this->motorRFLabel, "DT-RF", robot.drivetrain->rightFront.get_position());
  update_motor(this->motorLBLabel, "DT-LB", robot.drivetrain->leftBack.get_position());
  update_motor(this->motorRBLabel, "DT-RB", robot.drivetrain->rightBack.get_position());
  update_motor(this->flywheelLabel, "Flywheel", robot.flywheel->get_motor().get_velocity());
  update_motor(this->flywheelTempLabel, "Flywheel Temp", robot.flywheel->get_motor().get_temperature());
  update_motor_digital(this->indexerLabel, robot.indexer->get_motor().get_raw_motor(), false, "Indexer");
  update_motor_digital(this->intakeLabel, robot.intake->get_motor().get_raw_motor(), false, "Intake/Roller");
  set_label_text(this->flywheelSpeedLabel, "Flywheel Speed: %i", (int32_t)robot.controller->flywheel_speed());
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

lv_obj_t *create_info_label(lv_obj_t *screen, bool right, lv_coord_t index) {
  return create_label(screen, right ? halfWidth : 0, index * 16, halfWidth, 16);
}
} // namespace screen
