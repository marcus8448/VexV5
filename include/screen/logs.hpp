#ifndef SCREEN_LOGS_HPP
#define SCREEN_LOGS_HPP

#include "screen/screen.hpp"
#include <string>

namespace screen {
class Logging : public Screen {
private:
  lv_obj_t *logs = nullptr;

public:
  explicit Logging();

  void create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) override;
  void initialize(lv_coord_t width, lv_coord_t height) override;
  void update(robot::Robot *robot) override;
  void destroy(lv_obj_t *screen) override;

  void write_line(const char *string, lv_color_t colour);
  void write_line(const std::string &string, lv_color_t colour);
};

static Logging *logging = nullptr;
} // namespace screen
#endif // SCREEN_LOGS_HPP
