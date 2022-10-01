#include <cmath>
#include "screen/screen.hpp"
#include "screen/lvgl_util.hpp"
#include "screen/flywheel_chart.hpp"
#include "screen/colour.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_canvas.h"
#pragma GCC diagnostic pop

namespace screen {
FlywheelChart::FlywheelChart() = default;

void FlywheelChart::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
  this->velFlywheel.reserve(100);
  this->flywheelCanvas = lv_canvas_create(screen, nullptr);
  lv_obj_set_pos(this->flywheelCanvas, 0, 0);
  lv_obj_set_size(this->flywheelCanvas, width, static_cast<lv_coord_t>(height - BASE_HEIGHT));
  lv_canvas_set_buffer(this->flywheelCanvas, canvasBuffer, width, static_cast<lv_coord_t>(height - BASE_HEIGHT), CANVAS_COLOUR);

  create_label(screen,
               32,
               static_cast<lv_coord_t>(height - 32),
               static_cast<lv_coord_t>(width / 4), 16, "Flywheel (+)",
               create_text_color_style(screen::colour::BLUE)
  );
  create_label(screen,
               32,
               static_cast<lv_coord_t>(height - 16),
               static_cast<lv_coord_t>(width / 4),
               16, "Flywheel (-)",
               create_text_color_style(screen::colour::ORANGE)
  );
}

void FlywheelChart::initialize(lv_coord_t width, lv_coord_t height) {
  this->canvasWidth = static_cast<float>(width);
  this->canvasHeight = static_cast<float>(height - BASE_HEIGHT);
}

void FlywheelChart::update(robot::Robot *robot) {
  if (this->velFlywheel.size() == 100) {
    this->velFlywheel.erase(this->velFlywheel.begin());
  }

  auto prev = (float) robot->flywheel->motor->get_actual_velocity();
  if (prev == INFINITY || prev == -1) {
    prev = 5;
  }
  this->velFlywheel.push_back(prev);
  float heightScale = this->canvasHeight / 900.0f;
  float widthScale = this->canvasWidth / 100.0f;

  float x = 0;
  for (signed int i = (signed int) this->velFlywheel.size() - 2; i >= 0; --i) {
    float v = this->velFlywheel[i];
    lv_canvas_draw_line(
        flywheelCanvas, lv_point_t{
            static_cast<lv_coord_t>(this->canvasWidth - (x * widthScale)),
            static_cast<lv_coord_t>(this->canvasHeight - std::fabs(prev * heightScale))
        }, lv_point_t{
            static_cast<lv_coord_t>(this->canvasWidth - ((x + 1) * widthScale)),
            static_cast<lv_coord_t>((this->canvasHeight - std::fabs(v * heightScale)))
        }, v >= 0 ? screen::colour::BLUE : screen::colour::ORANGE);
    ++x;
    prev = v;
  }
  lv_obj_invalidate(this->flywheelCanvas);
}

void FlywheelChart::destroy(lv_obj_t *screen) {
  this->velFlywheel.clear();
}
} // screen
