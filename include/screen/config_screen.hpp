#ifndef SCREEN_CONFIG_HPP
#define SCREEN_CONFIG_HPP

#include "robot/robot.hpp"
#include "screen.hpp"

namespace screen {
void switch_drive_scheme([[maybe_unused]] lv_event_t *btn);

class ConfigurationScreen : public Screen {
private:
  lv_obj_t *driveSchemeBtn = nullptr;
  lv_obj_t *driveSchemeBtnLbl = nullptr;

public:
  robot::Robot &robot;

  explicit ConfigurationScreen(robot::Robot &robot);

  void initialize(lv_obj_t *screen) override;
  void update() override;
  void cleanup() override;

  void update_drive_scheme_label();
};
} // namespace screen
#endif // SCREEN_CONFIG_HPP
