#include "screen/information.hpp"
#include "configuration.hpp"
#include "screen/lvgl_util.hpp"
#include "screen/screen.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#pragma GCC diagnostic pop

namespace screen {
extern lv_coord_t halfWidth;
void update_device_digital(lv_obj_t *label, const robot::device::Device &device, bool engaged);
void update_device(lv_obj_t *label, const robot::device::Device &device, double value);
void update_device(lv_obj_t *label, const robot::device::Device &device, int32_t value);
void update_motor_pos(lv_obj_t *label, const robot::device::Motor &motor);
void update_motor_vel(lv_obj_t *label, const robot::device::Motor &motor);
void update_motor_temp(lv_obj_t *label, const robot::device::Motor &motor);

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
  this->flywheelSecondaryLabel = create_info_label(screen, true, 5);

  this->flywheelTempLabel = create_info_label(screen, false, 6);
  this->flywheelSecondaryTempLabel = create_info_label(screen, true, 6);

  this->flywheelSpeedLabel = create_info_label(screen, false, 7);
  //  this->_unused = create_info_label(screen, true, 7);

  this->rollerUpperHue = create_info_label(screen, false, 8);
  this->rollerLowerHue = create_info_label(screen, true, 8);
}

void Information::update(robot::Robot &robot) {
  set_label_text(this->uptimeLabel, "Uptime: %i", pros::millis());
  set_label_text(this->controlSchemeLabel, "Control Scheme: %s",
                 config::get_drive_scheme_name(config::get_instance()->get_control_scheme()));
  set_label_text(this->teamColourLabel, "Team Colour: %s",
                 config::get_alliance_colour_name(config::get_instance()->get_alliance_colour()));
  update_motor_pos(this->motorLFLabel, robot.drivetrain->leftFront);
  update_motor_pos(this->motorRFLabel, robot.drivetrain->rightFront);
  update_motor_pos(this->motorLBLabel, robot.drivetrain->leftBack);
  update_motor_pos(this->motorRBLabel, robot.drivetrain->rightBack);
  update_device_digital(this->intakeLabel, robot.intake->get_motor(), robot.intake->is_engaged());
  update_device_digital(this->indexerLabel, robot.indexer->get_motor(), false);
  update_motor_vel(this->flywheelLabel, robot.flywheel->get_primary_motor());
  update_motor_vel(this->flywheelSecondaryLabel, robot.flywheel->get_secondary_motor());
  update_motor_temp(this->flywheelTempLabel, robot.flywheel->get_primary_motor());
  update_motor_temp(this->flywheelSecondaryTempLabel, robot.flywheel->get_secondary_motor());
  set_label_text(this->flywheelSpeedLabel, "Flywheel Speed: %i", robot.controller->flywheel_speed());
  //  set_label_text(this->_unused, "<unused>");

  update_device(this->rollerUpperHue, robot.intake->get_upper_optical(), robot.intake->get_upper_optical().get_hue());
  update_device(this->rollerLowerHue, robot.intake->get_lower_optical(), robot.intake->get_lower_optical().get_hue());
}

void update_device_digital(lv_obj_t *label, const robot::device::Device &device, bool engaged) {
  if (!device.is_connected()) {
    set_label_text(label, "%s: Disconnected", device.get_name());
  } else {
    if (engaged) {
      set_label_text(label, "%s: Enabled", device.get_name());
    } else {
      set_label_text(label, "%s: Disabled", device.get_name());
    }
  }
}

void update_device(lv_obj_t *label, const robot::device::Device &device, double value) {
  if (!device.is_connected()) {
    set_label_text(label, "%s: Disconnected", device.get_name());
  } else {
    set_label_text(label, "%s: %f", device.get_name(), value);
  }
}

void update_device(lv_obj_t *label, const robot::device::Device &device, int32_t value) {
  if (!device.is_connected()) {
    set_label_text(label, "%s: Disconnected", device.get_name());
  } else {
    set_label_text(label, "%s: %i", device.get_name(), value);
  }
}

void update_motor_pos(lv_obj_t *label, const robot::device::Motor &motor) {
  update_device(label, motor, motor.get_position());
}

void update_motor_vel(lv_obj_t *label, const robot::device::Motor &motor) {
  update_device(label, motor, motor.get_velocity());
}

void update_motor_temp(lv_obj_t *label, const robot::device::Motor &motor) {
  update_device(label, motor, motor.get_temperature());
}

} // namespace screen
