#include <cstddef>
#include <cstdlib>
#include <vector>
#include <cmath>
#include "display/lv_objx/lv_btn.h"
#include "display/lv_objx/lv_canvas.h"
#include "logger.hpp"
#include "pros/rtos.hpp"
#include "screen.hpp"
#include "display/lvgl.h"

namespace screen {
static std::vector<lv_obj_t *> screens(4);
static uint16_t activeScreen = 0;
static char buffer[32];
static const lv_color_t LV_WHITE = lv_color_hex(WHITE);
static const lv_color_t LV_RED = lv_color_hex(RED);
static const lv_color_t LV_GREEN = lv_color_hex(GREEN);
static const lv_color_t LV_BLUE = lv_color_hex(BLUE);
static const lv_color_t LV_YELLOW = lv_color_hex(YELLOW);

static int16_t width = 0;
static int16_t height = 0;

static void *canvasBuffer;

// screen 0
static lv_obj_t *uptimeLabel = nullptr;
static lv_obj_t *motorLFLabel = nullptr;
static lv_obj_t *motorRFLabel = nullptr;
static lv_obj_t *motorLBLabel = nullptr;
static lv_obj_t *motorRBLabel = nullptr;
static lv_obj_t *flywheelLabel = nullptr;
static lv_obj_t *rollerLabel = nullptr;
static lv_obj_t *intakeLabel = nullptr;

// screen 1
static std::vector<float> velMotorLF;
static std::vector<float> velMotorRF;
static std::vector<float> velMotorLB;
static std::vector<float> velMotorRB;
static lv_obj_t *drivetrainCanvas = nullptr;

// screen 2
static std::vector<float> velFlywheel;
static lv_obj_t *flywheelCanvas = nullptr;

// screen 3
static lv_obj_t *logs = nullptr;

void create_info_screen(lv_obj_t *screen);
void create_drivetrain_screen(lv_obj_t *screen);
void create_log_screen(lv_obj_t *screen);
lv_obj_t* create_label(lv_obj_t *screen, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char *text = "<uninitialized>");
lv_res_t prev_page(lv_obj_t *btn);
lv_res_t next_page(lv_obj_t *btn);
void create_flywheel_screen(lv_obj_t *screen);
void update_motor(lv_obj_t *label, const char *name, int32_t voltage);
void update_motor_digital(lv_obj_t *label, pros::Motor *motor, bool engaged, const char* name);
lv_res_t drop_log(lv_obj_t *obj);
void drop_screen(uint16_t screen);
void init_screen(uint16_t screen);
void update(Robot *robot);
[[noreturn]] void update_task(void *params);
void create_prev_btn(lv_obj_t *obj);
void create_next_btn(lv_obj_t *obj);

void initialize(Robot *robot) {
  lv_init();
  lv_obj_t *base_view = lv_scr_act();
  width = lv_obj_get_width(base_view);
  height = lv_obj_get_height(base_view);
  canvasBuffer = calloc(((lv_img_color_format_get_px_size(LV_IMG_CF_TRUE_COLOR)) * width * (height - 40)), 1);
  activeScreen = 0;
  lv_obj_t *screen_0 = lv_obj_create(base_view, nullptr);
  lv_obj_t *screen_1 = lv_obj_create(base_view, nullptr);
  lv_obj_t *screen_2 = lv_obj_create(base_view, nullptr);
  lv_obj_t *screen_3 = lv_obj_create(base_view, nullptr);
  lv_obj_set_size(screen_0, width, height);
  lv_obj_set_size(screen_1, width, height);
  lv_obj_set_size(screen_2, width, height);
  lv_obj_set_size(screen_3, width, height);
  lv_obj_set_hidden(screen_0, true);
  lv_obj_set_hidden(screen_1, true);
  lv_obj_set_hidden(screen_2, true);
  lv_obj_set_hidden(screen_3, true);

  create_next_btn(screen_0);
  create_prev_btn(screen_1);
  create_next_btn(screen_1);
  create_prev_btn(screen_2);
  create_next_btn(screen_2);
  create_prev_btn(screen_3);

  screens[0] = screen_0;
  screens[1] = screen_1;
  screens[2] = screen_2;
  screens[3] = screen_3;


  create_info_screen(screen_0);
  create_drivetrain_screen(screen_1);
  create_flywheel_screen(screen_2);
  create_log_screen(screen_3);

  init_screen(activeScreen);

  pros::Task(update_task, robot, "Screen Update");
}

void create_prev_btn(lv_obj_t *obj) {
  auto prevBtn = lv_btn_create(obj, nullptr);
  lv_obj_set_pos(prevBtn, 0, (int16_t)(height - 40));
  lv_obj_set_size(prevBtn, 40, 40);
  lv_btn_set_action(prevBtn, LV_BTN_ACTION_CLICK, prev_page);
}

void create_next_btn(lv_obj_t *obj) {
  auto nextBtn = lv_btn_create(obj, nullptr);
  lv_obj_set_pos(nextBtn, (int16_t)(width - 40), (int16_t)(height - 40));
  lv_obj_set_size(nextBtn, 40, 40);
  lv_btn_set_action(nextBtn, LV_BTN_ACTION_CLICK, next_page);
}

[[noreturn]] void update_task(void* params) {
  auto *robot = static_cast<Robot *>(params);
  while (true) {
    update(robot);
    pros::delay(50);
  }
}

void update(Robot *robot) {
  snprintf(buffer, 32, "Uptime: %u", pros::millis());
  lv_label_set_text(uptimeLabel, buffer);

  if (activeScreen == 0) {
    update_motor(motorLFLabel, "DT-LF", robot->drivetrain->leftFront->get_voltage());
    update_motor(motorRFLabel, "DT-RF", robot->drivetrain->rightFront->get_voltage());
    update_motor(motorLBLabel, "DT-LB", robot->drivetrain->leftBack->get_voltage());
    update_motor(motorRBLabel, "DT-RB", robot->drivetrain->rightBack->get_voltage());
    update_motor_digital(flywheelLabel, robot->flywheel->motor, robot->flywheel->isEngaged(), "Flywheel");
    update_motor_digital(rollerLabel, nullptr, false, "Roller");
    update_motor_digital(intakeLabel, nullptr, false, "Intake");
  } else if (activeScreen == 1) {
    if (velMotorLF.size() == 100) {
      logger::debug("Dropping old motor data");
      velMotorLF.erase(velMotorLF.begin());
      velMotorRF.erase(velMotorRF.begin());
      velMotorLB.erase(velMotorLB.begin());
      velMotorRB.erase(velMotorRB.begin());
    }

    auto prevLF = (float)robot->drivetrain->leftFront->get_actual_velocity();
    auto prevRF = (float)robot->drivetrain->rightFront->get_actual_velocity();
    auto prevLB = (float)robot->drivetrain->leftBack->get_actual_velocity();
    auto prevRB = (float)robot->drivetrain->rightBack->get_actual_velocity();
    if (prevLF == INFINITY || prevLF == -1) prevLF = 5;
    if (prevRF == INFINITY || prevRF == -1) prevRF = 5;
    if (prevLB == INFINITY || prevLB == -1) prevLB = 5;
    if (prevRB == INFINITY || prevRB == -1) prevRB = 5;
    velMotorLF.push_back(prevLF);
    velMotorRF.push_back(prevRF);
    velMotorLB.push_back(prevLB);
    velMotorRB.push_back(prevRB);
    float heightScale = ((float)height - 40.0f) / 100.0f;
    float widthScale = ((float)width) / 100.0f;

    logger::debug("for, %i %i %f", velMotorLF.size(), height, heightScale);
    unsigned int x = 0;
    for (signed int i = (signed int)velMotorLF.size() - 2; i >= 0; --i) {
      float v = velMotorLF[i];

      lv_canvas_draw_line(drivetrainCanvas, lv_point_t {
          (int16_t) ((float)width - ((float)x * widthScale)),
          (int16_t) (prevLF * heightScale)
      }, lv_point_t{
          (int16_t) ((float) width - (float) (x + 1) * widthScale),
          (int16_t) (v * heightScale)
      }, LV_WHITE);
      prevLF = v;
      v = velMotorRF[i];
      lv_canvas_draw_line(drivetrainCanvas, lv_point_t {
          (int16_t) ((float)width - ((float)x * widthScale)),
          (int16_t) (prevRF * heightScale)
      }, lv_point_t{
          (int16_t) ((float) width - (float) (x + 1) * widthScale),
          (int16_t) (v * heightScale)
      }, LV_RED);
      prevRF = v;
      v = velMotorLB[i];
      lv_canvas_draw_line(drivetrainCanvas, lv_point_t {
          (int16_t) ((float)width - ((float)x * widthScale)),
          (int16_t) (prevLB * heightScale)
      }, lv_point_t{
          (int16_t) ((float) width - (float) (x + 1) * widthScale),
          (int16_t) (v * heightScale)
      }, LV_GREEN);
      prevLB = v;
      v = velMotorRB[i];
      lv_canvas_draw_line(drivetrainCanvas, lv_point_t {
          (int16_t) ((float)width - ((float)x * widthScale)),
          (int16_t) (prevRB * heightScale)
      }, lv_point_t{
          (int16_t) ((float) width - (float) (x + 1) * widthScale),
          (int16_t) (v * heightScale)
      }, LV_BLUE);
      prevRB = v;
      ++x;
    }
    logger::debug("endn for");
  } else if (activeScreen == 2) {
    if (velFlywheel.capacity() == 0) velFlywheel.erase(velFlywheel.begin());
    auto prev = (float)robot->flywheel->motor->get_actual_velocity();
    if (prev == INFINITY || prev == -1) prev = 5;
    velFlywheel.push_back(prev);
    float heightScale = ((float)height - 40.0f) / 600.0f;
    float widthScale = ((float)width) / 100.0f;

    unsigned int x = 0;
    for (signed int i = (signed int)velFlywheel.size() - 2; i >= 0; --i) {
      float v = velFlywheel[i];
      lv_canvas_draw_line(flywheelCanvas, lv_point_t {
        (int16_t) ((float)width - ((float)x * widthScale)),
        (int16_t) (prev * heightScale)
        }, lv_point_t{
        (int16_t) ((float) width - (float) (x + 1) * widthScale),
        (int16_t) (v * heightScale)
      }, LV_WHITE);
      ++x;
      prev = v;
    }
  } else if (activeScreen == 3) {

  }
}

void update_motor_digital(lv_obj_t *label, pros::Motor *motor, bool engaged, const char* name) {
  if (motor == nullptr || motor->get_voltage() == INT32_MAX) {
    snprintf(buffer, 32, "%s: Disconnected", name);
  } else {
    if (engaged) {
      snprintf(buffer, 32, "%s: Engaged", name);
    } else {
      snprintf(buffer, 32, "%s: Disengaged", name);
    }
  }
  lv_label_set_text(label, buffer);
}

void update_motor(lv_obj_t *label, const char *name, int32_t voltage) {
  if (voltage == INT32_MAX) {
    snprintf(buffer, 32, "%s: Disconnected", name);
  } else {
    snprintf(buffer, 32, "%s: %i", name, voltage);
  }
  lv_label_set_text(label, buffer);
}

void create_info_screen(lv_obj_t *screen) {
  create_label(screen, 0, 16 * 0, width / 2, 16, "Build: " __DATE__ " " __TIME__);
  uptimeLabel = create_label(screen, width / 2, 16 * 0, width / 2, 16);

//  create_label(screen, 0, 16 * 1, width / 2, 16, "");
//  create_label(screen, width / 2, 16 * 1, width / 2, 16, "");

  // Port Statuses
  motorLFLabel = create_label(screen, 0, 16 * 2, width / 2, 16);
  motorRFLabel = create_label(screen, width / 2, 16 * 2, width / 2, 16);
  motorLBLabel = create_label(screen, 0, 16 * 3, width / 2, 16);
  motorRBLabel = create_label(screen, width / 2, 16 * 3, width / 2, 16);
  flywheelLabel = create_label(screen, 0, 16 * 4, width / 2, 16);
  rollerLabel = create_label(screen, width / 2, 16 * 4, width / 2, 16);
  intakeLabel = create_label(screen, 0, 16 * 5, width / 2, 16);
//  create_label(screen, width / 2, 16 * 5, width / 2, 16, "");
}

void create_drivetrain_screen(lv_obj_t *screen) {
  drivetrainCanvas = lv_canvas_create(screen, nullptr);
  velMotorLF.reserve(100);
  velMotorRF.reserve(100);
  velMotorLB.reserve(100);
  velMotorRB.reserve(100);
  lv_obj_set_pos(drivetrainCanvas, 0, 0);
  lv_obj_set_size(drivetrainCanvas, width, height - 40);
  lv_canvas_set_buffer(drivetrainCanvas, canvasBuffer, width, height - 40, LV_IMG_CF_TRUE_COLOR);
}

void create_flywheel_screen(lv_obj_t *screen) {
  velFlywheel.reserve(100);
  flywheelCanvas = lv_canvas_create(screen, nullptr);
  lv_obj_set_pos(flywheelCanvas, 0, 0);
  lv_obj_set_size(flywheelCanvas, width, height - 40);
  lv_canvas_set_buffer(flywheelCanvas, canvasBuffer, width, height - 40, LV_IMG_CF_TRUE_COLOR);
}

void create_log_screen(lv_obj_t *screen) {
  logs = lv_list_create(screen, nullptr);
  lv_obj_set_pos(logs, 0, 0);
  lv_obj_set_size(logs, width, height - 40);
}

void write_line(const char *string, Colour colour) {
  if (logs != nullptr) {
    if (lv_list_get_size(logs) >= 12) {
      lv_list_remove(logs, 0);
    }

    lv_list_add(logs, nullptr, string, drop_log);
  }
}

void write_line(const std::string& string, Colour colour) {
  if (logs != nullptr) {
    write_line(string.c_str(), colour);
  }
}

lv_obj_t* create_label(lv_obj_t *screen, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char* text) {
  auto label = lv_label_create(screen, nullptr);
  lv_obj_set_pos(label, x, y);
  lv_obj_set_size(label, w, h);
  lv_label_set_text(label, text);
  return label;
}

lv_res_t prev_page(lv_obj_t *btn) {
  if (activeScreen != 0) {
    drop_screen(activeScreen);
    init_screen(--activeScreen);
  } else {
    lv_obj_invalidate(btn);
    return LV_RES_INV;
  }
  return LV_RES_OK;
}

lv_res_t next_page(lv_obj_t *btn) {
  if (activeScreen != screens.size() - 1) {
    drop_screen(activeScreen);
    init_screen(++activeScreen);
  } else {
    lv_obj_invalidate(btn);
    return LV_RES_INV;
  }
  return LV_RES_OK;
}

void init_screen(uint16_t screen) {
  logger::push_section("Init screen");
  lv_obj_set_hidden(screens[screen], false);
  logger::pop_section();
}

void drop_screen(uint16_t screen) {
  logger::push_section("Drop screen");
  lv_obj_set_hidden(screens[screen], true);
  if (screen == 1) {
    velMotorLF.clear();
    velMotorRF.clear();
    velMotorLB.clear();
    velMotorRB.clear();
  } else if (screen == 2) {
    velFlywheel.clear();
  }
  logger::pop_section();
}

lv_res_t drop_log(lv_obj_t *obj) {
  lv_obj_invalidate(obj);
  return LV_RES_INV;
}
}
