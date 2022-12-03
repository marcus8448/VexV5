#include "robot/device/device.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"
#include <cerrno>
#include <map>

namespace robot::device {
static bool initialized;
static std::vector<Device *> pendingDevices;
static std::map<Device *, bool> devices;

[[noreturn]] void reconfigure_task([[maybe_unused]] void *params);

Device::Device(uint8_t port) : port(port) { pendingDevices.push_back(this); }

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

[[noreturn]] void reconfigure_task([[maybe_unused]] void *params) {
  logger::info("Device reconfigure task started.");
  while (true) {
    if (!pendingDevices.empty()) {
      std::vector<Device *> pd;
      pd = pendingDevices;
      pendingDevices.clear();
      for (Device *&device : pd) {
        if (device->is_connected()) {
          devices.emplace(device, true);
        } else {
          devices.emplace(device, false);
          logger::warn("No device on port %i (expected %s).", device->get_port(), device->device_name());
        }
      }
    }

    for (auto &pair : devices) {
      bool connected = pair.first->is_connected();
      if (pair.second && !connected) {
        pair.second = false;
        logger::warn("%s on port %i disconnected.", pair.first->device_name(), pair.first->get_port());
      } else if (!pair.second && connected) {
        logger::info("%s on port %i reconnected. Reconfiguring...", pair.first->device_name(), pair.first->get_port());
        pair.second = true;
        pair.first->reconfigure();
      }
      pros::delay(100);
    }
  }
}
} // namespace robot::device
