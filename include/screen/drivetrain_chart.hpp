#ifndef SCREEN_DRIVETRAIN_CHART_HPP
#define SCREEN_DRIVETRAIN_CHART_HPP

#include "robot/robot.hpp"
#include "screen.hpp"
#include "structure/fixed_queue.hpp"

namespace screen {
class DrivetrainChart : public Screen {
private:
  robot::Robot &robot;

  structure::FixedQueue<float, 100> velMotorLF = structure::FixedQueue<float, 100>(0.0f);
  structure::FixedQueue<float, 100> velMotorRF = structure::FixedQueue<float, 100>(0.0f);
  structure::FixedQueue<float, 100> velMotorLB = structure::FixedQueue<float, 100>(0.0f);
  structure::FixedQueue<float, 100> velMotorRB = structure::FixedQueue<float, 100>(0.0f);
  lv_obj_t *drivetrainCanvas = nullptr;

public:
  explicit DrivetrainChart(robot::Robot &robot);

  void initialize(lv_obj_t *screen) override;
  void update() override;
  void cleanup() override;
};
} // namespace screen

#endif // SCREEN_DRIVETRAIN_CHART_HPP
