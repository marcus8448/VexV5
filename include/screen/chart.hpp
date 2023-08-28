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
  robot::Robot &robot;
  const char *title;

  DataSet *dataSets = nullptr;
  structure::FixedQueue<Points> data[Sets];

  lv_color_t *canvasBuffer = nullptr;
  lv_obj_t *canvas = nullptr;
  lv_obj_t *titleLabel = nullptr;

public:
  explicit Chart(robot::Robot &robot, const char *title, DataSet dataSets[Sets]);

  void initialize(lv_obj_t *screen) override;
  void update() override;
  void cleanup() override;
};
} // namespace screen

#endif // SCREEN_CHART_HPP
