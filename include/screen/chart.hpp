#ifndef SCREEN_CHART_HPP
#define SCREEN_CHART_HPP

#include "robot/robot.hpp"
#include "screen.hpp"
#include "structure/fixed_queue.hpp"
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
private:
  const char *title;

  DataSet *dataSets = nullptr;
  structure::FixedQueue<Points> data[Sets];

  lv_color_t *canvasBuffer = nullptr;
  lv_obj_t *canvas = nullptr;
  lv_obj_t *titleLabel = nullptr;

public:
  explicit Chart(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height, const char *title,
                 DataSet dataSets[Sets]);
  ~Chart() override;

  void update() override;
};
} // namespace screen
#endif // SCREEN_CHART_HPP
