#ifndef SCREEN_BANNER_HPP
#define SCREEN_BANNER_HPP

#include "screen/screen.hpp"

namespace screen {
class Banner : public Screen {
private:
  lv_obj_t *imageCanvas = nullptr;

public:
  explicit Banner();

  void create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) override;
  void update(robot::Robot *robot) override;
};
} // namespace screen

#endif // SCREEN_BANNER_HPP
