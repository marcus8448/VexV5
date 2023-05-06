#ifndef CONTROL_AUTONOMOUS_NONE_HPP
#define CONTROL_AUTONOMOUS_NONE_HPP
#include "autonomous.hpp"

namespace control::autonomous {

/**
 * Autonomous run that does noting.
 */
class None : public Autonomous {
public:
  explicit None() : Autonomous("zNone") {}

  void run(Robot &robot) override;
};
} // namespace control::autonomous
#endif // CONTROL_AUTONOMOUS_NONE_HPP
