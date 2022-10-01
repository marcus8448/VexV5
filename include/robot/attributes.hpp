#ifndef ROBOT_ATTRIBUTES_HPP
#define ROBOT_ATTRIBUTES_HPP

namespace robot {
class Resettable {
public:
  virtual void stop() {};
  virtual void reset() {};
};
class Targeting {
public:
  virtual bool is_offset_within(double distance) = 0;
};
}

#endif //ROBOT_ATTRIBUTES_HPP
