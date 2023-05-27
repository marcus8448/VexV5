#ifndef SCREEN_GRAPH_HPP
#define SCREEN_GRAPH_HPP

#include "robot/robot.hpp"
#include "screen.hpp"
#include "structure/fixed_queue.hpp"

namespace screen {

template <typename T>
concept Number = requires(T a) {
  T() == 0;
  a * 100.0f;
  a / 100.0f;
  a + a;
};

template <int INTEGER>
concept Max4 = INTEGER > 0 && INTEGER <= 4;

template <Number Num> struct DataProvider {
  const char *name;
  const lv_color_t colour;

  Num (*function)(robot::Robot &);
};

template <int PROVIDERS, Number Num>
  requires Max4<PROVIDERS>
class Graph : public Screen {
private:
  const char *title;
  const char *xLabel;
  const char *yLabel;

  const Num max;
  const Num min;

  const Num heightScale;
  const Num widthScale;

  DataProvider<Num> dataProviders[PROVIDERS];
  structure::FixedQueue<Num, 100> queues[PROVIDERS] = {};

protected:
  const robot::Robot &robot;
  lv_obj_t *canvas = nullptr;

public:
  explicit Graph(robot::Robot &robot, const char *title, const char *xLabel, const char *yLabel, Num max, Num min,
                 DataProvider<Num> dataProviders[PROVIDERS]);

  void initialize(lv_obj_t *screen) override;
  void update() override;
};

template <int PROVIDERS, Number Num>
  requires Max4<PROVIDERS>
Graph<PROVIDERS, Num>::Graph(robot::Robot &robot, const char *title, const char *xLabel, const char *yLabel,
                             const Num max, const Num min, DataProvider<Num> dataProviders[PROVIDERS])
    : title(title), xLabel(xLabel), yLabel(yLabel), max(max), min(min), heightScale(CANVAS_HEIGHT / (max - min)),
      widthScale(CANVAS_WIDTH / static_cast<float>(100)), robot(robot), dataProviders(dataProviders) {
  for (auto &item : queues) {
    item = structure::FixedQueue<Num, 100>(Num());
  }
}

template <int PROVIDERS, Number Num>
  requires Max4<PROVIDERS>
void Graph<PROVIDERS, Num>::initialize(lv_obj_t *screen) {
  this->canvas = create_canvas(screen);
  lv_obj_t *titleLabel = create_label(screen, SCREEN_HALF_WIDTH - 30, 15, 60, 20, this->title);
  lv_label_set_align(titleLabel, LV_LABEL_ALIGN_CENTER);
  //  lv_obj_t *xAxisLabel = create_label(screen, SCREEN_HALF_WIDTH - 30, SCREEN_HEIGHT - 20, 60, 20, this->xLabel);
  //  lv_label_set_align(xAxisLabel, LV_LABEL_ALIGN_CENTER);
  //  lv_obj_t *yAxisLabel = create_label(screen, SCREEN_HALF_WIDTH - 30, SCREEN_HEIGHT - 40, 60, 20, this->yLabel);
  //  lv_label_set_align(yAxisLabel, LV_LABEL_ALIGN_CENTER);
}

template <int PROVIDERS, Number Num>
  requires Max4<PROVIDERS>
void Graph<PROVIDERS, Num>::update() {
  for (auto i = 0; i < PROVIDERS; i++) {
    DataProvider<Num> provider = dataProviders[i];
    Num value = provider.function(this->robot);
    if (value > max) {
      value = max;
    } else if (value > min) {
      value = min;
    }

    structure::FixedQueue<Num, 100> queue = this->queues[i];
    queue.pushPop(value);

    float x = 0.0f;
    for (auto j = static_cast<int32_t>(queue.size()) - 1; j >= 0; --j) {
      float v = this->velMotorLF[i];
      lv_canvas_draw_line(
          this->drivetrainCanvas,
          lv_point_t{static_cast<lv_coord_t>(CANVAS_WIDTH - (x * widthScale)),
                     static_cast<lv_coord_t>(CANVAS_HEIGHT - std::fabs(value * heightScale) + (min * heightScale))},
          lv_point_t{static_cast<lv_coord_t>(CANVAS_WIDTH - ((x + 1) * widthScale)),
                     static_cast<lv_coord_t>(CANVAS_HEIGHT - std::fabs(v * heightScale) + (min * heightScale))},
          provider.colour);
      value = v;
      ++x;
    }
  }
  lv_obj_invalidate(this->canvas);
}
} // namespace screen

#endif // SCREEN_GRAPH_HPP
