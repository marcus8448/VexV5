#include "screen/drivetrain_chart.hpp"
#include "screen/colour.hpp"
#include "screen/lvgl_util.hpp"
#include "screen/screen.hpp"
#include <cmath>

namespace screen {
DrivetrainChart::DrivetrainChart(robot::Robot &robot) : robot(robot) {}

void DrivetrainChart::initialize(lv_obj_t *screen) {
  this->drivetrainCanvas = create_canvas(screen);

  auto qtrWidth = static_cast<lv_coord_t>((SCREEN_WIDTH - 64) / 4);
  create_label(screen, 16 + 32, static_cast<lv_coord_t>(SCREEN_HEIGHT - 32 - 6), qtrWidth, 16, "LF (+)",
               create_text_color_style(screen::colour::RED));
  create_label(screen, 16 + 32, static_cast<lv_coord_t>(SCREEN_HEIGHT - 16 - 6), qtrWidth, 16, "LF (-)",
               create_text_color_style(screen::colour::BLUE));

  create_label(screen, static_cast<lv_coord_t>(16 + 32 + qtrWidth), static_cast<lv_coord_t>(SCREEN_HEIGHT - 32 - 6),
               qtrWidth, 16, "RF (+)", create_text_color_style(screen::colour::ORANGE));
  create_label(screen, static_cast<lv_coord_t>(16 + 32 + qtrWidth), static_cast<lv_coord_t>(SCREEN_HEIGHT - 16 - 6),
               qtrWidth, 16, "RF (-)", create_text_color_style(screen::colour::VIOLET));

  create_label(screen, static_cast<lv_coord_t>(16 + 32 + qtrWidth * 2), static_cast<lv_coord_t>(SCREEN_HEIGHT - 32 - 6),
               qtrWidth, 16, "LB (+)", create_text_color_style(screen::colour::YELLOW));
  create_label(screen, static_cast<lv_coord_t>(16 + 32 + qtrWidth * 2), static_cast<lv_coord_t>(SCREEN_HEIGHT - 16 - 6),
               qtrWidth, 16, "LB (-)", create_text_color_style(screen::colour::GREEN));

  create_label(screen, static_cast<lv_coord_t>(16 + 32 + qtrWidth * 3), static_cast<lv_coord_t>(SCREEN_HEIGHT - 32 - 6),
               qtrWidth, 16, "RB (+)", create_text_color_style(screen::colour::PINK));
  create_label(screen, static_cast<lv_coord_t>(16 + 32 + qtrWidth * 3), static_cast<lv_coord_t>(SCREEN_HEIGHT - 16 - 6),
               qtrWidth, 16, "RB (-)", create_text_color_style(screen::colour::LIGHT_BLUE));
}

void DrivetrainChart::update() {
  auto prevLF = static_cast<float>(this->robot.drivetrain.leftFront.getVelocity());
  auto prevRF = static_cast<float>(this->robot.drivetrain.rightFront.getVelocity());
  auto prevLB = static_cast<float>(this->robot.drivetrain.leftBack.getVelocity());
  auto prevRB = static_cast<float>(this->robot.drivetrain.rightBack.getVelocity());
  if (prevLF == INFINITY || prevLF == -1)
    prevLF = 5;
  if (prevRF == INFINITY || prevRF == -1)
    prevRF = 5;
  if (prevLB == INFINITY || prevLB == -1)
    prevLB = 5;
  if (prevRB == INFINITY || prevRB == -1)
    prevRB = 5;
  this->velMotorLF.pushPop(prevLF);
  this->velMotorRF.pushPop(prevRF);
  this->velMotorLB.pushPop(prevLB);
  this->velMotorRB.pushPop(prevRB);
  float heightScale = CANVAS_HEIGHT / 800.0f;
  float widthScale = CANVAS_WIDTH / 100.0f;

  float x = 0;
  for (auto i = static_cast<int32_t>(this->velMotorLF.size()) - 1; i >= 0; --i) {
    float v = this->velMotorLF[i];
    lv_canvas_draw_line(this->drivetrainCanvas,
                        lv_point_t{static_cast<lv_coord_t>(CANVAS_WIDTH - (x * widthScale)),
                                   static_cast<lv_coord_t>(CANVAS_HEIGHT - std::fabs(prevLF * heightScale))},
                        lv_point_t{static_cast<lv_coord_t>(CANVAS_WIDTH - ((x + 1) * widthScale)),
                                   static_cast<lv_coord_t>(CANVAS_HEIGHT - std::fabs(v * heightScale))},
                        v >= 0 ? screen::colour::RED : screen::colour::BLUE);
    prevLF = v;
    v = this->velMotorRF[i];
    lv_canvas_draw_line(this->drivetrainCanvas,
                        lv_point_t{static_cast<lv_coord_t>(CANVAS_WIDTH - (x * widthScale)),
                                   static_cast<lv_coord_t>(CANVAS_HEIGHT - std::fabs(prevRF * heightScale))},
                        lv_point_t{static_cast<lv_coord_t>(CANVAS_WIDTH - ((x + 1) * widthScale)),
                                   static_cast<lv_coord_t>(CANVAS_HEIGHT - std::fabs(v * heightScale))},
                        v >= 0 ? screen::colour::ORANGE : screen::colour::VIOLET);
    prevRF = v;
    v = this->velMotorLB[i];
    lv_canvas_draw_line(this->drivetrainCanvas,
                        lv_point_t{static_cast<lv_coord_t>(CANVAS_WIDTH - (x * widthScale)),
                                   static_cast<lv_coord_t>(CANVAS_HEIGHT - std::fabs(prevLB * heightScale))},
                        lv_point_t{static_cast<lv_coord_t>(CANVAS_WIDTH - ((x + 1) * widthScale)),
                                   static_cast<lv_coord_t>(CANVAS_HEIGHT - std::fabs(v * heightScale))},
                        v >= 0 ? screen::colour::YELLOW : screen::colour::GREEN);
    prevLB = v;
    v = this->velMotorRB[i];
    lv_canvas_draw_line(this->drivetrainCanvas,
                        lv_point_t{static_cast<lv_coord_t>(CANVAS_WIDTH - (x * widthScale)),
                                   static_cast<lv_coord_t>(CANVAS_HEIGHT - std::fabs(prevRB * heightScale))},
                        lv_point_t{static_cast<lv_coord_t>(CANVAS_WIDTH - ((x + 1) * widthScale)),
                                   static_cast<lv_coord_t>(CANVAS_HEIGHT - std::fabs(v * heightScale))},
                        v >= 0 ? screen::colour::PINK : screen::colour::LIGHT_BLUE);
    prevRB = v;
    ++x;
  }
  lv_obj_invalidate(this->drivetrainCanvas);
}
} // namespace screen
