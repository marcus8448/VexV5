#ifndef SCREEN_INFO_HPP
#define SCREEN_INFO_HPP

#include "robot/robot.hpp"
#include "screen.hpp"

namespace screen {
class Information : public Screen {
private:
  robot::Robot &robot;

  lv_obj_t *uptimeLabel = nullptr;
  lv_obj_t *controlSchemeLabel = nullptr;
  //  lv_obj_t *teamColourLabel = nullptr;
  lv_obj_t *motorLFLabel = nullptr;
  lv_obj_t *motorRFLabel = nullptr;
  lv_obj_t *motorLBLabel = nullptr;
  lv_obj_t *motorRBLabel = nullptr;
  //  lv_obj_t *_unused0 = nullptr;
  //  lv_obj_t *_unused1 = nullptr;
  //  lv_obj_t *_unused2 = nullptr;
  //  lv_obj_t *_unused3 = nullptr;
  //  lv_obj_t *_unused4 = nullptr;
  //  lv_obj_t *_unused5 = nullptr;
  lv_obj_t *digitalSpeed = nullptr;

public:
  explicit Information(robot::Robot &robot);

  void initialize(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) override;
  void update() override;
};
} // namespace screen

#endif // SCREEN_INFO_HPP
