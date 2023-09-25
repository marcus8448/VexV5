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

  lv_obj increaseBtn;
  lv_obj valueLabel;
  lv_obj decreaseBtn;

public:
  double *value;
  const double delta;

  explicit ControlGroup(lv_obj_t *screen, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, double delta,
                        double *value);

  void update();
};

class PidTuning : public Screen {
private:
  robot::device::PID &pid;

  ControlGroup Kp;
  ControlGroup Ki;
  ControlGroup Kd;

  lv_obj testBtn;
  lv_obj errorLabel;
  lv_obj changeLabel;
  lv_obj oscillationsLabel;
  lv_obj overshootLabel;
  lv_obj headingLabel;
  lv_obj leftPower;
  lv_obj rightPower;

public:
  std::string_view runName;
  double prevError = INFINITY;
  double overshoot = 0;
  uint16_t oscillations = 0;
  bool testing = false;
  void *taskHandle = nullptr;

  explicit PidTuning(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height,
                     robot::device::PID &pid, std::string_view runName);

  void update() override;
  void startTest();
};
} // namespace screen
#endif // SCREEN_PID_TUNING_HPP
