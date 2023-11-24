#include "screen/information.hpp"
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
    lv_label_set_text(obj, "");
    this->leftColumn[i].reset(obj);
  }

  for (int i = 0; i < INFO_COLUMNS; i++) {
    lv_obj_t *obj = lv_label_create(screen);
    lv_obj_set_pos(obj, coord(width / 2), coord(i * 16));
    lv_obj_set_width(obj, coord(width / 2));
    lv_label_set_text(obj, "");
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

  lv_label_set_text(this->rightColumn[0].get(),
                    fmt::string_format("Control Scheme: {}", driveSchemeName(robot.drivetrain.controlScheme)).c_str());
  lv_label_set_text(this->rightColumn[1].get(),
                    fmt::string_format("X-position: {}in", this->robot.drivetrain.posX).c_str());
  lv_label_set_text(this->rightColumn[2].get(),
                    fmt::string_format("Y-position: {}in", this->robot.drivetrain.posY).c_str());
}

void update_device(lv_obj_t *label, const robot::device::Device &device, bool enabled) {
  if (!device.isConnected()) {
    lv_label_set_text(label, fmt::string_format("{}: Disconnected", device.getName()).c_str());
  } else {
    if (enabled) {
      lv_label_set_text(label, fmt::string_format("{}: Enabled", device.getName()).c_str());
    } else {
      lv_label_set_text(label, fmt::string_format("{}: Disabled", device.getName()).c_str());
    }
  }
}

void update_connectivity(lv_obj_t *label, std::string_view name, bool connected) {
  if (connected) {
    lv_label_set_text(label, fmt::string_format("{}: Connected", name).c_str());
  } else {
    lv_label_set_text(label, fmt::string_format("{}: Disconnected", name).c_str());
  }
}

void update_device(lv_obj_t *label, const robot::device::Device &device, double value) {
  if (!device.isConnected()) {
    lv_label_set_text(label, fmt::string_format("{}: Disconnected", device.getName()).c_str());
  } else {
    lv_label_set_text(label, fmt::string_format("{}: {}", device.getName(), value).c_str());
  }
}

void update_device(lv_obj_t *label, const robot::device::Device &device, int32_t value) {
  if (!device.isConnected()) {
    lv_label_set_text(label, fmt::string_format("{}: Disconnected", device.getName()).c_str());
  } else {
    lv_label_set_text(label, fmt::string_format("{}: {}", device.getName(), value).c_str());
  }
}
} // namespace screen
