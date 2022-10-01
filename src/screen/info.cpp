#include "screen/screen.hpp"
#include "screen/lvgl_util.hpp"
#include "screen/info.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#pragma GCC diagnostic pop

namespace screen {
void update_motor_digital(lv_obj_t *label, pros::Motor *motor, bool engaged, const char *name);
void update_motor(lv_obj_t *label, const char *name, int32_t voltage);

Information::Information() = default;

void Information::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
  auto halfWidth = static_cast<lv_coord_t>(width / 2);

  create_label(screen, 0, 16 * 0, halfWidth, 16, "Build: " __DATE__ " " __TIME__);
  this->uptimeLabel = create_label(screen, halfWidth, 16 * 0, halfWidth, 16);

  // Port Statuses
  this->motorLFLabel = create_label(screen, 0, 16 * 2, halfWidth, 16);
  this->motorRFLabel = create_label(screen, halfWidth, 16 * 2, halfWidth, 16);
  this->motorLBLabel = create_label(screen, 0, 16 * 3, halfWidth, 16);
  this->motorRBLabel = create_label(screen, halfWidth, 16 * 3, halfWidth, 16);
  this->flywheelLabel = create_label(screen, 0, 16 * 4, halfWidth, 16);
  this->rollerLabel = create_label(screen, halfWidth, 16 * 4, halfWidth, 16);
  this->intakeLabel = create_label(screen, 0, 16 * 5, halfWidth, 16);

  // special
  this->digitalSpeedLabel = create_label(screen, halfWidth, 16 * 5, halfWidth, 16);
}

void Information::initialize(lv_coord_t width, lv_coord_t height) {
}

void Information::update(robot::Robot *robot) {
  set_label_text(this->uptimeLabel, "Uptime: %i", pros::millis());
  update_motor(this->motorLFLabel, "DT-LF", robot->drivetrain->leftFront->get_voltage());
  update_motor(this->motorRFLabel, "DT-RF", robot->drivetrain->rightFront->get_voltage());
  update_motor(this->motorLBLabel, "DT-LB", robot->drivetrain->leftBack->get_voltage());
  update_motor(this->motorRBLabel, "DT-RB", robot->drivetrain->rightBack->get_voltage());
  update_motor_digital(this->flywheelLabel, robot->flywheel->motor, robot->flywheel->isEngaged(), "Flywheel");
  update_motor_digital(this->rollerLabel, nullptr, false, "Roller");
  update_motor_digital(this->intakeLabel, nullptr, false, "Intake");
  set_label_text(this->digitalSpeedLabel, "Digital Speed: %i", robot->controller->digital_speed());
}

void Information::destroy(lv_obj_t *screen) {

}

void update_motor_digital(lv_obj_t *label, pros::Motor *motor, bool engaged, const char *name) {
  if (motor == nullptr || motor->get_voltage() == INT32_MAX) {
    set_label_text(label, "%s: Disconnected", name);
  } else {
    if (engaged) {
      set_label_text(label, "%s: Enabled", name);
    } else {
      set_label_text(label, "%s: Disabled", name);
    }
  }
}

void update_motor(lv_obj_t *label, const char *name, int32_t voltage) {
  if (voltage == INT32_MAX) {
    set_label_text(label, "%s: Disconnected", name);
  } else {
    set_label_text(label, "%s: %i", name, voltage);
  }
}
} // screen::info