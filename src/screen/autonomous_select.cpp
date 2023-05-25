#include "screen/autonomous_select.hpp"
#include "control/autonomous/autonomous.hpp"
#include "robot/robot.hpp"
#include "screen/colour.hpp"
#include "screen/screen.hpp"
#include "screen/lvgl_util.hpp"

namespace screen {
AutonomousSelect *autonomous_select_instance = nullptr;
static lv_style_t *default_style = nullptr;
static lv_style_t *selected_style = nullptr;

lv_res_t drop([[maybe_unused]] lv_obj_t *obj);
lv_res_t click(lv_obj_t *btn);

AutonomousSelect::AutonomousSelect(robot::Robot &robot) : robot(robot) { autonomous_select_instance = this; }

void AutonomousSelect::initialize(lv_obj_t *screen) {
  this->selections = lv_list_create(screen, nullptr);
  lv_obj_set_pos(this->selections, 0, 0);
  lv_obj_set_size(this->selections, SCREEN_WIDTH, static_cast<lv_coord_t>(SCREEN_HEIGHT - BUTTON_SIZE));
  delete default_style;
  delete selected_style;
  default_style = new lv_style_t;
  selected_style = new lv_style_t;
  lv_style_copy(default_style, &lv_style_btn_rel);
  auto programs = control::autonomous::getPrograms();
  for (auto const &[name, program] : programs) {
    lv_obj_t *btn = lv_list_add(this->selections, nullptr, name.c_str(), drop);
    lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, ::screen::click);
    if (name == *control::autonomous::getActive()) {
      lv_style_copy(default_style, lv_obj_get_style(btn));
      lv_style_copy(selected_style, default_style);
      lv_obj_set_style(btn, selected_style);
      this->selected = btn;
    }
  }
  selected_style->text.color = colour::GREEN;
}

void AutonomousSelect::update() {}

void AutonomousSelect::click(lv_obj_t *btn) {
  if (this->selected != nullptr) {
    lv_obj_set_style(this->selected, default_style);
  }
  this->selected = btn;

  auto name = new std::string(lv_list_get_btn_text(btn));

  if (this->robot.controller != nullptr) {
    this->robot.controller->setLine(0, 0, name->c_str());
  }

  control::autonomous::set_active(name);
  lv_obj_set_style(btn, selected_style);
}

lv_res_t drop([[maybe_unused]] lv_obj_t *obj) { return LV_RES_INV; }

lv_res_t click(lv_obj_t *btn) {
  autonomous_select_instance->click(btn);
  return LV_RES_OK;
}

} // namespace screen
