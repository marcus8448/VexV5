#include "serial/robot_state.hpp"
#include "serial/serial_connection.hpp"
#include <cstring>

namespace serial {
typedef double floating;

RobotStatePlugin::RobotStatePlugin(uint8_t id, robot::Robot &robot) : PacketHandler(id), robot(robot) {}

void RobotStatePlugin::initialize() {}

void serialize_motor(uint8_t *buffer, const robot::device::Motor &motor) {
  floating src;
  src = motor.getTargetPosition();
  std::memcpy(&buffer[sizeof(floating) * 0], &src, sizeof(floating));
  src = motor.getVelocity();
  std::memcpy(&buffer[sizeof(floating) * 1], &src, sizeof(floating));
  src = motor.getPosition();
  std::memcpy(&buffer[sizeof(floating) * 2], &src, sizeof(floating));
  src = motor.getEfficiency();
  std::memcpy(&buffer[sizeof(floating) * 3], &src, sizeof(floating));
  src = motor.getPower();
  std::memcpy(&buffer[sizeof(floating) * 4], &src, sizeof(floating));
  src = motor.getTemperature();
  std::memcpy(&buffer[sizeof(floating) * 5], &src, sizeof(floating));
  src = motor.getTorque();
  std::memcpy(&buffer[sizeof(floating) * 6], &src, sizeof(floating));
  uint32_t i_src;
  i_src = motor.getTargetVelocity();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 0], &i_src, sizeof(int));
  i_src = motor.getCurrentDraw();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 1], &i_src, sizeof(int));
  i_src = motor.getVoltage();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 2], &i_src, sizeof(int));
  i_src = motor.getCurrentLimit();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 3], &i_src, sizeof(int));
  i_src = motor.getVoltageLimit();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 4], &i_src, sizeof(int));
}

void RobotStatePlugin::handle(SerialConnection *connection, void *buffer, size_t len) {
  const uint32_t CONTROLLER_SIZE = 3 + (sizeof(floating) * 4);
  const uint32_t MOTOR_SIZE = (sizeof(floating) * 7) + (sizeof(int) * 5);
  const uint32_t SIZE = CONTROLLER_SIZE + (MOTOR_SIZE * 4);
  uint8_t buf[SIZE];

  buf[0] = 0;
  buf[1] = 0;

  if (this->robot.controller->aPressed())
    buf[0] |= 0b00000001;
  if (this->robot.controller->bPressed())
    buf[0] |= 0b00000010;
  if (this->robot.controller->xPressed())
    buf[0] |= 0b00000100;
  if (this->robot.controller->yPressed())
    buf[0] |= 0b00001000;
  if (this->robot.controller->upPressed())
    buf[0] |= 0b00010000;
  if (this->robot.controller->downPressed())
    buf[0] |= 0b00100000;
  if (this->robot.controller->leftPressed())
    buf[0] |= 0b01000000;
  if (this->robot.controller->rightPressed())
    buf[0] |= 0b10000000;
  if (this->robot.controller->l1Pressed())
    buf[1] |= 0b00000001;
  if (this->robot.controller->l2Pressed())
    buf[1] |= 0b00000010;
  if (this->robot.controller->r1Pressed())
    buf[1] |= 0b00000100;
  if (this->robot.controller->r2Pressed())
    buf[1] |= 0b00001000;
  // buf[2] = this->robot.controller->flywheel_speed(); todo
  floating src;
  src = static_cast<floating>(this->robot.controller->leftStickX());
  std::memcpy(&buf[3 + sizeof(float) * 0], &src, sizeof(float));
  src = static_cast<floating>(this->robot.controller->leftStickY());
  std::memcpy(&buf[3 + sizeof(float) * 1], &src, sizeof(float));
  src = static_cast<floating>(this->robot.controller->rightStickX());
  std::memcpy(&buf[3 + sizeof(float) * 2], &src, sizeof(float));
  src = static_cast<floating>(this->robot.controller->rightStickY());
  std::memcpy(&buf[3 + sizeof(float) * 3], &src, sizeof(float));

  serialize_motor(&buf[CONTROLLER_SIZE + MOTOR_SIZE * 0], this->robot.drivetrain.rightFront);
  serialize_motor(&buf[CONTROLLER_SIZE + MOTOR_SIZE * 1], this->robot.drivetrain.leftFront);
  serialize_motor(&buf[CONTROLLER_SIZE + MOTOR_SIZE * 2], this->robot.drivetrain.rightBack);
  serialize_motor(&buf[CONTROLLER_SIZE + MOTOR_SIZE * 3], this->robot.drivetrain.leftBack);
  connection->send_packet(this->id, &buf, SIZE);
}
} // namespace serial
