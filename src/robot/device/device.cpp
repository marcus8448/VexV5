#include "robot/device/device.hpp"
#include "debug/logger.hpp"
#include "pros/rtos.h"
#include <cerrno>
#include <map>
#include <vector>

#define DEVICE_CONFIGURE_RATE 500

namespace robot::device {
static std::vector<Device *> pendingDevices;
static std::map<Device *, bool> devices;

[[noreturn]] void reconfigureTask([[maybe_unused]] void *params);

Device::Device(const char *typeName, const char *name, uint8_t port) : typeName(typeName), name(name), port(port) {
  pendingDevices.push_back(this);
}

void initialize() {
  static bool init = false;
  if (!init) {
    init = true;
    pros::c::task_create(reconfigureTask, nullptr, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT,
                         "Device reconfigure task");
  }
}

bool Device::checkConnect() {
  bool b = errno != ENODEV;
  errno = 0;
  return b;
}

[[nodiscard]] uint8_t Device::getPort() const { return this->port; }

[[nodiscard]] const char *Device::getTypeName() const { return this->typeName; }

[[nodiscard]] const char *Device::getName() const { return this->name; }

[[noreturn]] void reconfigureTask([[maybe_unused]] void *params) {
  pros::c::delay(DEVICE_CONFIGURE_RATE * 2); // printing race condition?
  info("Device reconfigure task started.");
  while (true) {
    if (!pendingDevices.empty()) {
      std::vector<Device *> pd;
      pd = pendingDevices;
      pendingDevices.clear();
      for (Device *&device : pd) {
        if (device->isConnected()) {
          debug("%s '%s' connected on port %i", device->getTypeName(), device->getName(), device->getPort());
          devices.emplace(device, true);
        } else {
          devices.emplace(device, false);
          warn("No device on port %i (expected %s '%s').", device->getPort(), device->getTypeName(), device->getName());
        }
      }
    }

    for (auto &pair : devices) {
      bool connected = pair.first->isConnected();
      if (pair.second && !connected) {
        pair.second = false;
        error("%s '%s' (port %i) disconnected.", pair.first->getTypeName(), pair.first->getName(),
              pair.first->getPort());
      } else if (!pair.second && connected) {
        warn("%s '%s' (port %i) reconnected. Reconfiguring...", pair.first->getTypeName(), pair.first->getName(),
             pair.first->getPort());
        pair.second = true;
        pair.first->reconfigure();
      }
      pros::c::delay(DEVICE_CONFIGURE_RATE);
    }
  }
}
} // namespace robot::device
