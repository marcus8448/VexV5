#include "screen.hpp"
#include "debug/logger.hpp"
#include "pros/rtos.h"

#include <vector>

namespace screen {
constexpr lv_coord_t WIDTH = 480;
constexpr lv_coord_t HEIGHT = 240;

static std::vector<ScreenConstructor> registry = std::vector<ScreenConstructor>();

static std::unique_ptr<Screen> activeScreen;
static size_t screenIndex = std::numeric_limits<size_t>::max();

static lv_obj_t *previousBtn = nullptr;
static lv_obj_t *nextBtn = nullptr;

static pros::mutex_t mutex = pros::c::mutex_create();
static robot::Robot *_robot;

void switch_to_screen(size_t screen);
void prev_page([[maybe_unused]] lv_event_t *event);
void next_page([[maybe_unused]] lv_event_t *event);
void update();
void update_task([[maybe_unused]] lv_timer_t *);

void initialize(robot::Robot &robot) {
  _robot = &robot;
  lv_theme_default_init(lv_disp_get_default(), LV_THEME_DEFAULT_COLOR_PRIMARY, LV_THEME_DEFAULT_COLOR_SECONDARY,
                        LV_THEME_DEFAULT_DARK, LV_THEME_DEFAULT_FONT_NORMAL);

  previousBtn = lv_btn_create(lv_scr_act());
  lv_obj_set_pos(previousBtn, 0, coord(HEIGHT - BUTTON_SIZE));
  lv_obj_set_size(previousBtn, BUTTON_SIZE, BUTTON_SIZE);
  lv_obj_add_event_cb(previousBtn, prev_page, LV_EVENT_CLICKED, nullptr);

  nextBtn = lv_btn_create(lv_scr_act());
  lv_obj_set_pos(nextBtn, coord(WIDTH - BUTTON_SIZE), coord(HEIGHT - BUTTON_SIZE));
  lv_obj_set_size(nextBtn, BUTTON_SIZE, BUTTON_SIZE);
  lv_obj_add_event_cb(nextBtn, next_page, LV_EVENT_CLICKED, nullptr);

  if (registry.empty()) {
    logger::info("no screens!");
    switch_to_screen(std::numeric_limits<size_t>::max());
  } else {
    switch_to_screen(0);
  }

  lv_timer_t *timer = lv_timer_create(update_task, UPDATE_RATE, nullptr);
  lv_timer_set_repeat_count(timer, -1);
}

void switch_to_screen(const size_t screen) {
  while (!pros::c::mutex_take(mutex, 1000)) {
    pros::c::delay(5);
  }

  activeScreen.reset(nullptr);

  screenIndex = screen;

  if (screen != std::numeric_limits<size_t>::max() && screen < registry.size()) {
    activeScreen = registry[screen](*_robot, lv_scr_act(), WIDTH, HEIGHT);
  }

  if (screen != std::numeric_limits<size_t>::max() && screenIndex > 0) {
    lv_obj_clear_flag(previousBtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(previousBtn);
  } else {
    lv_obj_add_flag(previousBtn, LV_OBJ_FLAG_HIDDEN);
  }

  if (screen != std::numeric_limits<size_t>::max() && screenIndex + 1 < registry.size()) {
    lv_obj_clear_flag(nextBtn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(nextBtn);
  } else {
    lv_obj_add_flag(nextBtn, LV_OBJ_FLAG_HIDDEN);
  }
  pros::c::mutex_give(mutex);
}

void update_task([[maybe_unused]] lv_timer_t *timer) { update(); }

void update() {
  if (activeScreen != nullptr && pros::c::mutex_take(mutex, 100)) {
    activeScreen->update();
    pros::c::mutex_give(mutex);
  }
}

void addScreen(const ScreenConstructor &screen) {
  while (!pros::c::mutex_take(mutex, 1000)) {
    pros::c::delay(5);
  }

  registry.emplace_back(screen);

  if (nextBtn != nullptr) {
    if (screenIndex + 2 == registry.size()) {
      lv_obj_clear_flag(nextBtn, LV_OBJ_FLAG_HIDDEN);
      lv_obj_move_foreground(nextBtn);
    }

    if (screenIndex == std::numeric_limits<size_t>::max()) {
      switch_to_screen(0);
    }
  }
  pros::c::mutex_give(mutex);
}

void removeScreen(const ScreenConstructor &screen) {
  // while (!pros::c::mutex_take(mutex, 1000)) {
  //   pros::c::delay(5);
  // }
  //
  // int index = std::find(registry.begin(), registry.end(), screen) - registry.begin();
  // if (screenIndex == index) {
  //   if (index + 1 == registry.size()) {
  //     switch_to_screen(--screenIndex);
  //   } else {
  //     switch_to_screen(screenIndex + 1);
  //   }
  // } else if (index < screenIndex) {
  //   screenIndex--;
  // }
  //
  // if (screenIndex == 0) {
  //   lv_obj_add_flag(previousBtn, LV_OBJ_FLAG_HIDDEN);
  // }
  //
  // std::erase(registry, &screen);
  // pros::c::mutex_give(mutex);
}

void prev_page([[maybe_unused]] lv_event_t *event) {
  logger::scope("ScreenPrev");
  switch_to_screen(--screenIndex);
  logger::endScope();
}

void next_page([[maybe_unused]] lv_event_t *event) {
  logger::scope("ScreenNext");
  switch_to_screen(++screenIndex);
  logger::endScope();
}

void LvObjDeleter::operator()(lv_obj_t *obj) const { lv_obj_del(obj); }

Screen::Screen(robot::Robot &robot, const lv_coord_t width, const lv_coord_t height)
    : width(width), height(height), robot(robot) {}

Screen::~Screen() = default;
} // namespace screen
