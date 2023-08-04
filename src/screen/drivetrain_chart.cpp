#include "screen/drivetrain_chart.hpp"
#include "screen/colour.hpp"
#include "screen/lvgl_util.hpp"
#include "screen/screen.hpp"
#include <cmath>

#define QUARTER_WIDTH static_cast<lv_coord_t>((SCREEN_WIDTH - 64) / 4)

namespace screen {
DrivetrainChart::DrivetrainChart(robot::Robot &robot) : robot(robot) {}

void DrivetrainChart::initialize(lv_obj_t *screen) {
  this->drivetrainCanvas = create_canvas(screen);
}

void DrivetrainChart::update() {
  auto prevLF = static_cast<float>(this->robot.drivetrain.leftFrontMotor.getVelocity());
  auto prevRF = static_cast<float>(this->robot.drivetrain.rightFrontMotor.getVelocity());
  auto prevLB = static_cast<float>(this->robot.drivetrain.leftBackMotor.getVelocity());
  auto prevRB = static_cast<float>(this->robot.drivetrain.rightBackMotor.getVelocity());
  if (prevLF == INFINITY || prevLF == -1)
    prevLF = 0;
  if (prevRF == INFINITY || prevRF == -1)
    prevRF = 0;
  if (prevLB == INFINITY || prevLB == -1)
    prevLB = 0;
  if (prevRB == INFINITY || prevRB == -1)
    prevRB = 0;
  this->velMotorLF.pushPop(prevLF);
  this->velMotorRF.pushPop(prevRF);
  this->velMotorLB.pushPop(prevLB);
  this->velMotorRB.pushPop(prevRB);
  float heightScale = CANVAS_HEIGHT / 800.0f;
  float widthScale = CANVAS_WIDTH / 100.0f;

  size_t size = this->velMotorLF.size();
  lv_point_t points[size];
  lv_draw_line_dsc_t lineDesc;
  lv_draw_line_dsc_init(&lineDesc);
  lv_draw_label_dsc_t textDesc;
  lv_draw_label_dsc_init(&textDesc);

  for (size_t i = 0; i < size; i++) {
    points[i] = lv_point_t{static_cast<lv_coord_t>(CANVAS_WIDTH - (i * widthScale)),
                             static_cast<lv_coord_t>(CANVAS_HEIGHT - std::fabs(this->velMotorLF.get(size - 1 - i) * heightScale))};
  }

  textDesc.color = lineDesc.color = screen::colour::BLUE;
  lv_canvas_draw_line(this->drivetrainCanvas, points, size, &lineDesc);
  lv_canvas_draw_text(this->drivetrainCanvas, 16 + 32, static_cast<lv_coord_t>(SCREEN_HEIGHT - 16 - 6), QUARTER_WIDTH, &textDesc, "Drivetrain LF");

  for (size_t i = 0; i < size; i++) {
    points[i].x = static_cast<lv_coord_t>(CANVAS_WIDTH - (i * widthScale));
    points[i].y = static_cast<lv_coord_t>(CANVAS_HEIGHT - std::fabs(this->velMotorRF.get(size - 1 - i) * heightScale));
  }

  textDesc.color = lineDesc.color = screen::colour::ORANGE;
  lv_canvas_draw_line(this->drivetrainCanvas, points, size, &lineDesc);
  lv_canvas_draw_text(this->drivetrainCanvas, static_cast<lv_coord_t>(16 + 32 + QUARTER_WIDTH), static_cast<lv_coord_t>(SCREEN_HEIGHT - 16 - 6), QUARTER_WIDTH, &textDesc, "Drivetrain RF");

  for (size_t i = 0; i < size; i++) {
    points[i].x = static_cast<lv_coord_t>(CANVAS_WIDTH - (i * widthScale));
    points[i].y = static_cast<lv_coord_t>(CANVAS_HEIGHT - std::fabs(this->velMotorLB.get(size - 1 - i) * heightScale));
  }

  textDesc.color = lineDesc.color = screen::colour::GREEN;
  lv_canvas_draw_line(this->drivetrainCanvas, points, size, &lineDesc);
  lv_canvas_draw_text(this->drivetrainCanvas, static_cast<lv_coord_t>(16 + 32 + QUARTER_WIDTH * 2), static_cast<lv_coord_t>(SCREEN_HEIGHT - 16 - 6), QUARTER_WIDTH, &textDesc, "Drivetrain LB");

  for (size_t i = 0; i < size; i++) {
    points[i].x = static_cast<lv_coord_t>(CANVAS_WIDTH - (i * widthScale));
    points[i].y = static_cast<lv_coord_t>(CANVAS_HEIGHT - std::fabs(this->velMotorRB.get(size - 1 - i) * heightScale));
  }

  textDesc.color = lineDesc.color = screen::colour::PINK;
  lv_canvas_draw_line(this->drivetrainCanvas, points, size, &lineDesc);
  lv_canvas_draw_text(this->drivetrainCanvas, static_cast<lv_coord_t>(16 + 32 + QUARTER_WIDTH * 3), static_cast<lv_coord_t>(SCREEN_HEIGHT - 16 - 6), QUARTER_WIDTH, &textDesc, "Drivetrain RB");

  lv_obj_invalidate(this->drivetrainCanvas);
}

void DrivetrainChart::cleanup() {}
} // namespace screen
