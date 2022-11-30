#ifndef SCREEN_LOGGING_HPP
#define SCREEN_LOGGING_HPP

#include "screen/screen.hpp"
#include <cstring>
#include <string>

namespace screen {
class Logging : public Screen {
private:
  lv_obj_t *logs = nullptr;

public:
  explicit Logging();
  ~Logging();

  void create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) override;
  void update(robot::Robot &robot) override;

  void write_line(const char *string, lv_color_t colour);
  void write_line(const std::string &string, lv_color_t colour);
};

extern Logging *logging;
} // namespace screen
#endif // SCREEN_LOGGING_HPP
