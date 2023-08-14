#include "screen/autonomous_select.hpp"
#include "control/autonomous/autonomous.hpp"
#include "robot/robot.hpp"
#include "screen/colour.hpp"
#include "screen/screen.hpp"

namespace screen {
struct StateHolder {
  AutonomousSelect *instance;
  lv_obj_t *btn;
};

static void click(lv_event_t *event);

AutonomousSelect::AutonomousSelect(robot::Robot &robot) : robot(robot) {}

void AutonomousSelect::initialize(lv_obj_t *screen) {
  this->list = lv_list_create(screen);
  lv_obj_set_pos(this->list, 0, 0);
  lv_obj_set_size(this->list, SCREEN_WIDTH, SCREEN_HEIGHT);

  auto programs = control::autonomous::getPrograms();
  for (auto const &[name, program] : programs) {
    if (name.starts_with('!'))
      continue;

    lv_obj_t *btn = lv_list_add_btn(this->list, nullptr, name.c_str());
    lv_obj_add_event_cb(btn, ::screen::click, LV_EVENT_CLICKED, new StateHolder{this, btn});
    lv_obj_set_style_text_color(btn, colour::GREEN, LV_STATE_CHECKED);
    if (name == this->robot.autonomous) {
      this->selected = btn;
      lv_obj_add_state(btn, LV_STATE_CHECKED);
    } else {
      lv_obj_clear_state(btn, LV_STATE_CHECKED);
    }
  }
}

void AutonomousSelect::update() {}

void AutonomousSelect::click(lv_obj_t *btn) {
  if (this->selected != nullptr) {
    lv_obj_clear_state(this->selected, LV_STATE_CHECKED);
  }
  this->selected = btn;

  auto name = std::string(lv_list_get_btn_text(this->list, btn));

  if (this->robot.controller != nullptr) {
    this->robot.controller->setLine(0, 0, name.c_str());
  }

  this->robot.autonomous = name;
  lv_obj_add_state(this->selected, LV_STATE_CHECKED);
}

void AutonomousSelect::cleanup() {
  lv_obj_del_async(this->list);
  lv_obj_del_async(this->selected);

  this->list = nullptr;
  this->selected = nullptr;
}

static void click(lv_event_t *event) {
  auto state = static_cast<StateHolder *>(event->user_data);
  state->instance->click(state->btn);
}
} // namespace screen
