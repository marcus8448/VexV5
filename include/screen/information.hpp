#ifndef SCREEN_INFORMATION_HPP
#define SCREEN_INFORMATION_HPP

#include "robot/robot.hpp"
#include "screen.hpp"

namespace screen {
constexpr size_t INFO_COLUMNS = 12;

class Information : public Screen {
private:
  std::array<lv_obj_t *, INFO_COLUMNS> leftColumn = {};
  std::array<lv_obj_t *, INFO_COLUMNS> rightColumn = {};

public:
  explicit Information(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height);
  ~Information() override;

  void update() override;
};
} // namespace screen

#endif // SCREEN_INFORMATION_HPP
