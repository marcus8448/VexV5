#ifndef SCREEN_CONFIG_HPP
#define SCREEN_CONFIG_HPP

#include "robot/robot.hpp"
#include "screen.hpp"

namespace screen {
class ConfigurationScreen : public Screen {
private:
  lv_obj_t *driveSchemeBtn = nullptr;
  lv_obj_t *driveSchemeBtnLbl = nullptr;

public:
  explicit ConfigurationScreen(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height);
  ~ConfigurationScreen() override;

  void update() override;

  void click();
};
} // namespace screen
#endif // SCREEN_CONFIG_HPP
