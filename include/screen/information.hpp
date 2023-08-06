#ifndef SCREEN_INFO_HPP
#define SCREEN_INFO_HPP

#include "robot/robot.hpp"
#include "screen.hpp"

#define INFO_COLUMNS 12

namespace screen {
class Information : public Screen {
private:
  robot::Robot &robot;

  lv_obj_t **leftColumn = static_cast<lv_obj_t **>(calloc(sizeof(lv_obj_t *), INFO_COLUMNS));
  lv_obj_t **rightColumn = static_cast<lv_obj_t **>(calloc(sizeof(lv_obj_t *), INFO_COLUMNS));

public:
  explicit Information(robot::Robot &robot);

  void initialize(lv_obj_t *screen) override;
  void update() override;
  void cleanup() override;
};
} // namespace screen

#endif // SCREEN_INFO_HPP
