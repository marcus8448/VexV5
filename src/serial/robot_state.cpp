#include "serial/robot_state.hpp"
#include "serial/serial_connection.hpp"
#include <cstring>

namespace serial {
typedef double floating;

RobotStatePlugin::RobotStatePlugin(uint8_t id, robot::Robot &robot) : PacketHandler(id), robot(robot) {}

void RobotStatePlugin::initialize() {}

void serialize_motor(uint8_t *buffer, const pros::Motor &motor) {
  floating src;
  src = motor.get_target_position();
  std::memcpy(&buffer[sizeof(floating) * 0], &src, sizeof(floating));
  src = motor.get_actual_velocity();
  std::memcpy(&buffer[sizeof(floating) * 1], &src, sizeof(floating));
  src = motor.get_position();
  std::memcpy(&buffer[sizeof(floating) * 2], &src, sizeof(floating));
  src = motor.get_efficiency();
  std::memcpy(&buffer[sizeof(floating) * 3], &src, sizeof(floating));
  src = motor.get_power();
  std::memcpy(&buffer[sizeof(floating) * 4], &src, sizeof(floating));
  src = motor.get_temperature();
  std::memcpy(&buffer[sizeof(floating) * 5], &src, sizeof(floating));
  src = motor.get_torque();
  std::memcpy(&buffer[sizeof(floating) * 6], &src, sizeof(floating));
  uint32_t i_src;
  i_src = motor.get_target_velocity();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 0], &i_src, sizeof(int));
  i_src = motor.get_current_draw();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 1], &i_src, sizeof(int));
  i_src = motor.get_voltage();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 2], &i_src, sizeof(int));
  i_src = motor.get_current_limit();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 3], &i_src, sizeof(int));
  i_src = motor.get_voltage_limit();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 4], &i_src, sizeof(int));
}

void RobotStatePlugin::handle(SerialConnection *connection, void *buffer, size_t len) {
  const uint32_t CONTROLLER_SIZE = 3 + (sizeof(floating) * 4);
  const uint32_t MOTOR_SIZE = (sizeof(floating) * 7) + (sizeof(int) * 5);
  const uint32_t SIZE = CONTROLLER_SIZE + (MOTOR_SIZE * 4);
  uint8_t buf[SIZE];

  if (this->robot.controller->a_pressed())
    buf[0] |= 0b00000001;
  if (this->robot.controller->b_pressed())
    buf[0] |= 0b00000010;
  if (this->robot.controller->x_pressed())
    buf[0] |= 0b00000100;
  if (this->robot.controller->y_pressed())
    buf[0] |= 0b00001000;
  if (this->robot.controller->up_pressed())
    buf[0] |= 0b00010000;
  if (this->robot.controller->down_pressed())
    buf[0] |= 0b00100000;
  if (this->robot.controller->left_pressed())
    buf[0] |= 0b01000000;
  if (this->robot.controller->right_pressed())
    buf[0] |= 0b10000000;
  if (this->robot.controller->l1_pressed())
    buf[1] |= 0b00000001;
  if (this->robot.controller->l2_pressed())
    buf[1] |= 0b00000010;
  if (this->robot.controller->r1_pressed())
    buf[1] |= 0b00000100;
  if (this->robot.controller->r2_pressed())
    buf[1] |= 0b00001000;
  // buf[2] = this->robot.controller->flywheel_speed(); todo
  floating src;
  src = static_cast<floating>(this->robot.controller->left_stick_x());
  std::memcpy(&buf[3 + sizeof(float) * 0], &src, sizeof(float));
  src = static_cast<floating>(this->robot.controller->left_stick_y());
  std::memcpy(&buf[3 + sizeof(float) * 1], &src, sizeof(float));
  src = static_cast<floating>(this->robot.controller->right_stick_x());
  std::memcpy(&buf[3 + sizeof(float) * 2], &src, sizeof(float));
  src = static_cast<floating>(this->robot.controller->right_stick_y());
  std::memcpy(&buf[3 + sizeof(float) * 3], &src, sizeof(float));

  serialize_motor(&buf[CONTROLLER_SIZE + MOTOR_SIZE * 0], this->robot.drivetrain->rightFront.get_raw_motor());
  serialize_motor(&buf[CONTROLLER_SIZE + MOTOR_SIZE * 1], this->robot.drivetrain->leftFront.get_raw_motor());
  serialize_motor(&buf[CONTROLLER_SIZE + MOTOR_SIZE * 2], this->robot.drivetrain->rightBack.get_raw_motor());
  serialize_motor(&buf[CONTROLLER_SIZE + MOTOR_SIZE * 3], this->robot.drivetrain->leftBack.get_raw_motor());
  connection->send_packet(this->id, &buf, SIZE);
}
} // namespace serial
