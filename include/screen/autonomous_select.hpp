#ifndef SCREEN_AUTONOMOUS_SELECT_HPP
#define SCREEN_AUTONOMOUS_SELECT_HPP

#include "robot/robot.hpp"
#include "screen.hpp"

namespace screen {
class AutonomousSelect : public Screen {
private:
  lv_obj_t *list = nullptr;
  lv_obj_t *selected = nullptr;

public:
  explicit AutonomousSelect(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height);
  ~AutonomousSelect() override;

  void update() override;

  void click(lv_event_t *event);
};
} // namespace screen
#endif // SCREEN_AUTONOMOUS_SELECT_HPP
