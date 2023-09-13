#ifndef SCREEN_INFO_HPP
#define SCREEN_INFO_HPP

#include "robot/robot.hpp"
#include "screen.hpp"

#define INFO_COLUMNS 12

namespace screen {
class Information : public Screen {
private:
  lv_obj_t **leftColumn = static_cast<lv_obj_t **>(calloc(sizeof(lv_obj_t *), INFO_COLUMNS));
  lv_obj_t **rightColumn = static_cast<lv_obj_t **>(calloc(sizeof(lv_obj_t *), INFO_COLUMNS));

public:
  explicit Information(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height);
  ~Information() override;

  void update() override;
};
} // namespace screen

#endif // SCREEN_INFO_HPP
