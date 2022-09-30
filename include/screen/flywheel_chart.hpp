#ifndef SCREEN_FLYWHEEL_CHART_HPP
#define SCREEN_FLYWHEEL_CHART_HPP

namespace screen {
class FlywheelChart : public Screen {
private:
  float canvasWidth = 0.0f;
  float canvasHeight = 0.0f;

  std::vector<float> velFlywheel;
  lv_obj_t *flywheelCanvas = nullptr;
public:
  explicit FlywheelChart();

  void create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) override;
  void initialize(lv_coord_t width, lv_coord_t height) override;
  void update(Robot *robot) override;
  void destroy(lv_obj_t *screen) override;
};
} // screen

#endif //SCREEN_FLYWHEEL_CHART_HPP
