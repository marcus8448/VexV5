#include "screen/screen.hpp"
#include "debug/logger.hpp"
#include "pros/rtos.h"
#include "tasks.hpp"

#include "liblvgl/lv_api_map.h"

#include <vector>

namespace screen {
constexpr lv_coord_t WIDTH = 480;
constexpr lv_coord_t HEIGHT = 240;

static std::vector<
    std::function<std::unique_ptr<Screen>(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height)>>
    registry = std::vector<std::function<std::unique_ptr<Screen>(robot::Robot &robot, lv_obj_t *screen,
                                                                 lv_coord_t width, lv_coord_t height)>>();

static std::unique_ptr<Screen> activeScreen;
static int screenIndex = -1;

static lv_obj_t *previousBtn = nullptr;
static lv_obj_t *nextBtn = nullptr;

static pros::mutex_t mutex = pros::c::mutex_create();
static robot::Robot *robot;

void switch_to_screen(int screen);
void prev_page([[maybe_unused]] lv_event_t *event);
void next_page([[maybe_unused]] lv_event_t *event);
void update();
void update_task([[maybe_unused]] lv_timer_t *);

void initialize(robot::Robot &bot) {
  robot = &bot;
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
    switch_to_screen(-1);
  } else {
    switch_to_screen(0);
  }

  lv_timer_t *timer = lv_timer_create(update_task, UPDATE_RATE, nullptr);
  lv_timer_set_repeat_count(timer, -1);
}

void switch_to_screen(int screen) {
  while (!pros::c::mutex_take(mutex, 1000)) {
    pros::c::delay(5);
  }

  activeScreen.reset(nullptr);

  screenIndex = screen;

  if (screen >= 0 && screen < static_cast<int>(registry.size())) {
    activeScreen = registry[screen](*robot, lv_scr_act(), WIDTH, HEIGHT);
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
  pros::c::mutex_give(mutex);
}

void update_task([[maybe_unused]] lv_timer_t *timer) { update(); }

void update() {
  if (activeScreen != nullptr && pros::c::mutex_take(mutex, 100)) {
    activeScreen->update();
    pros::c::mutex_give(mutex);
  }
}

void addScreen(const std::function<std::unique_ptr<Screen>(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width,
                                                           lv_coord_t height)> &screen) {
  while (!pros::c::mutex_take(mutex, 1000)) {
    pros::c::delay(5);
  }

  registry.emplace_back(screen);

  if (nextBtn != nullptr) {
    if (screenIndex + 2 == static_cast<int>(registry.size())) {
      lv_obj_clear_flag(nextBtn, LV_OBJ_FLAG_HIDDEN);
      lv_obj_move_foreground(nextBtn);
    }

    if (screenIndex == -1) {
      switch_to_screen(0);
    }
  }
  pros::c::mutex_give(mutex);
}

void removeScreen(const std::function<std::unique_ptr<Screen>(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width,
                                                              lv_coord_t height)> &screen) {
  //  while (!pros::c::mutex_take(mutex, 1000)) {
  //    pros::c::delay(5);
  //  }
  //
  //  int index = std::find(registry.begin(), registry.end(), screen) - registry.begin();
  //  if (screenIndex == index) {
  //    if (index + 1 == static_cast<int>(registry.size())) {
  //      switch_to_screen(--screenIndex);
  //    } else {
  //      switch_to_screen(screenIndex + 1);
  //    }
  //  } else if (index < screenIndex) {
  //    screenIndex--;
  //  }
  //
  //  if (screenIndex == 0) {
  //    lv_obj_add_flag(previousBtn, LV_OBJ_FLAG_HIDDEN);
  //  }
  //
  //  registry.erase(std::remove(registry.begin(), registry.end(), &screen), registry.end());
  //  pros::c::mutex_give(mutex);
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

void LvObjDeleter::operator()(lv_obj_t *obj) { lv_obj_del(obj); }

Screen::Screen(robot::Robot &robot, lv_coord_t width, lv_coord_t height) : width(width), height(height), robot(robot) {}

Screen::~Screen() = default;
} // namespace screen
