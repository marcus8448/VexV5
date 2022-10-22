#ifndef ROBOT_INDEXER_HPP
#define ROBOT_INDEXER_HPP

#include "pros/motors.hpp"
#include "robot/controller/controller.hpp"

namespace robot {

/**
 * Represents the flywheel of the robot.
 */
class Indexer : public Updatable {
public:
  enum State { CHARGED, PUSHING, PUSHED, CHARGING };

private:
  /**
   * The motor of the flywheel.
   */
  pros::Motor *motor;
  State state = State::CHARGED;
  int32_t ticksInState = 0;

public:
  /**
   * Creates a new indexer with the specified motor.
   * @param motor The motor of the flywheel.
   */
  explicit Indexer(pros::Motor *motor);
  virtual ~Indexer();

  /**
   * Engages the indexer.
   * Does not do anything if the indexer is already pushing.
   */
  void push();

  void charge();

  void cycle();

  void awaitReady(int millis_timeout = 2500);

  void awaitPush(int millis_timeout = 2500);

  [[nodiscard]] pros::Motor *get_motor() const;

  void update(Controller *controller) override;

private:
  void set_state(State value);
};
} // namespace robot
#endif // ROBOT_INDEXER_HPP
