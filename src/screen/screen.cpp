#include <cstdlib>
#include <cstring>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_btn.h"
#pragma GCC diagnostic pop

#include "logger.hpp"
#include "pros/rtos.hpp"
#include "screen/screen.hpp"
#include "screen/lvgl_util.hpp"

namespace screen {

static std::vector<Screen *> registry;
static std::vector<lv_obj_t *> screens;
static uint16_t activeScreen = -1;

static size_t canvasSize = 0;
static int16_t width = 0;
static int16_t height = 0;

lv_res_t prev_page(lv_obj_t *btn);
lv_res_t next_page(lv_obj_t *btn);
void destroy_screen(uint16_t screen);
void init_screen(uint16_t screen);
void update(robot::Robot *robot);
[[noreturn]] void update_task(void *params);
void create_prev_btn(lv_obj_t *obj);
void create_next_btn(lv_obj_t *obj);
lv_obj_t *create_screen(lv_obj_t *parent, bool beginning = false, bool end = false);


void initialize(robot::Robot *robot) {
  logger::push_section("Initialize Screen");
  logger::push_section("Initialize LVGL");
  lv_init();
  logger::pop_section();

  lv_obj_t *base_view = lv_scr_act();
  width = lv_obj_get_width(base_view);
  height = lv_obj_get_height(base_view);
  canvasSize = lv_img_color_format_get_px_size(CANVAS_COLOUR) * width * (height - BASE_HEIGHT);
  canvasBuffer = calloc(canvasSize, 1);
  activeScreen = 0;

  logger::debug("Width: %i\nHeight: %i", width, height);
  logger::push_section("Create screens");
  for (size_t i = 0; i < registry.size(); i++) {
    screens[i] = create_screen(base_view, i == 0, i == registry.size()-1);
    registry[i]->create(screens[i], width, height);
  }
  logger::pop_section();

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

[[noreturn]] void update_task(void *params) {
  auto *robot = static_cast<robot::Robot *>(params);
  while (true) {
    update(robot);
    pros::delay(50);
  }
}

void update(robot::Robot *robot) {
  memset(canvasBuffer, 0x00000000, canvasSize);
  registry[activeScreen]->update(robot);
}

void add_screen(Screen *screen) {
  registry.push_back(screen);
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

lv_res_t prev_page(lv_obj_t *btn) {
  destroy_screen(activeScreen);
  init_screen(--activeScreen);
  return LV_RES_OK;
}

lv_res_t next_page(lv_obj_t *btn) {
  destroy_screen(activeScreen);
  init_screen(++activeScreen);
  return LV_RES_OK;
}

void init_screen(uint16_t screen) {
  logger::push_section("Create screen");
  lv_obj_set_hidden(screens[screen], false);
  registry[screen]->initialize(width, height);
  logger::pop_section();
}

void destroy_screen(uint16_t screen) {
  logger::push_section("Drop screen");
  lv_obj_set_hidden(screens[screen], true);
  registry[screen]->destroy(screens[screen]);
  logger::pop_section();
}
}
