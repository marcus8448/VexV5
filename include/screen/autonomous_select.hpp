#ifndef SCREEN_AUTONOMOUS_SELECT_HPP
#define SCREEN_AUTONOMOUS_SELECT_HPP

#include "robot/robot.hpp"
#include "screen.hpp"

namespace screen {
class AutonomousSelect : public Screen {
private:
  robot::Robot &robot;

  lv_obj_t *list = nullptr;
  lv_obj_t *selected = nullptr;

public:
  explicit AutonomousSelect(robot::Robot &robot);

  void initialize(lv_obj_t *screen) override;
  void update() override;
  void cleanup() override;

  void click(lv_obj_t *btn);
};

extern AutonomousSelect *autonomous_select_instance;
} // namespace screen
#endif // SCREEN_AUTONOMOUS_SELECT_HPP
