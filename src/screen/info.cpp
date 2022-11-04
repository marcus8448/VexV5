#include "screen/info.hpp"
#include "configuration.hpp"
#include "screen/lvgl_util.hpp"
#include "screen/screen.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#pragma GCC diagnostic pop

namespace screen {
void update_motor_digital(lv_obj_t *label, const pros::Motor &motor, bool engaged, const char *name);
void update_motor(lv_obj_t *label, const char *name, double position);

Information::Information() = default;

void Information::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
  auto halfWidth = static_cast<lv_coord_t>(width / 2);

  create_label(screen, 0, 16 * 0, halfWidth, 16, "Build: " __DATE__ " " __TIME__);
  this->uptimeLabel = create_label(screen, halfWidth, 16 * 0, halfWidth, 16);
  this->controlSchemeLabel = create_label(screen, 0, 16 * 1, halfWidth, 16);

  // Port Statuses
  this->motorLFLabel = create_label(screen, 0, 16 * 2, halfWidth, 16);
  this->motorRFLabel = create_label(screen, halfWidth, 16 * 2, halfWidth, 16);
  this->motorLBLabel = create_label(screen, 0, 16 * 3, halfWidth, 16);
  this->motorRBLabel = create_label(screen, halfWidth, 16 * 3, halfWidth, 16);
  this->flywheelLabel = create_label(screen, 0, 16 * 4, halfWidth, 16);
  this->indexerLabel = create_label(screen, halfWidth, 16 * 4, halfWidth, 16);
  this->intakeLabel = create_label(screen, 0, 16 * 5, halfWidth, 16);
  this->flywheelTempLabel = create_label(screen, halfWidth, 16 * 6, halfWidth, 16);

  // special
  this->digitalSpeedLabel = create_label(screen, halfWidth, 16 * 5, halfWidth, 16);
}

void Information::update(robot::Robot *robot) {
  set_label_text(this->uptimeLabel, "Uptime: %i", pros::millis());
  set_label_text(this->controlSchemeLabel, "Control Scheme: %s",
                 config::get_drive_scheme_name(config::get_instance()->get_drivetrain_control_scheme()));
  update_motor(this->motorLFLabel, "DT-LF", robot->drivetrain->leftFront.get_position());
  update_motor(this->motorRFLabel, "DT-RF", robot->drivetrain->rightFront.get_position());
  update_motor(this->motorLBLabel, "DT-LB", robot->drivetrain->leftBack.get_position());
  update_motor(this->motorRBLabel, "DT-RB", robot->drivetrain->rightBack.get_position());
  update_motor(this->flywheelLabel, "Flywheel", robot->flywheel->get_motor().get_velocity());
  update_motor(this->flywheelTempLabel, "Flywheel Temp", robot->flywheel->get_motor().get_temperature());
  update_motor_digital(this->indexerLabel, robot->indexer->get_motor().get_raw_motor(), false, "Indexer");
  update_motor_digital(this->intakeLabel, robot->intake->get_motor().get_raw_motor(), false, "Intake/Roller");
  set_label_text(this->digitalSpeedLabel, "Flywheel Speed: %i", (int32_t)robot->controller->flywheel_speed());
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
