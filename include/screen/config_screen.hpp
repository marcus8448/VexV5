#ifndef SCREEN_CONFIG_HPP
#define SCREEN_CONFIG_HPP

#include "screen.hpp"

namespace screen {
lv_res_t switch_alliance_colour([[maybe_unused]] lv_obj_t *btn);
lv_res_t switch_drive_scheme([[maybe_unused]] lv_obj_t *btn);

class ConfigurationScreen : public Screen {
private:
  lv_obj_t *driveSchemeBtn = nullptr;
  lv_obj_t *driveSchemeBtnLbl = nullptr;
  lv_obj_t *allianceColourBtn = nullptr;
  lv_obj_t *allianceColourBtnLbl = nullptr;
  bool styleSet = false;

public:
  explicit ConfigurationScreen();

  void create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) override;
  void update(robot::Robot &robot) override;
  void update_drive_scheme_label();
  void update_alliance_colour_label();
};
} // namespace screen
#endif // SCREEN_CONFIG_HPP
