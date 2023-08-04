#include "screen/screen.hpp"
#include "debug/logger.hpp"
#include "tasks.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "liblvgl/lvgl.h"
#include "liblvgl/lv_api_map.h"
#pragma GCC diagnostic pop

#include <algorithm>
#include <cstring>
#include <map>
#include <vector>

namespace screen {
extern void *canvasBuffer;
static std::vector<Screen *> *registry = new std::vector<Screen *>();

static Screen* activeScreen = nullptr;
static int screenIndex = -1;

static lv_obj_t *currentScreen = nullptr;

static lv_obj_t *previousBtn = nullptr;
static lv_obj_t *nextBtn = nullptr;

void switch_to_screen(int screen);
void prev_page([[maybe_unused]] lv_event_t * event);
void next_page([[maybe_unused]] lv_event_t * event);
void update();
[[noreturn]] void update_task([[maybe_unused]] void *params);

void initialize() {
  scopePush("Initialize LVGL");
  lv_init();
  scopePop();

  if (registry->empty()) {
    switch_to_screen(-1);
  } else {
    switch_to_screen(0);
  }

  createTask("Screen update", update_task, nullptr, TASK_PRIORITY_DEFAULT - 1, 0x2000);
}

void switch_to_screen(int screen) {
  info("TXX %p", lv_disp_get_default());
  lv_obj_t *base = lv_obj_create(nullptr);
  printf("%p\n", base);
  info("XX");
  lv_scr_load(base);
  info("TX");

  lv_obj_set_size(base, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_add_flag(base, LV_OBJ_FLAG_HIDDEN);

  if (currentScreen != nullptr) {
    if (activeScreen != nullptr) {
      activeScreen->cleanup();
    }

    lv_obj_set_parent(previousBtn, base);
    lv_obj_set_parent(nextBtn, base);

    lv_obj_del(currentScreen);
  } else {
    previousBtn = lv_btn_create(base);
    lv_obj_set_pos(previousBtn, 0, static_cast<lv_coord_t>(SCREEN_HEIGHT - BUTTON_SIZE));
    lv_obj_set_size(previousBtn, BUTTON_SIZE, BUTTON_SIZE);
    lv_obj_add_event_cb(previousBtn, prev_page, LV_EVENT_CLICKED, nullptr);

    nextBtn = lv_btn_create(base);
    lv_obj_set_pos(nextBtn, static_cast<lv_coord_t>(SCREEN_WIDTH - BUTTON_SIZE),
                   static_cast<lv_coord_t>(SCREEN_HEIGHT - BUTTON_SIZE));
    lv_obj_set_size(nextBtn, BUTTON_SIZE, BUTTON_SIZE);
    lv_obj_add_event_cb(nextBtn, next_page, LV_EVENT_CLICKED, nullptr);
  }

  screenIndex = screen;
  currentScreen = base;

  if (screen > 0 && screen < static_cast<int>(registry->size())) {
    activeScreen = (*registry)[screen];
    activeScreen->initialize(base);
  }

  if (screenIndex > 0) {
    lv_obj_clear_flag(previousBtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(previousBtn);
  } else {
    lv_obj_add_flag(previousBtn, LV_OBJ_FLAG_HIDDEN);
  }

  if (screenIndex + 1 < static_cast<int>(registry->size())) {
    lv_obj_clear_flag(nextBtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(nextBtn);
  } else {
    lv_obj_add_flag(nextBtn, LV_OBJ_FLAG_HIDDEN);
  }
}

[[noreturn]] void update_task([[maybe_unused]] void *params) {
  while (true) {
    update();
    pros::c::delay(SCREEN_UPDATE_RATE);
  }
}

void update() {
  if (canvasBuffer != nullptr) {
    memset(canvasBuffer, 0x00000000, lv_img_buf_get_img_size(CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR));
  }
  activeScreen->update();
}

void addScreen(Screen *screen) {
  registry->push_back(screen);

  if (currentScreen != nullptr) {
    if (screenIndex + 2 == static_cast<int>(registry->size())) {
      lv_obj_clear_flag(nextBtn, LV_OBJ_FLAG_HIDDEN);
      lv_obj_move_foreground(nextBtn);
    }

    if (screenIndex == -1) {
      switch_to_screen(0);
    }
  }
}

void removeScreen(Screen *screen) {
  int index = std::find(registry->begin(), registry->end(), screen) - registry->begin();
  if (screenIndex == index) {
    if (index + 1 == static_cast<int>(registry->size())) {
      switch_to_screen(--screenIndex);
    } else {
      switch_to_screen(screenIndex + 1);
    }
  } else if (index < screenIndex) {
    screenIndex--;
  }

  registry->erase(std::remove(registry->begin(), registry->end(), screen), registry->end());
}

void prev_page([[maybe_unused]] lv_event_t * event) {
  debug("Screen switching to previous page");
  switch_to_screen(--screenIndex);
}

void next_page([[maybe_unused]] lv_event_t * event) {
  debug("Screen switching to next page");
  switch_to_screen(++screenIndex);
}
} // namespace screen
