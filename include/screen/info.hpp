#ifndef SCREEN_INFO_HPP
#define SCREEN_INFO_HPP

#include "screen.hpp"

namespace screen {
class Information : public Screen {
private:
  lv_obj_t *uptimeLabel = nullptr;
  lv_obj_t *controlSchemeLabel = nullptr;
  lv_obj_t *motorLFLabel = nullptr;
  lv_obj_t *motorRFLabel = nullptr;
  lv_obj_t *motorLBLabel = nullptr;
  lv_obj_t *motorRBLabel = nullptr;
  lv_obj_t *flywheelLabel = nullptr;
  lv_obj_t *flywheelTempLabel = nullptr;
  lv_obj_t *indexerLabel = nullptr;
  lv_obj_t *intakeLabel = nullptr;
  lv_obj_t *digitalSpeedLabel = nullptr;

public:
  explicit Information();

  void create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) override;
  void update(robot::Robot *robot) override;
};
} // namespace screen

#endif // SCREEN_INFO_HPP
