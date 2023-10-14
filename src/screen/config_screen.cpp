#include "screen/config_screen.hpp"
#include "format.hpp"

namespace screen {
ConfigurationScreen::ConfigurationScreen(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height)
    : Screen(robot, width, height), driveSchemeBtn(lv_btn_create(screen)),
      driveSchemeBtnLbl(lv_label_create(this->driveSchemeBtn.get())) {
  lv_obj_set_pos(this->driveSchemeBtn.get(), 0, 0);
  lv_obj_set_size(this->driveSchemeBtn.get(), width / 2, 48);
  lv_obj_add_event_cb(this->driveSchemeBtn.get(), SCREEN_CB(ConfigurationScreen, click), LV_EVENT_CLICKED, this);

  lv_label_set_text(
      this->driveSchemeBtnLbl,
      fmt::string_format("Control Scheme: %s", robot::driveSchemeName(this->robot.drivetrain.controlScheme)).c_str());
}

void ConfigurationScreen::update() {}

void ConfigurationScreen::click() {
  this->robot.drivetrain.controlScheme = this->robot.drivetrain.controlScheme == robot::Drivetrain::ControlScheme::TANK
                                             ? robot::Drivetrain::ControlScheme::ARCADE
                                             : robot::Drivetrain::ControlScheme::TANK;
  lv_label_set_text(
      this->driveSchemeBtnLbl,
      fmt::string_format("Control Scheme: %s", robot::driveSchemeName(this->robot.drivetrain.controlScheme)).c_str());
}
} // namespace screen
