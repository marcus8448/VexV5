#include "screen/autonomous_select.hpp"
#include "control/autonomous/autonomous.hpp"
#include "robot/robot.hpp"
#include "screen/screen.hpp"

namespace screen {
AutonomousSelect::AutonomousSelect(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height)
    : Screen(robot, width, height), list(lv_list_create(screen)) {
  lv_obj_set_pos(this->list.get(), 0, 0);
  lv_obj_set_size(this->list.get(), width, height);

  auto programs = control::autonomous::getPrograms();
  for (auto const &[name, program] : programs) {
    if (name.starts_with('!')) {
      continue;
    }

    lv_obj_t *btn = lv_list_add_btn(this->list.get(), nullptr, name.data());
    lv_obj_add_event_cb(btn, SCREEN_CB_ADV(AutonomousSelect, click), LV_EVENT_CLICKED, this);
    lv_obj_set_style_text_color(btn, lv_color_hex(0x00FF00), LV_STATE_CHECKED);
    if (name == this->robot.autonomous) {
      this->selected = btn;
      lv_obj_add_state(btn, LV_STATE_CHECKED);
    } else {
      lv_obj_clear_state(btn, LV_STATE_CHECKED);
    }
  }
}

void AutonomousSelect::update() {}

void AutonomousSelect::click(lv_event_t *event) {
  if (this->selected != nullptr) {
    lv_obj_clear_state(this->selected, LV_STATE_CHECKED);
  }
  this->selected = lv_event_get_target(event);

  auto name = std::string(lv_list_get_btn_text(this->list.get(), this->selected));

  if (this->robot.controller != nullptr) {
    this->robot.controller->setLine(0, name.c_str());
  }

  this->robot.autonomous = name;
  lv_obj_add_state(this->selected, LV_STATE_CHECKED);
}
} // namespace screen
