#ifndef ROBOT_DRIVETRAIN_HPP
#define ROBOT_DRIVETRAIN_HPP

#include <cstdint>
#include "pros/motors.hpp"
#include "attributes.hpp"
#include "robot/controller/controller.hpp"

namespace robot {
class Drivetrain : public Resettable, public Targeting, public Updatable {
public:
  pros::Motor *rightFront;
  pros::Motor *leftFront;
  pros::Motor *rightBack;
  pros::Motor *leftBack;

public:
  Drivetrain(pros::Motor *rightFront, pros::Motor *leftFront, pros::Motor *rightBack, pros::Motor *leftBack);
  virtual ~Drivetrain();

  bool is_offset_within(double distance) override;
  void move_for(double right_distance, double left_distance, int32_t max_rpm = 60, bool block = true);
  void forwards(double distance, int32_t max_rpm = 100, bool block = true);
  void backwards(double distance, int32_t max_rpm = 100, bool block = true);
  void turn_right(double angle, int32_t max_rpm = 100, bool block = true);
  void turn_left(double angle, int32_t max_rpm = 100, bool block = true);
  void move_right(int32_t voltage);
  void move_left(int32_t voltage);

  void update(Controller *controller) override;
  void reset() override;
  void stop() override;
};
}

#endif //ROBOT_DRIVETRAIN_HPP
