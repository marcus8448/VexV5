#include "screen/screen.hpp"
#include "debug/logger.hpp"
#include "pros/rtos.h"
#include "tasks.hpp"

#include "liblvgl/lv_api_map.h"

#include <algorithm>
#include <vector>

namespace screen {
static std::vector<Screen *> registry = std::vector<Screen *>();

static Screen *activeScreen = nullptr;
static int screenIndex = -1;

static lv_obj_t *previousBtn = nullptr;
static lv_obj_t *nextBtn = nullptr;

void switch_to_screen(int screen);
void prev_page([[maybe_unused]] lv_event_t *event);
void next_page([[maybe_unused]] lv_event_t *event);
void update();
[[noreturn]] void update_task([[maybe_unused]] void *params);

void initialize() {
  lv_theme_default_init(lv_disp_get_default(), LV_THEME_DEFAULT_COLOR_PRIMARY, LV_THEME_DEFAULT_COLOR_SECONDARY,
                        LV_THEME_DEFAULT_DARK, LV_THEME_DEFAULT_FONT_NORMAL);

  previousBtn = lv_btn_create(lv_scr_act());
  lv_obj_set_pos(previousBtn, 0, static_cast<lv_coord_t>(SCREEN_HEIGHT - BUTTON_SIZE));
  lv_obj_set_size(previousBtn, BUTTON_SIZE, BUTTON_SIZE);
  lv_obj_add_event_cb(previousBtn, prev_page, LV_EVENT_CLICKED, nullptr);

  nextBtn = lv_btn_create(lv_scr_act());
  lv_obj_set_pos(nextBtn, static_cast<lv_coord_t>(SCREEN_WIDTH - BUTTON_SIZE),
                 static_cast<lv_coord_t>(SCREEN_HEIGHT - BUTTON_SIZE));
  lv_obj_set_size(nextBtn, BUTTON_SIZE, BUTTON_SIZE);
  lv_obj_add_event_cb(nextBtn, next_page, LV_EVENT_CLICKED, nullptr);

  if (registry.empty()) {
    info("no screens!");
    switch_to_screen(-1);
  } else {
    switch_to_screen(0);
  }

  rtos::createTask("Screen update", update_task, nullptr, TASK_PRIORITY_DEFAULT - 1, 0x2000);
}

void switch_to_screen(int screen) {
  if (activeScreen != nullptr) {
    Screen *temp = activeScreen;
    activeScreen = nullptr;
    temp->cleanup();
  }

  screenIndex = screen;

  if (screen >= 0 && screen < static_cast<int>(registry.size())) {
    Screen *scrn = registry[screen];
    scrn->initialize(lv_scr_act());
    activeScreen = scrn;
  }

  if (screenIndex > 0) {
    lv_obj_clear_flag(previousBtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(previousBtn);
  } else {
    lv_obj_add_flag(previousBtn, LV_OBJ_FLAG_HIDDEN);
  }

  if (screenIndex + 1 < static_cast<int>(registry.size())) {
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
  if (activeScreen != nullptr) {
    activeScreen->update();
  }
}

void addScreen(Screen *screen) {
  registry.push_back(screen);

  if (nextBtn != nullptr) {
    if (screenIndex + 2 == static_cast<int>(registry.size())) {
      lv_obj_clear_flag(nextBtn, LV_OBJ_FLAG_HIDDEN);
      lv_obj_move_foreground(nextBtn);
    }

    if (screenIndex == -1) {
      switch_to_screen(0);
    }
  }
}

void removeScreen(Screen *screen) {
  int index = std::find(registry.begin(), registry.end(), screen) - registry.begin();
  if (screenIndex == index) {
    if (index + 1 == static_cast<int>(registry.size())) {
      switch_to_screen(--screenIndex);
    } else {
      switch_to_screen(screenIndex + 1);
    }
  } else if (index < screenIndex) {
    screenIndex--;
  }

  if (screenIndex == 0) {
    lv_obj_add_flag(previousBtn, LV_OBJ_FLAG_HIDDEN);
  }

  registry.erase(std::remove(registry.begin(), registry.end(), screen), registry.end());
}

void prev_page([[maybe_unused]] lv_event_t *event) {
  scopePush("Screen switching to previous page");
  switch_to_screen(--screenIndex);
  scopePop();
}

void next_page([[maybe_unused]] lv_event_t *event) {
  scopePush("Screen switching to next page");
  switch_to_screen(++screenIndex);
  scopePop();
}
} // namespace screen
