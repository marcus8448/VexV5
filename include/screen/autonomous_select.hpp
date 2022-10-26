#ifndef SCREEN_AUTONOMOUS_SELECT_HPP
#define SCREEN_AUTONOMOUS_SELECT_HPP

#include "screen/screen.hpp"

namespace screen {
class AutonomousSelect : public Screen {
private:
  lv_obj_t *selections = nullptr;
  lv_obj_t *selected = nullptr;

public:
  explicit AutonomousSelect();

  void create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) override;
  void update(robot::Robot *robot) override;

  void click(lv_obj_t *btn);
};
} // namespace screen
#endif // SCREEN_AUTONOMOUS_SELECT_HPP
