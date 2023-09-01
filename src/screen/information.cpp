#include "screen/information.hpp"
#include "debug/logger.hpp"
#include "units.hpp"

namespace screen {
void update_connectivity(lv_obj_t *label, const char *name, bool connected);
void update_device(lv_obj_t *label, const robot::device::Device &device, bool enabled);
void update_device(lv_obj_t *label, const robot::device::Device &device, double value);
void update_device(lv_obj_t *label, const robot::device::Device &device, int32_t value);

Information::Information(robot::Robot &robot) : robot(robot) {}

void Information::initialize(lv_obj_t *screen) {
  for (size_t i = 0; i < INFO_COLUMNS; i++) {
    if (this->leftColumn[i] == nullptr) {
      lv_obj_t *obj = lv_label_create(screen);
      lv_obj_set_pos(obj, 0, i * 16);
      lv_obj_set_width(obj, SCREEN_WIDTH / 2);
      lv_label_set_text(obj, "");
      this->leftColumn[i] = obj;
    }
  }

  for (size_t i = 0; i < INFO_COLUMNS; i++) {
    if (this->rightColumn[i] == nullptr) {
      lv_obj_t *obj = lv_label_create(screen);
      lv_obj_set_pos(obj, SCREEN_WIDTH / 2, i * 16);
      lv_obj_set_width(obj, SCREEN_WIDTH / 2);
      lv_label_set_text(obj, "");
      this->rightColumn[i] = obj;
    }
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

void Information::cleanup() {
  for (int i = 0; i < INFO_COLUMNS; ++i) {
    lv_obj_del_async(this->leftColumn[i]);
    lv_obj_del_async(this->rightColumn[i]);
    this->leftColumn[i] = nullptr;
    this->rightColumn[i] = nullptr;
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

lv_obj_t *create_label(lv_obj_t *screen, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char *text) {
  lv_obj_t *label = lv_label_create(screen);

  lv_obj_set_pos(label, x, y);
  lv_obj_set_size(label, w, h);
  lv_label_set_text(label, text);

  return label;
}

lv_obj_t *create_label(lv_obj_t *screen, lv_coord_t index, bool right, const char *text) {
  return create_label(screen, right ? SCREEN_HALF_WIDTH : static_cast<lv_coord_t>(0),
                      static_cast<lv_coord_t>(index * 16), SCREEN_HALF_WIDTH, 16, text);
}
} // namespace screen
