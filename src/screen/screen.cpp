#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#include "display/lv_objx/lv_btn.h"
#include "display/lv_objx/lv_canvas.h"
#include "display/lv_core/lv_obj.h"
#include "display/lv_core/lv_style.h"
#include "display/lv_objx/lv_label.h"
#pragma clang diagnostic pop

#include "logger.hpp"
#include "pros/rtos.hpp"
#include "screen/screen.hpp"
#include "display/lvgl.h"

namespace screen {

#define CANVAS_COLOUR LV_IMG_CF_TRUE_COLOR

static lv_obj_t *screens[4];
static uint16_t activeScreen = -1;
static char buffer[32];

static const lv_color_t LV_WHITE = lv_color_hex(WHITE);
static const lv_color_t LV_BLACK = lv_color_hex(BLACK);
static const lv_color_t LV_VEX_GREY = lv_color_hex(VEX_GREY);
static const lv_color_t LV_RED = lv_color_hex(RED);
static const lv_color_t LV_ORANGE = lv_color_hex(ORANGE);
static const lv_color_t LV_YELLOW = lv_color_hex(YELLOW);
static const lv_color_t LV_GREEN = lv_color_hex(GREEN);
static const lv_color_t LV_DARK_GREEN = lv_color_hex(DARK_GREEN);
static const lv_color_t LV_LIGHT_BLUE = lv_color_hex(LIGHT_BLUE);
static const lv_color_t LV_BLUE = lv_color_hex(BLUE);
static const lv_color_t LV_DARK_BLUE = lv_color_hex(DARK_BLUE);
static const lv_color_t LV_PINK = lv_color_hex(PINK);
static const lv_color_t LV_INDIGO = lv_color_hex(INDIGO);
static const lv_color_t LV_VIOLET = lv_color_hex(VIOLET);

static size_t canvasSize = 0;
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
static lv_obj_t *digitalSpeedLabel = nullptr;

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
lv_obj_t *create_label(lv_obj_t *screen,
                       lv_coord_t x,
                       lv_coord_t y,
                       lv_coord_t w,
                       lv_coord_t h,
                       const char *text = "<uninitialized>",
                       lv_style_t *style = nullptr);
lv_res_t prev_page(lv_obj_t *btn);
lv_res_t next_page(lv_obj_t *btn);
void create_flywheel_screen(lv_obj_t *screen);
void update_motor(lv_obj_t *label, const char *name, int32_t voltage);
void update_motor_digital(lv_obj_t *label, pros::Motor *motor, bool engaged, const char *name);
lv_res_t drop_log(lv_obj_t *obj);
void drop_screen(uint16_t screen);
void init_screen(uint16_t screen);
void update(Robot *robot);
[[noreturn]] void update_task(void *params);
void create_prev_btn(lv_obj_t *obj);
void create_next_btn(lv_obj_t *obj);
lv_obj_t *create_screen(lv_obj_t *parent, bool beginning = false, bool end = false);
lv_style_t *create_text_color_style(lv_color_t colour);
template<typename ... Args> void set_label_text(lv_obj_t *label, const char *format, Args ... args);

void initialize(Robot *robot) {
  logger::push_section("Initialize Screen");
  lv_init();

  lv_obj_t *base_view = lv_scr_act();
  width = lv_obj_get_width(base_view);
  height = lv_obj_get_height(base_view);
  canvasSize = lv_img_color_format_get_px_size(CANVAS_COLOUR) * width * (height - BASE_HEIGHT);
  canvasBuffer = calloc(canvasSize, 1);
  activeScreen = 0;

  logger::debug("Width: %i\nHeight: %i", width, height);

  lv_obj_t *screen_0 = create_screen(base_view, true);
  lv_obj_t *screen_1 = create_screen(base_view);
  lv_obj_t *screen_2 = create_screen(base_view);
  lv_obj_t *screen_3 = create_screen(base_view, false, true);

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
  logger::pop_section();
}

lv_obj_t *create_screen(lv_obj_t *parent, bool beginning, bool end) {
  lv_obj_t *screen = lv_obj_create(parent, nullptr);
  lv_obj_set_size(screen, width, height);
  lv_obj_set_hidden(screen, true);
  if (!end) create_next_btn(screen);
  if (!beginning) create_prev_btn(screen);
  return screen;
}

void create_prev_btn(lv_obj_t *obj) {
  auto prevBtn = lv_btn_create(obj, nullptr);
  lv_obj_set_pos(prevBtn, 0, static_cast<lv_coord_t>(height - BASE_HEIGHT));
  lv_obj_set_size(prevBtn, BASE_HEIGHT, BASE_HEIGHT);
  lv_btn_set_action(prevBtn, LV_BTN_ACTION_CLICK, prev_page);
}

void create_next_btn(lv_obj_t *obj) {
  auto nextBtn = lv_btn_create(obj, nullptr);
  lv_obj_set_pos(nextBtn, static_cast<lv_coord_t>(width - BASE_HEIGHT), static_cast<lv_coord_t>(height - BASE_HEIGHT));
  lv_obj_set_size(nextBtn, BASE_HEIGHT, BASE_HEIGHT);
  lv_btn_set_action(nextBtn, LV_BTN_ACTION_CLICK, next_page);
}

[[noreturn]] void update_task(void *params) {
  auto *robot = static_cast<Robot *>(params);
  while (true) {
    update(robot);
    pros::delay(50);
  }
}

void update(Robot *robot) {
  set_label_text(uptimeLabel, "Uptime: %u", pros::millis());

  memset(canvasBuffer, VEX_GREY, canvasSize);
  if (activeScreen == 0) {
    update_motor(motorLFLabel, "DT-LF", robot->drivetrain->leftFront->get_voltage());
    update_motor(motorRFLabel, "DT-RF", robot->drivetrain->rightFront->get_voltage());
    update_motor(motorLBLabel, "DT-LB", robot->drivetrain->leftBack->get_voltage());
    update_motor(motorRBLabel, "DT-RB", robot->drivetrain->rightBack->get_voltage());
    update_motor_digital(flywheelLabel, robot->flywheel->motor, robot->flywheel->isEngaged(), "Flywheel");
    update_motor_digital(rollerLabel, nullptr, false, "Roller");
    update_motor_digital(intakeLabel, nullptr, false, "Intake");
    set_label_text(digitalSpeedLabel, "Digital Speed: %i", robot->controller->digital_speed());
  } else if (activeScreen == 1) {
    if (velMotorLF.size() == 100) {
      velMotorLF.erase(velMotorLF.begin());
      velMotorRF.erase(velMotorRF.begin());
      velMotorLB.erase(velMotorLB.begin());
      velMotorRB.erase(velMotorRB.begin());
    }

    auto prevLF = (float) robot->drivetrain->leftFront->get_actual_velocity();
    auto prevRF = (float) robot->drivetrain->rightFront->get_actual_velocity();
    auto prevLB = (float) robot->drivetrain->leftBack->get_actual_velocity();
    auto prevRB = (float) robot->drivetrain->rightBack->get_actual_velocity();
    if (prevLF == INFINITY || prevLF == -1)
      prevLF = 5;
    if (prevRF == INFINITY || prevRF == -1)
      prevRF = 5;
    if (prevLB == INFINITY || prevLB == -1)
      prevLB = 5;
    if (prevRB == INFINITY || prevRB == -1)
      prevRB = 5;
    velMotorLF.push_back(prevLF);
    velMotorRF.push_back(prevRF);
    velMotorLB.push_back(prevLB);
    velMotorRB.push_back(prevRB);
    auto canvasWidth = static_cast<float>(width);
    auto canvasHeight = static_cast<float>(height) - BASE_HEIGHT;
    float heightScale = canvasHeight / 800.0f;
    float widthScale = canvasWidth / 100.0f;

    float x = 0;
    for (int i = (int)velMotorLF.size() - 2; i >= 0; --i) {
      float v = velMotorLF[i];
      lv_canvas_draw_line(drivetrainCanvas, lv_point_t{
          static_cast<lv_coord_t>(canvasWidth - (x * widthScale)),
          static_cast<lv_coord_t>(canvasHeight - std::fabs(prevLF * heightScale))
      }, lv_point_t{
          static_cast<lv_coord_t>(canvasWidth - ((x + 1) * widthScale)),
          static_cast<lv_coord_t>(canvasHeight - BASE_HEIGHT - std::fabs(v * heightScale))
      }, v >= 0 ? LV_RED : LV_BLUE);
      prevLF = v;
      v = velMotorRF[i];
      lv_canvas_draw_line(drivetrainCanvas, lv_point_t{
          static_cast<lv_coord_t>(canvasWidth - (x * widthScale)),
          static_cast<lv_coord_t>(canvasHeight - std::fabs(prevRF * heightScale))
      }, lv_point_t{
          static_cast<lv_coord_t>((float) width - ((x + 1.0f) * widthScale)),
          static_cast<lv_coord_t>((canvasHeight - std::fabs(v * heightScale)))
      }, v >= 0 ? LV_ORANGE : LV_VIOLET);
      prevRF = v;
      v = velMotorLB[i];
      lv_canvas_draw_line(drivetrainCanvas, lv_point_t{
          static_cast<lv_coord_t>(canvasWidth - (x * widthScale)),
          static_cast<lv_coord_t>(canvasHeight - std::fabs(prevLB * heightScale))
      }, lv_point_t{
          static_cast<lv_coord_t>(canvasWidth - ((x + 1) * widthScale)),
          static_cast<lv_coord_t>((canvasHeight - std::fabs(v * heightScale)))
      }, v >= 0 ? LV_YELLOW : LV_GREEN);
      prevLB = v;
      v = velMotorRB[i];
      lv_canvas_draw_line(drivetrainCanvas, lv_point_t{
          static_cast<lv_coord_t>(canvasWidth - (x * widthScale)),
          static_cast<lv_coord_t>(canvasHeight - std::fabs(prevRB * heightScale))
      }, lv_point_t{
          static_cast<lv_coord_t>(canvasWidth - ((x + 1) * widthScale)),
          static_cast<lv_coord_t>((canvasHeight - std::fabs(v * heightScale)))
      }, v >= 0 ? LV_PINK : LV_LIGHT_BLUE);
      prevRB = v;
      ++x;
    }
    lv_obj_invalidate(drivetrainCanvas);
  } else if (activeScreen == 2) {
    if (velFlywheel.size() == 100) {
      velFlywheel.erase(velFlywheel.begin());
    }

    auto prev = (float) robot->flywheel->motor->get_actual_velocity();
    if (prev == INFINITY || prev == -1) {
      prev = 5;
    }
    velFlywheel.push_back(prev);
    auto canvasWidth = static_cast<float>(width);
    auto canvasHeight = static_cast<float>(height) - BASE_HEIGHT;
    float heightScale = canvasHeight / 900.0f;
    float widthScale = canvasWidth / 100.0f;

    float x = 0;
    for (signed int i = (signed int) velFlywheel.size() - 2; i >= 0; --i) {
      float v = velFlywheel[i];
      lv_canvas_draw_line(
          flywheelCanvas, lv_point_t{
              static_cast<lv_coord_t>(canvasWidth - (x * widthScale)),
              static_cast<lv_coord_t>(canvasHeight - std::fabs(prev * heightScale))
          }, lv_point_t{
              static_cast<lv_coord_t>(canvasWidth - ((x + 1) * widthScale)),
              static_cast<lv_coord_t>((canvasHeight - std::fabs(v * heightScale)))
          }, v >= 0 ? LV_BLUE : LV_ORANGE);
      ++x;
      prev = v;
    }
    lv_obj_invalidate(flywheelCanvas);
  }
}

template<typename ... Args> void set_label_text(lv_obj_t *label, const char *format, Args ... args) {
  snprintf(buffer, 32, format, args...);
  lv_label_set_text(label, buffer);
}

void update_motor_digital(lv_obj_t *label, pros::Motor *motor, bool engaged, const char *name) {
  if (motor == nullptr || motor->get_voltage() == INT32_MAX) {
    set_label_text(label, "%s: Disconnected", name);
  } else {
    if (engaged) {
      set_label_text(label, "%s: Enabled", name);
    } else {
      set_label_text(label, "%s: Disabled", name);
    }
  }
}

void update_motor(lv_obj_t *label, const char *name, int32_t voltage) {
  if (voltage == INT32_MAX) {
    set_label_text(label, "%s: Disconnected", name);
  } else {
    set_label_text(label, "%s: %i", name, voltage);
  }
}

void create_info_screen(lv_obj_t *screen) {
  auto halfWidth = static_cast<lv_coord_t>(width / 2);

  create_label(screen, 0, 16 * 0, halfWidth, 16, "Build: " __DATE__ " " __TIME__);
  uptimeLabel = create_label(screen, halfWidth, 16 * 0, halfWidth, 16);

  // Port Statuses
  motorLFLabel = create_label(screen, 0, 16 * 2, halfWidth, 16);
  motorRFLabel = create_label(screen, halfWidth, 16 * 2, halfWidth, 16);
  motorLBLabel = create_label(screen, 0, 16 * 3, halfWidth, 16);
  motorRBLabel = create_label(screen, halfWidth, 16 * 3, halfWidth, 16);
  flywheelLabel = create_label(screen, 0, 16 * 4, halfWidth, 16);
  rollerLabel = create_label(screen, halfWidth, 16 * 4, halfWidth, 16);
  intakeLabel = create_label(screen, 0, 16 * 5, halfWidth, 16);
  digitalSpeedLabel = create_label(screen, halfWidth, 16 * 5, halfWidth, 16);
}

void create_drivetrain_screen(lv_obj_t *screen) {
  auto canvasHeight = static_cast<lv_coord_t>(height - BASE_HEIGHT);
  drivetrainCanvas = lv_canvas_create(screen, nullptr);
  velMotorLF.reserve(100);
  velMotorRF.reserve(100);
  velMotorLB.reserve(100);
  velMotorRB.reserve(100);
  lv_obj_set_pos(drivetrainCanvas, 0, 0);
  lv_obj_set_size(drivetrainCanvas, width, canvasHeight);
  lv_canvas_set_buffer(drivetrainCanvas, canvasBuffer, width, canvasHeight, CANVAS_COLOUR);

  auto qtrWidth = static_cast<lv_coord_t>((width - 64) / 4);
  auto *style = new lv_style_t{};
  lv_style_copy(style, &lv_style_plain_color);
  style->text.color = LV_RED;
  create_label(screen,
               32,
               static_cast<lv_coord_t>(height - 32),
               qtrWidth,
               16,
               "Left Front (+)",
               create_text_color_style(LV_RED));
  create_label(screen,
               32,
               static_cast<lv_coord_t>(height - 16),
               qtrWidth,
               16,
               "Left Front (-)",
               create_text_color_style(LV_BLUE));
  
  create_label(screen,
               static_cast<lv_coord_t>(32 + qtrWidth),
               static_cast<lv_coord_t>(height - 32),
               qtrWidth,
               16,
               "Right Front (+)",
               create_text_color_style(LV_ORANGE));
  create_label(screen,
               static_cast<lv_coord_t>(32 + qtrWidth),
               static_cast<lv_coord_t>(height - 16),
               qtrWidth,
               16,
               "Right Front (-)",
               create_text_color_style(LV_VIOLET));
  
  create_label(screen,
               static_cast<lv_coord_t>(32 + qtrWidth * 2),
               static_cast<lv_coord_t>(height - 32),
               qtrWidth,
               16,
               "Left Back (+)",
               create_text_color_style(LV_YELLOW));
  create_label(screen,
               static_cast<lv_coord_t>(32 + qtrWidth * 2),
               static_cast<lv_coord_t>(height - 16),
               qtrWidth,
               16,
               "Left Back (-)",
               create_text_color_style(LV_GREEN));
  
  create_label(screen,
               static_cast<lv_coord_t>(32 + qtrWidth * 3),
               static_cast<lv_coord_t>(height - 32),
               qtrWidth,
               16,
               "Right Back (+)",
               create_text_color_style(LV_PINK));
  create_label(screen,
               static_cast<lv_coord_t>(32 + qtrWidth * 3),
               static_cast<lv_coord_t>(height - 16),
               qtrWidth,
               16,
               "Right Back (-)",
               create_text_color_style(LV_LIGHT_BLUE));
}

void create_flywheel_screen(lv_obj_t *screen) {
  velFlywheel.reserve(100);
  flywheelCanvas = lv_canvas_create(screen, nullptr);
  lv_obj_set_pos(flywheelCanvas, 0, 0);
  lv_obj_set_size(flywheelCanvas, width, static_cast<lv_coord_t>(height - BASE_HEIGHT));
  lv_canvas_set_buffer(flywheelCanvas, canvasBuffer, width, static_cast<lv_coord_t>(height - BASE_HEIGHT), CANVAS_COLOUR);

  create_label(screen,
               32,
               static_cast<lv_coord_t>(height - 32),
               static_cast<lv_coord_t>(width / 4), 16, "Flywheel (+)",
               create_text_color_style(LV_BLUE)
               );
  create_label(screen,
               32,
               static_cast<lv_coord_t>(height - 16),
               static_cast<lv_coord_t>(width / 4),
               16, "Flywheel (-)",
               create_text_color_style(LV_ORANGE)
               );
}

void create_log_screen(lv_obj_t *screen) {
  logs = lv_list_create(screen, nullptr);
  lv_obj_set_pos(logs, 0, 0);
  lv_obj_set_size(logs, width, static_cast<lv_coord_t>(height - BASE_HEIGHT));
}

void write_line(const char *string, Colour colour) {
  if (logs != nullptr) {
    if (lv_list_get_size(logs) >= 12) {
      lv_list_remove(logs, 0);
    }

    lv_obj_t *label = lv_list_add(logs, nullptr, string, drop_log);
    lv_obj_set_style(label, create_text_color_style(lv_color_hex(colour)));
  }
}

void write_line(const std::string &string, Colour colour) {
  if (logs != nullptr) {
    write_line(string.c_str(), colour);
  }
}

lv_style_t *create_text_color_style(lv_color_t colour) {
  auto *style = new lv_style_t{};
  lv_style_copy(style, &lv_style_plain_color);
  style->text.color = colour;
  return style;
}

lv_obj_t *create_label(lv_obj_t *screen,
                       lv_coord_t x,
                       lv_coord_t y,
                       lv_coord_t w,
                       lv_coord_t h,
                       const char *text,
                       lv_style_t *style) {
  auto label = lv_label_create(screen, nullptr);
  if (style != nullptr) {
    lv_obj_set_style(label, style);
  }
  lv_obj_set_pos(label, x, y);
  lv_obj_set_size(label, w, h);
  lv_label_set_text(label, text);
  return label;
}

lv_res_t prev_page(lv_obj_t *btn) {
  drop_screen(activeScreen);
  init_screen(--activeScreen);
  return LV_RES_OK;
}

lv_res_t next_page(lv_obj_t *btn) {
  drop_screen(activeScreen);
  init_screen(++activeScreen);
  return LV_RES_OK;
}

void init_screen(uint16_t screen) {
  logger::push_section("Create screen");
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
  delete lv_obj_get_style(obj); // safe as we create a new style for each button
  return LV_RES_INV;
}
}
