#include "screen/chart.hpp"
#include "format.hpp"
#include "screen/screen.hpp"

namespace screen {
DataSet::DataSet(const char *label, lv_color_t color, float (*function)(const robot::Robot &))
    : label(label), color(color), function(function) {}

template <size_t Sets, size_t Points>
Chart<Sets, Points>::Chart(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height,
                           const char *title, std::array<DataSet, Sets> dataSets)
    : Screen(robot, width, height), title(title), dataSets(dataSets),
      canvasBuffer(new lv_color_t[LV_CANVAS_BUF_SIZE_TRUE_COLOR(width, height)]), canvas(lv_canvas_create(screen)),
      titleLabel(lv_label_create(screen)) {
  static_assert(Points > 1);
  static_assert(Sets > 0);

  for (size_t i = 0; i < Sets; i++) {
    this->data.at(i) = structure::FixedQueue<Points>();
  }

  lv_canvas_set_buffer(this->canvas.get(), this->canvasBuffer.get(), width, height, LV_IMG_CF_TRUE_COLOR);

  lv_label_set_text(this->titleLabel.get(), this->title);

  lv_obj_set_pos(this->titleLabel.get(), width / 2 - lv_obj_get_width(this->titleLabel.get()) / 2, 0);
}

template <size_t Sets, size_t Points> void Chart<Sets, Points>::update() {
  lv_canvas_fill_bg(this->canvas.get(), lv_color_black(), 255);

  std::array<lv_point_t, Points> points = {};
  for (size_t i = 0; i < Points; ++i) {
    points[i] = lv_point_t(0, 0);
  }

  lv_draw_line_dsc_t lineDesc;
  lv_draw_label_dsc_t textDesc;
  lv_draw_line_dsc_init(&lineDesc);
  lv_draw_label_dsc_init(&textDesc);
  lineDesc.color = textDesc.color = lv_color_white();

  float max = -std::numeric_limits<float>::infinity();
  float min = std::numeric_limits<float>::infinity();
  for (size_t i = 0; i < Sets; ++i) {
    max = std::max(max, this->data[i].max);
    min = std::min(min, this->data[i].min);
  }

  if (std::abs(max) == std::numeric_limits<float>::infinity())
    max = 0;
  if (std::abs(min) == std::numeric_limits<float>::infinity())
    min = 0;

  if (max <= min) {
    max = min + 1.0f;
  }

  const double heightScale = (height - 17 - 41) / (max - min);
  const double widthScale = width / static_cast<double>(Points);

  lv_coord_t zero = height - (0 - min) * heightScale - 41;
  points[0] = {0, zero};
  points[1] = {width, zero};

  lv_canvas_draw_line(this->canvas.get(), points.data(), 2, &lineDesc);

  std::string str = fmt::string_format("{}", max);
  lv_canvas_draw_text(this->canvas.get(), 0, 0, 100, &textDesc, str.c_str());
  str = fmt::string_format("{}", min);
  lv_canvas_draw_text(this->canvas.get(), 40, height - 36, 100, &textDesc, str.c_str());

  for (size_t i = 0; i < Sets; ++i) {
    DataSet &set = this->dataSets[i];
    float value = set.function(this->robot);
    if (value == std::numeric_limits<float>::infinity()) {
      value = 0;
    }

    this->data[i].add(value);

    for (size_t j = 0; j < Points; j++) {
      points[j].x = coord(width - j * widthScale);
      points[j].y = coord(height - (this->data[i].get(Points - 1 - j) - min) * heightScale - 41);
    }

    textDesc.color = lineDesc.color = set.color;
    lv_canvas_draw_line(this->canvas.get(), points.data(), Points, &lineDesc);

    str = fmt::string_format("{}: {}", set.label, value);
    lv_canvas_draw_text(this->canvas.get(), 40 + i * ((width - 40 * 2) / Sets), height - 16, (width - 40 * 2) / Sets,
                        &textDesc, str.c_str());
  }
}

template class Chart<2, 100>;
} // namespace screen
