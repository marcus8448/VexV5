#ifndef SCREEN_CHART_HPP
#define SCREEN_CHART_HPP

#include "robot/robot.hpp"
#include "screen.hpp"
#include "structure/fixed_queue.hpp"
#include <array>
#include <functional>

namespace screen {
class DataSet {
public:
  const char *label;
  lv_color_t color;
  std::function<float(robot::Robot &)> function;

public:
  explicit DataSet(const char *label, lv_color_t color, std::function<float(robot::Robot &)> function);
};

template <size_t Sets, size_t Points> class Chart : public Screen {
  const char *title;

  std::array<DataSet, Sets> dataSets;
  std::array<structure::FixedQueue<Points>, Sets> data = {};

  lv_color_t *canvasBuffer;
  lv_obj_t *canvas;
  lv_obj_t *titleLabel;

public:
  explicit Chart(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height, const char *title,
                 std::array<DataSet, Sets> dataSets);
  ~Chart() override;

  void update() override;
};
} // namespace screen
#endif // SCREEN_CHART_HPP
