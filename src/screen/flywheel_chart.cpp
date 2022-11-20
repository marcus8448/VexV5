#include "screen/flywheel_chart.hpp"
#include "screen/colour.hpp"
#include "screen/lvgl_util.hpp"
#include "screen/screen.hpp"
#include <cmath>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_canvas.h"
#pragma GCC diagnostic pop

namespace screen {
extern bool *enableCanvas;
extern void *canvasBuffer;

FlywheelChart::FlywheelChart() { *enableCanvas = true; }

void FlywheelChart::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
  this->canvasWidth = static_cast<float>(width);
  auto trueHeight = static_cast<lv_coord_t>(height - BASE_HEIGHT);
  this->canvasHeight = static_cast<float>(trueHeight);
  this->velFlywheel.reserve(100);
  this->flywheelCanvas = lv_canvas_create(screen, nullptr);
  lv_obj_set_pos(this->flywheelCanvas, 0, 0);
  lv_obj_set_size(this->flywheelCanvas, width, trueHeight);
  lv_canvas_set_buffer(this->flywheelCanvas, canvasBuffer, width, trueHeight, CANVAS_COLOUR);

  create_label(screen, 90, static_cast<lv_coord_t>(height - 32 - 4), static_cast<lv_coord_t>(width / 3), 16, "Flywheel",
               create_text_color_style(screen::colour::BLUE));
}

void FlywheelChart::update(robot::Robot &robot) {
  if (this->velFlywheel.size() == 100) {
    this->velFlywheel.erase(this->velFlywheel.begin());
  }

  auto prev = std::fabs(static_cast<float>(robot.flywheel->getVelocity()));
  if (prev == INFINITY || prev == -1) {
    prev = 5;
  }
  this->velFlywheel.push_back(prev);
  float heightScale = this->canvasHeight / 900.0f;
  float widthScale = this->canvasWidth / 100.0f;

  float x = 0;
  for (signed int i = static_cast<int32_t>(this->velFlywheel.size()) - 2; i >= 0; --i) {
    float v = std::fabs(this->velFlywheel[i]);
    lv_canvas_draw_line(flywheelCanvas,
                        lv_point_t{static_cast<lv_coord_t>(this->canvasWidth - (x * widthScale)),
                                   static_cast<lv_coord_t>(this->canvasHeight - (prev * heightScale))},
                        lv_point_t{static_cast<lv_coord_t>(this->canvasWidth - ((x + 1) * widthScale)),
                                   static_cast<lv_coord_t>(this->canvasHeight - (v * heightScale))},
                        screen::colour::BLUE);
    ++x;
    prev = v;
  }
  lv_obj_invalidate(this->flywheelCanvas);
}
} // namespace screen
