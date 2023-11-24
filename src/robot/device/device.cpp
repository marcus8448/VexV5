#include "robot/device/device.hpp"
#include "debug/logger.hpp"
#include "pros/rtos.h"
#include "tasks.hpp"
#include <vector>

namespace robot::device {
static std::vector<Device *> pendingDevices;
static std::unordered_map<Device *, bool> devices;

[[noreturn]] void reconfigureTask([[maybe_unused]] void *params);

Device::Device(std::string_view typeName, std::string_view name, int8_t port)
    : typeName(typeName), name(name), port(port) {
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

[[nodiscard]] std::string_view Device::getTypeName() const { return this->typeName; }

[[nodiscard]] std::string_view Device::getName() const { return this->name; }

[[noreturn]] void reconfigureTask([[maybe_unused]] void *params) {
  logger::info("Device reconfigure task started.");
  while (true) {
    if (!pendingDevices.empty()) {
      for (Device *device : pendingDevices) {
        if (device->isConnected()) {
          logger::debug("{} '{}' connected on port {}", device->getTypeName(), device->getName(), device->getPort());
          devices.emplace(device, true);
        } else {
          devices.emplace(device, false);
          logger::warn("No device on port {} (expected {} '{}').", std::abs(device->getPort()), device->getTypeName(),
                       device->getName());
        }
      }
      pendingDevices.clear();
    }

    for (auto &[device, prevConnect] : devices) {
      if (const bool connected = device->isConnected(); prevConnect && !connected) {
        prevConnect = false;
        logger::error("{} '{}' (port {}) disconnected.", device->getTypeName(), device->getName(),
                      std::abs(device->getPort()));
      } else if (!prevConnect && connected) {
        logger::warn("{} '{}' (port {}) reconnected. Reconfiguring...", device->getTypeName(), device->getName(),
                     std::abs(device->getPort()));
        prevConnect = true;
        device->reconfigure();
      }
      pros::c::delay(Device::CONFIGURE_RATE);
    }
  }
}
} // namespace robot::device

template <> struct std::hash<robot::device::Device> {
  std::size_t operator()(const robot::device::Device &device) const noexcept { return device.getPort(); }
};
