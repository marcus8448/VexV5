#ifndef CONTROL_INPUT_AUTONOMOUS_OUTLINE_HPP
#define CONTROL_INPUT_AUTONOMOUS_OUTLINE_HPP

#include "controller.hpp"
#include "filesystem.hpp"
#include "pros/misc.h"
#include "robot/robot.hpp"
#include <cstdint>
#include <fstream>

namespace control::input {
class AutonomousOutlineController : public Controller {
private:
  enum DrivetrainTarget { FORWARDS, BACKWARDS, LEFT, RIGHT };

  pros::controller_id_e_t controller_id;
  robot::Robot &robot;

  std::unique_ptr<std::ofstream> output;

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
  explicit AutonomousOutlineController(robot::Robot &robot,
                                       pros::controller_id_e_t controller_id = pros::E_CONTROLLER_MASTER);

  [[nodiscard]] uint16_t aPressed() const override;
  [[nodiscard]] uint16_t bPressed() const override;
  [[nodiscard]] uint16_t xPressed() const override;
  [[nodiscard]] uint16_t yPressed() const override;

  [[nodiscard]] uint16_t upPressed() const override;
  [[nodiscard]] uint16_t downPressed() const override;
  [[nodiscard]] uint16_t leftPressed() const override;
  [[nodiscard]] uint16_t rightPressed() const override;

  [[nodiscard]] uint16_t l1Pressed() const override;
  [[nodiscard]] uint16_t l2Pressed() const override;
  [[nodiscard]] uint16_t r1Pressed() const override;
  [[nodiscard]] uint16_t r2Pressed() const override;

  [[nodiscard]] double leftStickX() const override;
  [[nodiscard]] double leftStickY() const override;
  [[nodiscard]] double rightStickX() const override;
  [[nodiscard]] double rightStickY() const override;

  [[nodiscard]] int16_t speedSetting() const override;
  void setSpeedSetting(int16_t speed) override;

  void setLine(uint8_t line, uint8_t col, const char *str) override;
  void clearLine(uint8_t line) override;

  void rumble(const char *str) override;

  void update() override;

private:
  bool write_drivetrain_update();
};
} // namespace control::input
#endif // CONTROL_INPUT_AUTONOMOUS_OUTLINE_HPP
