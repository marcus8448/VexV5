#ifndef SCREEN_SERIAL_SCREEN
#define SCREEN_SERIAL_SCREEN

#include "screen/screen.hpp"

namespace screen {
class SerialScreen : public Screen {
private:
  lv_obj_t *ipLabel = nullptr;
  lv_obj_t *connectionState = nullptr;

public:
  explicit SerialScreen();

  void create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) override;
  void update(robot::Robot &robot) override;
};

extern SerialScreen *serial_screen;
} // namespace screen

#endif
