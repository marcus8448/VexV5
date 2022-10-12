#include "screen/autonomous_select.hpp"
#include "robot/autonomous/autonomous.hpp"
#include "robot/robot.hpp"
#include "screen/screen.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_btn.h"
#include "display/lv_objx/lv_list.h"
#pragma GCC diagnostic pop

namespace screen {
extern std::vector<robot::autonomous::Autonomous *> *autonomousPrograms;
static AutonomousSelect *instance;

lv_res_t drop(lv_obj_t *obj);
lv_res_t click(lv_obj_t *obj);

AutonomousSelect::AutonomousSelect() = default;

void AutonomousSelect::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
  if (instance != nullptr) {
    return;
  }
  this->selections = lv_list_create(screen, nullptr);
  lv_obj_set_pos(this->selections, 0, 0);
  lv_obj_set_size(this->selections, width, height - BASE_HEIGHT);
  for (robot::autonomous::Autonomous *program : *autonomousPrograms) {
    lv_obj_t *btn = lv_list_add(this->selections, nullptr, program->name(), drop);
    lv_btn_set_toggle(btn, true);
    lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, ::screen::click);
  }
  instance = this;
}

void AutonomousSelect::initialize(lv_coord_t width, lv_coord_t height) {}

void AutonomousSelect::update(robot::Robot *robot) {}

void AutonomousSelect::destroy(lv_obj_t *screen) {}

void AutonomousSelect::click(lv_obj_t *btn) {
  if (this->selected == btn)
    return;
  if (this->selected != nullptr) {
    lv_btn_set_toggle(this->selected, false);
  }
  this->selected = btn;
  this->index = lv_list_get_btn_index(this->selections, btn);
  lv_btn_set_toggle(btn, true);

  robot::autonomous::set_active(index);
}

lv_res_t drop(lv_obj_t *obj) { return LV_RES_INV; }

lv_res_t click(lv_obj_t *obj) {
  instance->click(obj);
  return LV_RES_OK;
}

} // namespace screen