#include "screen/serial_screen.hpp"

namespace screen {
SerialScreen::SerialScreen() { serial_screen = this; }

void SerialScreen::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {}

void SerialScreen::update(robot::Robot &robot) {}

SerialScreen *serial_screen = nullptr;
} // namespace screen
