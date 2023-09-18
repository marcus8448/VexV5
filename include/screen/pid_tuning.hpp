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
  ControlGroup(const Screen &) = delete;
  ControlGroup &operator=(const Screen &) = delete;
  ~ControlGroup();

  void update();
};

class PidTuning : public Screen {
private:
  robot::device::PID &pid;

  ControlGroup Kp;
  ControlGroup Ki;
  ControlGroup Kd;

  lv_obj_t *testBtn;
  lv_obj_t *errorLabel;
  lv_obj_t *changeLabel;
  lv_obj_t *oscillationsLabel;
  lv_obj_t *overshootLabel;

public:
  const std::string runName;
  double prevError = INFINITY;
  double overshoot = 0;
  uint16_t oscillations = 0;
  bool testing = false;
  void *taskHandle = nullptr;

  explicit PidTuning(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height,
                     robot::device::PID &pid, std::string runName);
  ~PidTuning() override;

  void update() override;
  void startTest();
};
} // namespace screen
#endif // SCREEN_PID_TUNING_HPP
