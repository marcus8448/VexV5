#include "screen/drivetrain_chart.hpp"
#include "logger.hpp"
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

DrivetrainChart::DrivetrainChart() {
  *enableCanvas = true;
};

void DrivetrainChart::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
  auto trueHeight = static_cast<lv_coord_t>(height - BASE_HEIGHT);
  this->drivetrainCanvas = lv_canvas_create(screen, nullptr);
  this->velMotorLF.reserve(100);
  this->velMotorRF.reserve(100);
  this->velMotorLB.reserve(100);
  this->velMotorRB.reserve(100);
  lv_obj_set_pos(this->drivetrainCanvas, 0, 0);
  lv_obj_set_size(this->drivetrainCanvas, width, trueHeight);
  lv_canvas_set_buffer(this->drivetrainCanvas, canvasBuffer, width, trueHeight, CANVAS_COLOUR);

  auto qtrWidth = static_cast<lv_coord_t>((width - 64) / 4);
  create_label(screen, 16 + 32, static_cast<lv_coord_t>(height - 32 - 6), qtrWidth, 16, "LF (+)",
               create_text_color_style(screen::colour::RED));
  create_label(screen, 16 + 32, static_cast<lv_coord_t>(height - 16 - 6), qtrWidth, 16, "LF (-)",
               create_text_color_style(screen::colour::BLUE));

  create_label(screen, static_cast<lv_coord_t>(16 + 32 + qtrWidth), static_cast<lv_coord_t>(height - 32 - 6), qtrWidth,
               16, "RF (+)", create_text_color_style(screen::colour::ORANGE));
  create_label(screen, static_cast<lv_coord_t>(16 + 32 + qtrWidth), static_cast<lv_coord_t>(height - 16 - 6), qtrWidth,
               16, "RF (-)", create_text_color_style(screen::colour::VIOLET));

  create_label(screen, static_cast<lv_coord_t>(16 + 32 + qtrWidth * 2), static_cast<lv_coord_t>(height - 32 - 6),
               qtrWidth, 16, "LB (+)", create_text_color_style(screen::colour::YELLOW));
  create_label(screen, static_cast<lv_coord_t>(16 + 32 + qtrWidth * 2), static_cast<lv_coord_t>(height - 16 - 6),
               qtrWidth, 16, "LB (-)", create_text_color_style(screen::colour::GREEN));

  create_label(screen, static_cast<lv_coord_t>(16 + 32 + qtrWidth * 3), static_cast<lv_coord_t>(height - 32 - 6),
               qtrWidth, 16, "RB (+)", create_text_color_style(screen::colour::PINK));
  create_label(screen, static_cast<lv_coord_t>(16 + 32 + qtrWidth * 3), static_cast<lv_coord_t>(height - 16 - 6),
               qtrWidth, 16, "RB (-)", create_text_color_style(screen::colour::LIGHT_BLUE));
}

void DrivetrainChart::initialize(lv_coord_t width, lv_coord_t height) {
  this->canvasWidth = static_cast<float>(width);
  this->canvasHeight = static_cast<float>(height - BASE_HEIGHT);
}

void DrivetrainChart::update(robot::Robot *robot) {
  if (this->velMotorLF.size() == 100) {
    this->velMotorLF.erase(this->velMotorLF.begin());
    this->velMotorRF.erase(this->velMotorRF.begin());
    this->velMotorLB.erase(this->velMotorLB.begin());
    this->velMotorRB.erase(this->velMotorRB.begin());
  }

  auto prevLF = (float)robot->drivetrain->leftFront->get_actual_velocity();
  auto prevRF = (float)robot->drivetrain->rightFront->get_actual_velocity();
  auto prevLB = (float)robot->drivetrain->leftBack->get_actual_velocity();
  auto prevRB = (float)robot->drivetrain->rightBack->get_actual_velocity();
  if (prevLF == INFINITY || prevLF == -1)
    prevLF = 5;
  if (prevRF == INFINITY || prevRF == -1)
    prevRF = 5;
  if (prevLB == INFINITY || prevLB == -1)
    prevLB = 5;
  if (prevRB == INFINITY || prevRB == -1)
    prevRB = 5;
  this->velMotorLF.push_back(prevLF);
  this->velMotorRF.push_back(prevRF);
  this->velMotorLB.push_back(prevLB);
  this->velMotorRB.push_back(prevRB);
  float heightScale = this->canvasHeight / 800.0f;
  float widthScale = this->canvasWidth / 100.0f;

  float x = 0;
  for (int i = (int)this->velMotorLF.size() - 2; i >= 0; --i) {
    float v = this->velMotorLF[i];
    lv_canvas_draw_line(this->drivetrainCanvas,
                        lv_point_t{static_cast<lv_coord_t>(this->canvasWidth - (x * widthScale)),
                                   static_cast<lv_coord_t>(this->canvasHeight - std::fabs(prevLF * heightScale))},
                        lv_point_t{static_cast<lv_coord_t>(this->canvasWidth - ((x + 1) * widthScale)),
                                   static_cast<lv_coord_t>(this->canvasHeight - std::fabs(v * heightScale))},
                        v >= 0 ? screen::colour::RED : screen::colour::BLUE);
    prevLF = v;
    v = this->velMotorRF[i];
    lv_canvas_draw_line(this->drivetrainCanvas,
                        lv_point_t{static_cast<lv_coord_t>(this->canvasWidth - (x * widthScale)),
                                   static_cast<lv_coord_t>(this->canvasHeight - std::fabs(prevRF * heightScale))},
                        lv_point_t{static_cast<lv_coord_t>(this->canvasWidth - ((x + 1) * widthScale)),
                                   static_cast<lv_coord_t>(this->canvasHeight - std::fabs(v * heightScale))},
                        v >= 0 ? screen::colour::ORANGE : screen::colour::VIOLET);
    prevRF = v;
    v = this->velMotorLB[i];
    lv_canvas_draw_line(this->drivetrainCanvas,
                        lv_point_t{static_cast<lv_coord_t>(this->canvasWidth - (x * widthScale)),
                                   static_cast<lv_coord_t>(this->canvasHeight - std::fabs(prevLB * heightScale))},
                        lv_point_t{static_cast<lv_coord_t>(this->canvasWidth - ((x + 1) * widthScale)),
                                   static_cast<lv_coord_t>(this->canvasHeight - std::fabs(v * heightScale))},
                        v >= 0 ? screen::colour::YELLOW : screen::colour::GREEN);
    prevLB = v;
    v = this->velMotorRB[i];
    lv_canvas_draw_line(this->drivetrainCanvas,
                        lv_point_t{static_cast<lv_coord_t>(this->canvasWidth - (x * widthScale)),
                                   static_cast<lv_coord_t>(this->canvasHeight - std::fabs(prevRB * heightScale))},
                        lv_point_t{static_cast<lv_coord_t>(this->canvasWidth - ((x + 1) * widthScale)),
                                   static_cast<lv_coord_t>(this->canvasHeight - std::fabs(v * heightScale))},
                        v >= 0 ? screen::colour::PINK : screen::colour::LIGHT_BLUE);
    prevRB = v;
    ++x;
  }
  lv_obj_invalidate(this->drivetrainCanvas);
}

void DrivetrainChart::destroy(lv_obj_t *screen) {
  this->velMotorLF.clear();
  this->velMotorRF.clear();
  this->velMotorLB.clear();
  this->velMotorRB.clear();
}
} // namespace screen