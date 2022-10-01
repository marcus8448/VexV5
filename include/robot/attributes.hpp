#ifndef ROBOT_ATTRIBUTES_HPP
#define ROBOT_ATTRIBUTES_HPP

namespace robot {
class Resettable {
public:
  virtual void stop() = 0;
  virtual void reset() = 0;
};
class Targeting {
public:
  virtual bool is_offset_within(double distance) = 0;
};
} // namespace robot

#endif // ROBOT_ATTRIBUTES_HPP
