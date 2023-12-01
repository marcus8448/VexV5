#ifndef SCREEN_INFORMATION_HPP
#define SCREEN_INFORMATION_HPP

#include "robot/robot.hpp"
#include "screen.hpp"

namespace screen {
constexpr int INFO_COLUMNS = 12;

class Information final : public Screen {
  std::array<lv_obj, INFO_COLUMNS> leftColumn = {};
  std::array<lv_obj, INFO_COLUMNS> rightColumn = {};

public:
  explicit Information(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height);

  void update() override;
};
} // namespace screen

#endif // SCREEN_INFORMATION_HPP
