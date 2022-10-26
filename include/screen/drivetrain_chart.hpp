#ifndef SCREEN_DRIVETRAIN_CHART_HPP
#define SCREEN_DRIVETRAIN_CHART_HPP

#include "screen/screen.hpp"

namespace screen {
class DrivetrainChart : public Screen {
private:
  float canvasWidth = 0.0f;
  float canvasHeight = 0.0f;

  std::vector<float> velMotorLF;
  std::vector<float> velMotorRF;
  std::vector<float> velMotorLB;
  std::vector<float> velMotorRB;
  lv_obj_t *drivetrainCanvas = nullptr;

public:
  explicit DrivetrainChart();

  void create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) override;
  void update(robot::Robot *robot) override;
};
} // namespace screen

#endif // SCREEN_DRIVETRAIN_CHART_HPP
