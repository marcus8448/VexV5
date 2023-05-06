#ifndef CONTROL_INPUT_AUTONOMOUS_RECORDING_HPP
#define CONTROL_INPUT_AUTONOMOUS_RECORDING_HPP

#include "controller.hpp"
#include "fs/filesystem.hpp"
#include "pros/misc.h"
#include "robot/robot.hpp"
#include <cstdint>
#include <fstream>

namespace control::input {
class AutonomousRecordingController : public Controller {
private:
  enum DrivetrainTarget { FORWARDS, BACKWARDS, LEFT, RIGHT };

  pros::controller_id_e_t controller_id;
  robot::Robot &robot;

  std::ofstream output;

  uint16_t a = 0;
  uint16_t b = 0;
  uint16_t x = 0;
  uint16_t y = 0;

  uint16_t up = 0;
  uint16_t down = 0;
  uint16_t left = 0;
  uint16_t right = 0;

  uint16_t l1 = 0;
  uint16_t l2 = 0;
  uint16_t r1 = 0;
  uint16_t r2 = 0;

  DrivetrainTarget drivetrainTarget = FORWARDS;
  bool targetActive = false;
  bool targetDirty = false;

  int16_t flywheelRuntime = 0;
  int16_t flywheelSpeed = 7100;
  uint32_t ticks = 0;

  const char *enqueued_rumble = nullptr;

public:
  explicit AutonomousRecordingController(robot::Robot &robot,
                                         pros::controller_id_e_t controller_id = pros::E_CONTROLLER_MASTER);

  [[nodiscard]] uint16_t a_pressed() const override;
  [[nodiscard]] uint16_t b_pressed() const override;
  [[nodiscard]] uint16_t x_pressed() const override;
  [[nodiscard]] uint16_t y_pressed() const override;

  [[nodiscard]] uint16_t up_pressed() const override;
  [[nodiscard]] uint16_t down_pressed() const override;
  [[nodiscard]] uint16_t left_pressed() const override;
  [[nodiscard]] uint16_t right_pressed() const override;

  [[nodiscard]] uint16_t l1_pressed() const override;
  [[nodiscard]] uint16_t l2_pressed() const override;
  [[nodiscard]] uint16_t r1_pressed() const override;
  [[nodiscard]] uint16_t r2_pressed() const override;

  [[nodiscard]] double left_stick_x() const override;
  [[nodiscard]] double left_stick_y() const override;
  [[nodiscard]] double right_stick_x() const override;
  [[nodiscard]] double right_stick_y() const override;

  [[nodiscard]] int16_t flywheel_speed() const override;
  void flywheel_speed(int16_t speed) override;

  void set_line(uint8_t line, uint8_t col, const char *str) override;
  void clear_line(uint8_t line) override;

  void rumble(const char *str) override;

  void update() override;

private:
  bool write_drivetrain_update();
};
} // namespace control::input
#endif // CONTROL_INPUT_AUTONOMOUS_RECORDING_HPP
