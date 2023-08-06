#include "screen/information.hpp"
#include "units.hpp"
#include "debug/logger.hpp"

namespace screen {
template <typename... Args> void set_label_text(lv_obj_t *label, const char *format, Args... args) {
  static char *buffer = new char[64];
  snprintf(buffer, 64, format, args...);
  lv_label_set_text(label, buffer);
}

void update_connectivity(lv_obj_t *label, const char *name, bool connected);
void update_device(lv_obj_t *label, const robot::device::Device &device, bool enabled);
void update_device(lv_obj_t *label, const robot::device::Device &device, double value);
void update_device(lv_obj_t *label, const robot::device::Device &device, int32_t value);

Information::Information(robot::Robot &robot) : robot(robot) {
}

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
    if (i == INFO_COLUMNS) break;
  }

  set_label_text(this->rightColumn[0], "Control Scheme: %s",
                 robot::driveSchemeName(robot.drivetrain.controlScheme));
  set_label_text(this->rightColumn[1], "X-position: %fin", units::encoderToInch(this->robot.drivetrain.posX));
  set_label_text(this->rightColumn[2], "Y-position: %fin", units::encoderToInch(this->robot.drivetrain.posY));
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
    set_label_text(label, "%s: Disconnected", device.getName());
  } else {
    if (enabled) {
      set_label_text(label, "%s: Enabled", device.getName());
    } else {
      set_label_text(label, "%s: Disabled", device.getName());
    }
  }
}

void update_connectivity(lv_obj_t *label, const char *name, bool connected) {
  if (connected) {
    set_label_text(label, "%s: Connected", name);
  } else {
    set_label_text(label, "%s: Disconnected", name);
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
