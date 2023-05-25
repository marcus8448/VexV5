#include "screen/screen.hpp"
#include "debug/logger.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_objx/lv_btn.h"
#pragma GCC diagnostic pop

#include <algorithm>
#include <cstring>
#include <map>
#include <vector>

namespace screen {
extern void* canvasBuffer;
static std::vector<Screen *> *registry = new std::vector<Screen *>();
static std::map<Screen *, lv_obj_t **> *screens = new std::map<Screen *, lv_obj_t **>();
static Screen *activeScreen = nullptr;

static size_t canvasSize = 0;

lv_res_t prev_page([[maybe_unused]] lv_obj_t *btn);
lv_res_t next_page([[maybe_unused]] lv_obj_t *btn);
void hide_screen(Screen *screen);
void show_screen(Screen *screen);
void update();
[[noreturn]] void update_task([[maybe_unused]] void *params);
lv_obj_t *create_prev_btn(lv_obj_t *obj);
lv_obj_t *create_next_btn(lv_obj_t *obj);
void create_screen(lv_obj_t *output[], lv_obj_t *parent);

void initialize() {
  scopePush("Initialize LVGL");
  lv_init();
  scopePop();

  lv_obj_t *base_view = lv_scr_act();
  activeScreen = registry->at(0);

  scopePush("Create screens");
  for (auto screen : *registry) {
    auto **lvObjs = new lv_obj_t *[3];
    create_screen(lvObjs, base_view);
    screens->emplace(screen, lvObjs);
    screen->initialize(lvObjs[0]);
  }
  scopePop();

  show_screen(activeScreen);
  pros::c::task_create(update_task, nullptr, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Screen update");
}

void create_screen(lv_obj_t *output[], lv_obj_t *parent) {
  lv_obj_t *screen = lv_obj_create(parent, nullptr);
  lv_obj_set_size(screen, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_hidden(screen, true);

  output[0] = screen;
  output[1] = create_prev_btn(screen);
  output[2] = create_next_btn(screen);
}

[[noreturn]] void update_task([[maybe_unused]] void *params) {
  while (true) {
    update();
    pros::c::delay(SCREEN_UPDATE_RATE);
  }
}

void update() {
  if (canvasBuffer != nullptr) {
    memset(canvasBuffer, 0x00000000, canvasSize);
  }
  activeScreen->update();
}

void addScreen(Screen *screen) { registry->push_back(screen); }

void removeScreen(Screen *screen) {
  if (activeScreen == screen) {
    hide_screen(activeScreen);
    auto newScreen = registry->front() != activeScreen ? registry->front() : registry->back();
    activeScreen = newScreen;
    show_screen(newScreen);
  }

  registry->erase(std::remove(registry->begin(), registry->end(), screen), registry->end());

  delete[] *screens->at(screen);
  screens->erase(screen);

  lv_obj_set_hidden(screens->at(registry->front())[1], true);
  lv_obj_set_hidden(screens->at(registry->back())[2], true);
}

lv_obj_t *create_prev_btn(lv_obj_t *obj) {
  auto prevBtn = lv_btn_create(obj, nullptr);
  lv_obj_set_pos(prevBtn, 0, static_cast<lv_coord_t>(SCREEN_HEIGHT - BUTTON_SIZE));
  lv_obj_set_size(prevBtn, BUTTON_SIZE, BUTTON_SIZE);
  lv_btn_set_action(prevBtn, LV_BTN_ACTION_CLICK, prev_page);
  return prevBtn;
}

lv_obj_t *create_next_btn(lv_obj_t *obj) {
  auto nextBtn = lv_btn_create(obj, nullptr);
  lv_obj_set_pos(nextBtn, static_cast<lv_coord_t>(SCREEN_WIDTH - BUTTON_SIZE), static_cast<lv_coord_t>(SCREEN_HEIGHT - BUTTON_SIZE));
  lv_obj_set_size(nextBtn, BUTTON_SIZE, BUTTON_SIZE);
  lv_btn_set_action(nextBtn, LV_BTN_ACTION_CLICK, next_page);
  return nextBtn;
}

lv_res_t prev_page([[maybe_unused]] lv_obj_t *btn) {
  debug("Screen switching to previous page");
  hide_screen(activeScreen);
  auto x = std::find(registry->begin(), registry->end(), activeScreen);
  auto newScreen = registry->at(x - registry->begin() - 1);
  show_screen(newScreen);
  activeScreen = newScreen;
  return LV_RES_OK;
}

lv_res_t next_page([[maybe_unused]] lv_obj_t *btn) {
  debug("Screen switching to next page");
  hide_screen(activeScreen);
  auto x = std::find(registry->begin(), registry->end(), activeScreen);
  auto newScreen = registry->at(x - registry->begin() + 1);
  show_screen(newScreen);
  activeScreen = newScreen;
  return LV_RES_OK;
}

void show_screen(Screen *screen) {
  auto objects = screens->at(screen);
  lv_obj_set_hidden(objects[0], false);
  size_t index = std::find(registry->begin(), registry->end(), screen) - registry->begin();
  lv_obj_set_hidden(objects[1], index == 0);
  lv_obj_set_hidden(objects[2], index == registry->size() - 1);
}

void hide_screen(Screen *screen) { lv_obj_set_hidden(screens->at(screen)[0], true); }
} // namespace screen
