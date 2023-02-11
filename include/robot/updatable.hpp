#ifndef ROBOT_UPDATABLE_HPP
#define ROBOT_UPDATABLE_HPP
namespace robot {
class Updatable {
public:
  virtual ~Updatable() = default;

  virtual void update() = 0;
};
}
#endif //ROBOT_UPDATABLE_HPP
