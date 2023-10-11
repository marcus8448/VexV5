#include "robot/device/device.hpp"
#include "debug/logger.hpp"
#include "pros/rtos.h"
#include "tasks.hpp"
#include <vector>

namespace robot::device {
static std::vector<Device *> pendingDevices;
static std::unordered_map<Device *, bool> devices;

[[noreturn]] void reconfigureTask([[maybe_unused]] void *params);

Device::Device(const char *typeName, const char *name, int8_t port) : typeName(typeName), name(name), port(port) {
  pendingDevices.push_back(this);
}

void initialize() {
  static bool init = false;
  if (!init) {
    init = true;
    rtos::createTask("Device reconfigure task", reconfigureTask, nullptr, TASK_PRIORITY_DEFAULT - 1, 0x1000);
  }
}

std::unordered_map<Device *, bool> &getDevices() { return devices; }

[[nodiscard]] int8_t Device::getPort() const { return this->port; }

[[nodiscard]] const char *Device::getTypeName() const { return this->typeName; }

[[nodiscard]] const char *Device::getName() const { return this->name; }

[[noreturn]] void reconfigureTask([[maybe_unused]] void *params) {
  pros::c::delay(Device::CONFIGURE_RATE * 2); // printing race condition?
  logger::info("Device reconfigure task started.");
  while (true) {
    if (!pendingDevices.empty()) {
      for (Device *device : pendingDevices) {
        if (device->isConnected()) {
          logger::debug("%s '%s' connected on port %i", device->getTypeName(), device->getName(), device->getPort());
          devices.emplace(device, true);
        } else {
          devices.emplace(device, false);
          logger::warn("No device on port %i (expected %s '%s').", device->getPort(), device->getTypeName(),
                       device->getName());
        }
      }
      pendingDevices.clear();
    }

    for (auto &pair : devices) {
      bool connected = pair.first->isConnected();
      if (pair.second && !connected) {
        pair.second = false;
        logger::error("%s '%s' (port %i) disconnected.", pair.first->getTypeName(), pair.first->getName(),
                      pair.first->getPort());
      } else if (!pair.second && connected) {
        logger::warn("%s '%s' (port %i) reconnected. Reconfiguring...", pair.first->getTypeName(),
                     pair.first->getName(), pair.first->getPort());
        pair.second = true;
        pair.first->reconfigure();
      }
      pros::c::delay(Device::CONFIGURE_RATE);
    }
  }
}
} // namespace robot::device

template <> struct std::hash<robot::device::Device> {
  std::size_t operator()(const robot::device::Device &device) const { return device.getPort(); }
};
