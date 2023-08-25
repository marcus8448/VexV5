#include "screen/chart.hpp"
#include "debug/logger.hpp"
#include "format.hpp"
#include "screen/colour.hpp"
#include "screen/screen.hpp"
#include <cstring>

namespace screen {

DataSet::DataSet(const char *label, lv_color_t color, float (*function)(robot::Robot &))
    : label(label), color(color), function(function) {}

template <size_t Sets, size_t Points>
Chart<Sets, Points>::Chart(robot::Robot &robot, const char *title, DataSet dataSets[Sets])
    : robot(robot), title(title) {
  this->dataSets = dataSets;
}

template <size_t Sets, size_t Points> void Chart<Sets, Points>::initialize(lv_obj_t *screen) {
  this->canvasBuffer = std::malloc(LV_CANVAS_BUF_SIZE_TRUE_COLOR(SCREEN_WIDTH, SCREEN_HEIGHT));
  this->drivetrainCanvas = lv_canvas_create(screen);
  lv_canvas_set_buffer(this->drivetrainCanvas, this->canvasBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, LV_IMG_CF_TRUE_COLOR);

  this->titleLabel = lv_label_create(screen);
  lv_label_set_text(this->titleLabel, this->title);

  lv_obj_set_pos(this->titleLabel, SCREEN_WIDTH / 2 - lv_obj_get_width(this->titleLabel) / 2, 0);
}

template <size_t Sets, size_t Points> void Chart<Sets, Points>::update() {
  std::memset(this->canvasBuffer, 0x00000000, LV_CANVAS_BUF_SIZE_TRUE_COLOR(SCREEN_WIDTH, SCREEN_HEIGHT));

  lv_point_t points[Points];
  lv_draw_line_dsc_t lineDesc;
  lv_draw_line_dsc_init(&lineDesc);
  lv_draw_label_dsc_t textDesc;
  lv_draw_label_dsc_init(&textDesc);
  lineDesc.color = textDesc.color = colour::WHITE;

  float max = -INFINITY;
  float min = INFINITY;
  for (size_t i = 0; i < Sets; ++i) {
    max = std::max(max, this->data[i].max);
    min = std::min(min, this->data[i].min);
  }

  if (max == -INFINITY)
    max = 0;
  if (min == INFINITY)
    min = 0;

  if (max == min) {
    max += 1.0f;
  }

  double heightScale = (SCREEN_HEIGHT - 17 - 41) / (max - min);
  double widthScale = SCREEN_WIDTH / static_cast<double>(Points);

  lv_coord_t zero = SCREEN_HEIGHT - (0 - min) * heightScale - 41;
  lv_point_t pts[2]{{0, zero}, {SCREEN_WIDTH, zero}};
  lv_canvas_draw_line(this->drivetrainCanvas, pts, 2, &lineDesc);

  const char *str = fmt::static_format("%f", max);
  lv_canvas_draw_text(this->drivetrainCanvas, 0, 0, 100, &textDesc, str);
  str = fmt::static_format("%f", min);
  lv_canvas_draw_text(this->drivetrainCanvas, 40, SCREEN_HEIGHT - 36, 100, &textDesc, str);

  for (size_t i = 0; i < Sets; ++i) {
    DataSet set = this->dataSets[i];
    float value = set.function(this->robot);
    if (value == INFINITY) {
      value = 0;
    }

    this->data[i].add(value);

    for (size_t j = 0; j < Points; j++) {
      points[j].x = static_cast<lv_coord_t>(SCREEN_WIDTH - (j * widthScale));
      points[j].y =
          static_cast<lv_coord_t>(SCREEN_HEIGHT - (this->data[i].get(Points - 1 - j) - min) * heightScale - 41);
    }

    textDesc.color = lineDesc.color = set.color;
    lv_canvas_draw_line(this->drivetrainCanvas, points, Points, &lineDesc);

    str = fmt::static_format("%s: %f", set.label, value);
    lv_canvas_draw_text(this->drivetrainCanvas, 40 + i * ((SCREEN_WIDTH - (40 * 2)) / Sets), SCREEN_HEIGHT - 16,
                        (SCREEN_WIDTH - (40 * 2)) / Sets, &textDesc, str);
  }
}

template <size_t Sets, size_t Points> void Chart<Sets, Points>::cleanup() {
  free(this->canvasBuffer);
  lv_obj_del_async(this->drivetrainCanvas);
  lv_obj_del_async(this->titleLabel);

  this->canvasBuffer = nullptr;

  this->drivetrainCanvas = nullptr;
  this->titleLabel = nullptr;
}

template class Chart<4, 100>;
template class Chart<2, 100>;
} // namespace screen
