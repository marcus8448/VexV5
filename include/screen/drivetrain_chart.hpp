#ifndef SCREEN_DRIVETRAIN_CHART_HPP
#define SCREEN_DRIVETRAIN_CHART_HPP

#include "robot/robot.hpp"
#include "screen.hpp"
#include <vector>

namespace screen {
class DrivetrainChart : public Screen {
private:
  robot::Robot &robot;

  float canvasWidth = 0.0f;
  float canvasHeight = 0.0f;

  std::vector<float> velMotorLF;
  std::vector<float> velMotorRF;
  std::vector<float> velMotorLB;
  std::vector<float> velMotorRB;
  lv_obj_t *drivetrainCanvas = nullptr;

public:
  explicit DrivetrainChart(robot::Robot &robot);

  void initialize(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) override;
  void update() override;
};
} // namespace screen

#endif // SCREEN_DRIVETRAIN_CHART_HPP
