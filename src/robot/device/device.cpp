#include "robot/device/device.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"
#include <algorithm>
#include <cerrno>
#include <map>
#include <vector>

namespace robot::device {
static bool initialized;
static std::map<Device *, bool> devices;

[[noreturn]] void reconfigure_task(void *params);

Device::Device(uint8_t port): port(port) {
  devices.emplace(this, true);
  if (!initialized) {
    initialized = true;
    pros::Task(reconfigure_task, nullptr, "Device reconfigure task");
  }
}

bool Device::checkConnect() const {
  bool b = errno != ENODEV;
  errno = 0;
  return b;
}

[[nodiscard]] uint8_t Device::get_port() const {
  return this->port;
}

[[noreturn]] void reconfigure_task(void *params) {
  pros::delay(2500);
  logger::info("Device reconfigure task started.");
  bool init = false;
  while (true) {
    for (auto &pair : devices) {
      bool connected = pair.first->is_connected();
      if (pair.second && !connected) {
        pair.second = false;
        if (init) {
          logger::warn("Device on port %i disconnected.", pair.first->get_port());
        } else {
          logger::warn("No device on port %i.", pair.first->get_port());
        }
      } else if (!pair.second && connected) {
        logger::info("Device on port %i reconnected. Reconfiguring...", pair.first->get_port());
        pair.second = true;
        pair.first->reconfigure();
      }
      pros::delay(100);
    }
    init = true;
  }
}
} // namespace robot::device
