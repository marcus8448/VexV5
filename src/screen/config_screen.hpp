#ifndef SCREEN_CONFIG_SCREEN_HPP
#define SCREEN_CONFIG_SCREEN_HPP

#include "robot/robot.hpp"
#include "screen.hpp"

namespace screen {
class ConfigurationScreen final : public Screen {
  lv_obj driveSchemeBtn;
  lv_obj_t *driveSchemeBtnLbl;

public:
  explicit ConfigurationScreen(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height);

  void update() override;

  void click();
};
} // namespace screen
#endif // SCREEN_CONFIG_SCREEN_HPP
