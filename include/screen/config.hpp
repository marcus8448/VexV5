#ifndef SCREEN_CONFIG_HPP
#define SCREEN_CONFIG_HPP

#include "screen.hpp"

namespace screen {
class Configuration : public Screen {
public:
  explicit Configuration();

  void create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) override;
  void initialize(lv_coord_t width, lv_coord_t height) override;
  void update(robot::Robot *robot) override;
  void destroy(lv_obj_t *screen) override;
};
} // namespace screen
#endif // SCREEN_CONFIG_HPP
