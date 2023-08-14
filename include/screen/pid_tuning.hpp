#ifndef SCREEN_PID_TUNING_HPP
#define SCREEN_PID_TUNING_HPP

#include "robot/robot.hpp"
#include "screen.hpp"

namespace screen {
class ControlGroup {
  const lv_coord_t x;
  const lv_coord_t y;
  const lv_coord_t width;
  const lv_coord_t height;

  lv_obj_t *increaseBtn;
  lv_obj_t *valueLabel;
  lv_obj_t *decreaseBtn;

public:
  double *value;
  const double delta;

  explicit ControlGroup(lv_obj_t *screen, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, double delta,
                        double *value);
  ~ControlGroup();

  void update();
};

class PidTuning : public Screen {
private:
  robot::device::PID &pid;

  ControlGroup *Kp = nullptr;
  ControlGroup *Ki = nullptr;
  ControlGroup *Kd = nullptr;

  lv_obj_t *testBtn = nullptr;
  lv_obj_t *errorLabel = nullptr;
  lv_obj_t *changeLabel = nullptr;
  lv_obj_t *oscillationsLabel = nullptr;
  lv_obj_t *overshootLabel = nullptr;

  bool prevTesting = false;

public:
  const std::string runName;
  robot::Robot &robot;
  double prevError = INFINITY;
  double overshoot = 0;
  uint16_t oscillations = 0;
  bool testing = false;

  explicit PidTuning(robot::Robot &robot, robot::device::PID &pid, std::string runName);

  void initialize(lv_obj_t *screen) override;
  void update() override;
  void cleanup() override;
};
} // namespace screen
#endif // SCREEN_PID_TUNING_HPP
