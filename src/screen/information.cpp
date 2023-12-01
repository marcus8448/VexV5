#include "information.hpp"
#include "debug/logger.hpp"

namespace screen {
void update_connectivity(lv_obj_t *label, std::string_view name, bool connected);
void update_device(lv_obj_t *label, const robot::device::Device &device, bool enabled);
void update_device(lv_obj_t *label, const robot::device::Device &device, double value);
void update_device(lv_obj_t *label, const robot::device::Device &device, int32_t value);

Information::Information(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height)
    : Screen(robot, width, height) {
  for (int i = 0; i < INFO_COLUMNS; i++) {
    lv_obj_t *obj = lv_label_create(screen);
    lv_obj_set_pos(obj, 0, coord(i * 16));
    lv_obj_set_width(obj, coord(width / 2));
    lv_label_set_text_static(obj, "");
    this->leftColumn[i].reset(obj);
  }

  for (int i = 0; i < INFO_COLUMNS; i++) {
    lv_obj_t *obj = lv_label_create(screen);
    lv_obj_set_pos(obj, coord(width / 2), coord(i * 16));
    lv_obj_set_width(obj, coord(width / 2));
    lv_label_set_text_static(obj, "");
    this->rightColumn[i].reset(obj);
  }
}

void Information::update() {
  int i = 0;
  for (const auto &[device, connected] : robot::device::getDevices()) {
    update_connectivity(this->leftColumn[i++].get(), device->getName(), connected);
    if (i == INFO_COLUMNS)
      break;
  }

  lv_label_set_text_fmt(this->rightColumn[0].get(),
    "Control Scheme: %s", driveSchemeName(robot.drivetrain.controlScheme));
  lv_label_set_text_fmt(this->rightColumn[1].get(), "X-position: %.2fin", this->robot.drivetrain.posX);
  lv_label_set_text_fmt(this->rightColumn[2].get(), "Y-position: %.2fin", this->robot.drivetrain.posY);
}

void update_device(lv_obj_t *label, const robot::device::Device &device, bool enabled) {
  if (!device.isConnected()) {
    lv_label_set_text_fmt(label, "%.*s: Disconnected", device.getName().length(), device.getName().data());
  } else {
    if (enabled) {
      lv_label_set_text_fmt(label, "%.*s: Enabled", device.getName().length(), device.getName().data());
    } else {
      lv_label_set_text_fmt(label, "%.*s: Disabled", device.getName().length(), device.getName().data());
    }
  }
}

void update_connectivity(lv_obj_t *label, std::string_view name, bool connected) {
  if (connected) {
    lv_label_set_text_fmt(label, "%.*s: Connected", name.length(), name.data());
  } else {
    lv_label_set_text_fmt(label, "%.*s: Disconnected", name.length(), name.data());
  }
}

void update_device(lv_obj_t *label, const robot::device::Device &device, double value) {
  if (!device.isConnected()) {
    lv_label_set_text_fmt(label, "%.*s: Disconnected", device.getName().length(), device.getName().data());
  } else {
    lv_label_set_text_fmt(label, "%.*s: %.2f", device.getName().length(), device.getName().data(), value);
  }
}

void update_device(lv_obj_t *label, const robot::device::Device &device, const int32_t value) {
  if (!device.isConnected()) {
    lv_label_set_text_fmt(label, "%.*s: Disconnected", device.getName().length(), device.getName().data());
  } else {
    lv_label_set_text_fmt(label, "%.*s: %i", device.getName().length(), device.getName().data(), value);
  }
}
} // namespace screen
