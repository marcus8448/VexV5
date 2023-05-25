#ifndef STRUCTURE_FIXED_QUEUE
#define STRUCTURE_FIXED_QUEUE
#include <cstddef>

namespace structure {
template<class Type, size_t SIZE>
class FixedQueue {
  Type values[SIZE] = {};
  size_t position = 0;

public:
  explicit FixedQueue(Type defaultValue);

  Type operator[](size_t index) const;

  [[nodiscard]] Type get(size_t index) const;
  Type pushPop(Type value);
  [[nodiscard]] constexpr size_t size() const;
};

template <class Type, size_t SIZE> FixedQueue<Type, SIZE>::FixedQueue(Type defaultValue) {
  for (auto &item : this->values) {
    item = defaultValue;
  }
}

template <class Type, size_t SIZE> Type FixedQueue<Type, SIZE>::operator[](size_t index) const {
  return this->get(index);
}

template <class Type, size_t SIZE> Type FixedQueue<Type, SIZE>::get(size_t index) const {
  return this->values[(this->position + index) % SIZE];
}

template <class Type, size_t SIZE> Type FixedQueue<Type, SIZE>::pushPop(Type value) {
  Type previous = this->values[this->position];
  this->values[this->position] = value;
  this->position++;
  if (this->position == SIZE) {
    this->position = 0;
  }
  return previous;
}

template <class Type, size_t SIZE> constexpr size_t FixedQueue<Type, SIZE>::size() const { return SIZE; }
}
#endif // STRUCTURE_FIXED_QUEUE