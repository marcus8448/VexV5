#ifndef STRUCTURE_FIXED_QUEUE_HPP
#define STRUCTURE_FIXED_QUEUE_HPP
#include <array>
#include <cmath>
#include <limits>

namespace structure {
template <size_t SIZE> class FixedQueue {
  std::array<float, SIZE> values = {};
  size_t position = 0;

public:
  float max = -std::numeric_limits<float>::infinity();
  float min = std::numeric_limits<float>::infinity();

  FixedQueue();

  float operator[](size_t index) const;

  [[nodiscard]] float get(size_t index) const;
  void add(float value);
  [[nodiscard]] constexpr size_t size() const;

  void clear();
};

template <size_t SIZE> FixedQueue<SIZE>::FixedQueue() = default;

template <size_t SIZE> float FixedQueue<SIZE>::operator[](size_t index) const { return this->get(index); }

template <size_t SIZE> float FixedQueue<SIZE>::get(size_t index) const {
  return this->values[(this->position + index) % SIZE];
}

template <size_t SIZE> void FixedQueue<SIZE>::add(float value) {
  if (value >= this->max) {
    this->max = value;
  } else if (this->values[this->position] == this->max) {
    this->max = -std::numeric_limits<float>::infinity();
    for (size_t i = 0; i < SIZE; ++i) {
      if (i == this->position) {
        continue;
      }
      this->max = std::max(this->max, this->values[i]);
    }
  }

  if (value <= this->min) {
    this->min = value;
  } else if (this->values[this->position] == this->min) {
    this->min = std::numeric_limits<float>::infinity();
    for (size_t i = 0; i < SIZE; ++i) {
      if (i == this->position) {
        continue;
      }
      this->min = std::min(this->min, this->values[i]);
    }
  }

  this->values[this->position++] = value;
  if (this->position == SIZE) {
    this->position = 0;
  }
}

template <size_t SIZE> constexpr size_t FixedQueue<SIZE>::size() const { return SIZE; }

template <size_t SIZE> void FixedQueue<SIZE>::clear() {
  this->position = 0;
  memset(this->values, 0, SIZE * sizeof(float));
}
} // namespace structure
#endif // STRUCTURE_FIXED_QUEUE_HPP
