#include "screen/information.hpp"
#include "pros/rtos.h"
#include "screen/lvgl_util.hpp"

namespace screen {
extern lv_coord_t halfWidth;
void update_device_digital(lv_obj_t *label, const robot::device::Device &device, bool engaged);
void update_device(lv_obj_t *label, const robot::device::Device &device, double value);
void update_device(lv_obj_t *label, const robot::device::Device &device, int32_t value);
void update_motor_pos(lv_obj_t *label, const robot::device::Motor &motor);
void update_motor_vel(lv_obj_t *label, const robot::device::Motor &motor);
void update_motor_temp(lv_obj_t *label, const robot::device::Motor &motor);

Information::Information(robot::Robot &robot) : robot(robot) {}

void Information::initialize(lv_obj_t *screen) {
  create_info_label(screen, false, 0, "Build: " __DATE__ " " __TIME__);
  this->uptimeLabel = create_info_label(screen, true, 0);

  this->controlSchemeLabel = create_info_label(screen, false, 1);
  //  this->teamColourLabel = create_info_label(screen, true, 1);

  // Port Statuses
  this->motorLFLabel = create_info_label(screen, false, 2);
  this->motorRFLabel = create_info_label(screen, true, 2);

  this->motorLBLabel = create_info_label(screen, false, 3);
  this->motorRBLabel = create_info_label(screen, true, 3);

  //  this->_unused1 = create_info_label(screen, false, 4);
  //  this->_unused0 = create_info_label(screen, true, 4);
  //
  //  this->_unused2 = create_info_label(screen, false, 5);
  //  this->_unused3 = create_info_label(screen, true, 5);
  //
  //  this->_unused4 = create_info_label(screen, false, 6);
  //  this->_unused5 = create_info_label(screen, true, 6);

  this->digitalSpeed = create_info_label(screen, false, 7);
  //  this->_unused = create_info_label(screen, true, 7);
  //  this->_unused = create_info_label(screen, false, 8);
  //  this->_unused = create_info_label(screen, true, 8);
}

void Information::update() {
  set_label_text(this->uptimeLabel, "Uptime: %i", pros::c::millis());
  set_label_text(this->controlSchemeLabel, "Control Scheme: %s",
                 robot::driveSchemeName(robot.drivetrain.controlScheme));
  update_motor_pos(this->motorLFLabel, this->robot.drivetrain.leftFront);
  update_motor_pos(this->motorRFLabel, this->robot.drivetrain.rightFront);
  update_motor_pos(this->motorLBLabel, this->robot.drivetrain.leftBack);
  update_motor_pos(this->motorRBLabel, this->robot.drivetrain.rightBack);
  set_label_text(this->digitalSpeed, "Flywheel Speed: %i", this->robot.controller->speedSetting());
}

void update_device_digital(lv_obj_t *label, const robot::device::Device &device, bool engaged) {
  if (!device.isConnected()) {
    set_label_text(label, "%s: Disconnected", device.getName());
  } else {
    if (engaged) {
      set_label_text(label, "%s: Enabled", device.getName());
    } else {
      set_label_text(label, "%s: Disabled", device.getName());
    }
  }
}

void update_device(lv_obj_t *label, const robot::device::Device &device, double value) {
  if (!device.isConnected()) {
    set_label_text(label, "%s: Disconnected", device.getName());
  } else {
    set_label_text(label, "%s: %f", device.getName(), value);
  }
}

void update_device(lv_obj_t *label, const robot::device::Device &device, int32_t value) {
  if (!device.isConnected()) {
    set_label_text(label, "%s: Disconnected", device.getName());
  } else {
    set_label_text(label, "%s: %i", device.getName(), value);
  }
}

void update_motor_pos(lv_obj_t *label, const robot::device::Motor &motor) {
  update_device(label, motor, motor.getPosition());
}

void update_motor_vel(lv_obj_t *label, const robot::device::Motor &motor) {
  update_device(label, motor, motor.getVelocity());
}

void update_motor_temp(lv_obj_t *label, const robot::device::Motor &motor) {
  update_device(label, motor, motor.getTemperature());
}

} // namespace screen
