#include "screen/information.hpp"
#include "debug/logger.hpp"
#include "units.hpp"

namespace screen {
void update_connectivity(lv_obj_t *label, const char *name, bool connected);
void update_device(lv_obj_t *label, const robot::device::Device &device, bool enabled);
void update_device(lv_obj_t *label, const robot::device::Device &device, double value);
void update_device(lv_obj_t *label, const robot::device::Device &device, int32_t value);

Information::Information(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height)
    : Screen(robot, width, height) {
  for (size_t i = 0; i < INFO_COLUMNS; i++) {
    lv_obj_t *obj = lv_label_create(screen);
    lv_obj_set_pos(obj, 0, i * 16);
    lv_obj_set_width(obj, width / 2);
    lv_label_set_text(obj, "");
    this->leftColumn[i] = obj;
  }

  for (size_t i = 0; i < INFO_COLUMNS; i++) {
    lv_obj_t *obj = lv_label_create(screen);
    lv_obj_set_pos(obj, width / 2, i * 16);
    lv_obj_set_width(obj, width / 2);
    lv_label_set_text(obj, "");
    this->rightColumn[i] = obj;
  }
}

void Information::update() {
  int i = 0;
  for (const auto &item : *robot::device::get_devices()) {
    update_connectivity(this->leftColumn[i++], item.first->getName(), item.second);
    if (i == INFO_COLUMNS)
      break;
  }

  lv_label_set_text(
      this->rightColumn[0],
      fmt::string_format("Control Scheme: %s", robot::driveSchemeName(robot.drivetrain.controlScheme)).c_str());
  lv_label_set_text(this->rightColumn[1],
                    fmt::string_format("X-position: %fin", units::encoderToInch(this->robot.drivetrain.cPosX)).c_str());
  lv_label_set_text(this->rightColumn[2],
                    fmt::string_format("Y-position: %fin", units::encoderToInch(this->robot.drivetrain.cPosY)).c_str());
}

Information::~Information() {
  for (size_t i = 0; i < INFO_COLUMNS; ++i) {
    lv_obj_del_async(this->leftColumn[i]);
    lv_obj_del_async(this->rightColumn[i]);
  }
}

void update_device(lv_obj_t *label, const robot::device::Device &device, bool enabled) {
  if (!device.isConnected()) {
    lv_label_set_text(label, fmt::string_format("%s: Disconnected", device.getName()).c_str());
  } else {
    if (enabled) {
      lv_label_set_text(label, fmt::string_format("%s: Enabled", device.getName()).c_str());
    } else {
      lv_label_set_text(label, fmt::string_format("%s: Disabled", device.getName()).c_str());
    }
  }
}

void update_connectivity(lv_obj_t *label, const char *name, bool connected) {
  if (connected) {
    lv_label_set_text(label, fmt::string_format("%s: Connected", name).c_str());
  } else {
    lv_label_set_text(label, fmt::string_format("%s: Disconnected", name).c_str());
  }
}

void update_device(lv_obj_t *label, const robot::device::Device &device, double value) {
  if (!device.isConnected()) {
    lv_label_set_text(label, fmt::string_format("%s: Disconnected", device.getName()).c_str());
  } else {
    lv_label_set_text(label, fmt::string_format("%s: %f", device.getName(), value).c_str());
  }
}

void update_device(lv_obj_t *label, const robot::device::Device &device, int32_t value) {
  if (!device.isConnected()) {
    lv_label_set_text(label, fmt::string_format("%s: Disconnected", device.getName()).c_str());
  } else {
    lv_label_set_text(label, fmt::string_format("%s: %i", device.getName(), value).c_str());
  }
}
} // namespace screen
