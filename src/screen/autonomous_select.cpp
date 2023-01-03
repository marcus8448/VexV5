#include "screen/autonomous_select.hpp"
#include "robot/autonomous/autonomous.hpp"
#include "robot/robot.hpp"
#include "screen/colour.hpp"
#include "screen/screen.hpp"
#include <map>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#include "display/lv_core/lv_style.h"
#include "display/lv_objx/lv_btn.h"
#include "display/lv_objx/lv_list.h"
#pragma GCC diagnostic pop

extern std::map<const std::string, robot::autonomous::Autonomous *> *autonomousPrograms;
namespace screen {
AutonomousSelect *autonomous_select_instance = nullptr;
static lv_style_t *default_style = nullptr;
static lv_style_t *selected_style = nullptr;

lv_res_t drop([[maybe_unused]] lv_obj_t *obj);
lv_res_t click(lv_obj_t *btn);

AutonomousSelect::AutonomousSelect() { autonomous_select_instance = this; }

void AutonomousSelect::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
  this->selections = lv_list_create(screen, nullptr);
  lv_obj_set_pos(this->selections, 0, 0);
  lv_obj_set_size(this->selections, width, static_cast<lv_coord_t>(height - BASE_HEIGHT));
  free(default_style);
  free(selected_style);
  default_style = new lv_style_t;
  selected_style = new lv_style_t;
  lv_style_copy(default_style, &lv_style_btn_rel);
  for (auto const &[name, program] : *autonomousPrograms) {
    lv_obj_t *btn = lv_list_add(this->selections, nullptr, name.c_str(), drop);
    lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, ::screen::click);
    if (name == *robot::autonomous::get_active()) {
      lv_style_copy(default_style, lv_obj_get_style(btn));
      lv_style_copy(selected_style, default_style);
      lv_obj_set_style(btn, selected_style);
      this->selected = btn;
    }
  }
  selected_style->text.color = colour::GREEN;
}

static robot::Robot *robot_maybe = nullptr;

void AutonomousSelect::update(robot::Robot &robot) {
  robot_maybe = &robot;
}

void AutonomousSelect::click(lv_obj_t *btn) {
  if (this->selected != nullptr) {
    lv_obj_set_style(this->selected, default_style);
  }
  this->selected = btn;

  auto name = new std::string(lv_list_get_btn_text(btn));

  if (robot_maybe != nullptr) {
    if (robot_maybe->controller != nullptr) {
      robot_maybe->controller->set_line(0, 0, name->c_str());
    }
  }

  robot::autonomous::set_active(name);
  lv_obj_set_style(btn, selected_style);
}

lv_res_t drop([[maybe_unused]] lv_obj_t *obj) { return LV_RES_INV; }

lv_res_t click(lv_obj_t *btn) {
  autonomous_select_instance->click(btn);
  return LV_RES_OK;
}

} // namespace screen
