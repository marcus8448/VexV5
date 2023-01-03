#include "robot/device/device.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"
#include <cerrno>
#include <map>

namespace robot::device {
static std::vector<Device *> pendingDevices;
static std::map<Device *, bool> devices;

[[noreturn]] void reconfigure_task([[maybe_unused]] void *params);

Device::Device(uint8_t port, const char *name) : port(port), name(name) { pendingDevices.push_back(this); }

void initialize() {
  static bool init = false;
  if (!init) {
    init = true;
    pros::Task(reconfigure_task, nullptr, "Device reconfigure task");
  }
}

bool Device::checkConnect() {
  bool b = errno != ENODEV;
  errno = 0;
  return b;
}

[[nodiscard]] uint8_t Device::get_port() const { return this->port; }

[[nodiscard]] const char *Device::get_name() const { return this->name; }

[[noreturn]] void reconfigure_task([[maybe_unused]] void *params) {
  info("Device reconfigure task started.");
  while (true) {
    if (!pendingDevices.empty()) {
      std::vector<Device *> pd;
      pd = pendingDevices;
      pendingDevices.clear();
      for (Device *&device : pd) {
        if (device->is_connected()) {
          debug("%s '%s' connected on port %i", device->get_type_name(), device->get_name(), device->get_port());
          devices.emplace(device, true);
        } else {
          devices.emplace(device, false);
          warn("No device on port %i (expected %s '%s').", device->get_port(), device->get_type_name(),
                        device->get_name());
        }
      }
    }

    for (auto &pair : devices) {
      bool connected = pair.first->is_connected();
      if (pair.second && !connected) {
        pair.second = false;
        error("%s '%s' (port %i) disconnected.", pair.first->get_type_name(), pair.first->get_name(),
                       pair.first->get_port());
      } else if (!pair.second && connected) {
        warn("%s '%s' (port %i) reconnected. Reconfiguring...", pair.first->get_type_name(),
                      pair.first->get_name(), pair.first->get_port());
        pair.second = true;
        pair.first->reconfigure();
      }
      pros::delay(100);
    }
  }
}
} // namespace robot::device
