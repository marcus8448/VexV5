#ifndef ATTRIBUTES_HPP
#define ATTRIBUTES_HPP

#include "controller.hpp"

class Resettable {
public:
  virtual void stop() {};
  virtual void reset() {};
};

class Targeting {
public:
  virtual bool is_offset_within(double distance) = 0;
};

class Updatable {
public:
  virtual void update(Controller *controller) {};
};

#endif //ATTRIBUTES_HPP
