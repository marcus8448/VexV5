#include "screen/autonomous_select.hpp"
#include "control/autonomous/autonomous.hpp"
#include "robot/robot.hpp"
#include "screen/colour.hpp"
#include "screen/lvgl_util.hpp"
#include "screen/screen.hpp"

namespace screen {
AutonomousSelect *autonomous_select_instance = nullptr;

void click(lv_event_t *event);

AutonomousSelect::AutonomousSelect(robot::Robot &robot) : robot(robot) { autonomous_select_instance = this; }

void AutonomousSelect::initialize(lv_obj_t *screen) {
  this->list = lv_list_create(screen);
  lv_obj_set_pos(this->list, 0, 0);
  lv_obj_set_size(this->list, SCREEN_WIDTH, static_cast<lv_coord_t>(SCREEN_HEIGHT - BUTTON_SIZE));

  auto programs = control::autonomous::getPrograms();
  for (auto const &[name, program] : programs) {
    lv_obj_t *btn = lv_list_add_btn(this->list, nullptr, name.c_str());
    lv_obj_add_event_cb(btn, ::screen::click, LV_EVENT_CLICKED, btn);
    if (name == *control::autonomous::getActive()) {
      this->selected = btn;
      lv_obj_set_style_text_color(this->list, colour::GREEN, LV_PART_ANY);
    } else {
      lv_obj_set_style_text_color(btn, colour::WHITE, LV_PART_ANY);
    }
  }
}

void AutonomousSelect::update() {}

void AutonomousSelect::click(lv_obj_t *btn) {
  if (this->selected != nullptr) {
    lv_obj_set_style_text_color(this->selected, colour::WHITE, LV_PART_ANY);
  }
  this->selected = btn;

  auto name = new std::string(lv_list_get_btn_text(this->list, btn));

  if (this->robot.controller != nullptr) {
    this->robot.controller->setLine(0, 0, name->c_str());
  }

  control::autonomous::set_active(name);
  lv_obj_set_style_text_color(this->selected, colour::GREEN, LV_PART_ANY);
}

void AutonomousSelect::cleanup() {}

void click(lv_event_t *event) {
  auto btn = static_cast<lv_obj_t *>(event->user_data);
  autonomous_select_instance->click(btn);
}

} // namespace screen
