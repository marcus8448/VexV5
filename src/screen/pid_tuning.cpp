#include "screen/pid_tuning.hpp"

#include "debug/logger.hpp"
#include "format.hpp"
#include "main.hpp"
#include "robot/robot.hpp"
#include "screen/screen.hpp"
#include "tasks.hpp"
#include <utility>

namespace screen {
static void increaseValue(lv_event_t *event);
static void decreaseValue(lv_event_t *event);

void PidTuning::update() {
  this->Kp.update();
  this->Ki.update();
  this->Kd.update();

  if (this->testing) {
    double error = this->pid.getError();
    if (this->prevError != INFINITY) {
      if (std::signbit(this->prevError) != std::signbit(error)) {
        this->oscillations++;
      }

      if (this->oscillations == 1) {
        this->overshoot = std::max(this->overshoot, std::abs(error));
      }

      lv_label_set_text(this->changeLabel, fmt::string_format("Change: %.4f", error - this->prevError).c_str());
    }
    lv_label_set_text(this->errorLabel, fmt::string_format("Error: %.4f", error).c_str());
    lv_label_set_text(this->overshootLabel, fmt::string_format("Overshoot: %.4f", this->overshootLabel).c_str());
    lv_label_set_text(this->oscillationsLabel, fmt::string_format("Oscillations: %i", this->oscillations).c_str());
    this->prevError = error;
  }
}

PidTuning::PidTuning(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height,
                     robot::device::PID &pid, std::string_view runName)
    : Screen(robot, width, height), pid(pid),
      Kp(screen, 0, 0, coord((height - BUTTON_SIZE) / 3), coord(height - BUTTON_SIZE), 1.0, &pid.kp),
      Ki(screen, coord(width / 2 / 3), 0, coord((height - BUTTON_SIZE) / 3), coord(height - BUTTON_SIZE), 1.0, &pid.ki),
      Kd(screen, coord(width / 2 / 3 * 2), 0, coord((height - BUTTON_SIZE) / 3), coord(height - BUTTON_SIZE), 1.0,
         &pid.kd),
      testBtn(lv_btn_create(screen)), errorLabel(lv_label_create(screen)), changeLabel(lv_label_create(screen)),
      oscillationsLabel(lv_label_create(screen)), overshootLabel(lv_label_create(screen)), runName(runName) {

  lv_obj_t *testLabel = lv_label_create(this->testBtn);
  lv_label_set_text(testLabel, "Test");
  lv_obj_set_style_text_font(testLabel, LV_THEME_DEFAULT_FONT_TITLE, LV_STATE_ANY);
  lv_obj_set_align(testLabel, LV_ALIGN_CENTER);

  lv_obj_add_event_cb(this->testBtn, SCREEN_CB(PidTuning, startTest), LV_EVENT_CLICKED, this);
  lv_obj_set_pos(this->testBtn, coord(width / 2), coord(height - BUTTON_SIZE - 80));
  lv_obj_set_size(this->testBtn, coord(width / 2), 80);

  lv_obj_set_pos(this->errorLabel, coord(width / 2), 0);
  lv_obj_set_pos(this->changeLabel, coord(width / 2), 16);
  lv_obj_set_pos(this->oscillationsLabel, coord(width / 2), 32);
  lv_obj_set_pos(this->overshootLabel, coord(width / 2), 48);

  lv_label_set_text(this->errorLabel, "Error: ?");

  lv_label_set_text(this->changeLabel, "Change: ?");
  lv_label_set_text(this->overshootLabel, "Overshoot: ?");
  lv_label_set_text(this->oscillationsLabel, "Oscillations: ?");
}

PidTuning::~PidTuning() {
  lv_obj_del_async(this->testBtn);
  lv_obj_del_async(this->errorLabel);
  lv_obj_del_async(this->changeLabel);
  lv_obj_del_async(this->overshootLabel);
  lv_obj_del_async(this->oscillationsLabel);

  if (this->taskHandle != nullptr) {
    this->testing = false;
    pros::task_state_e_t state = pros::c::task_get_state(this->taskHandle);
    if (state != pros::E_TASK_STATE_DELETED && state != pros::E_TASK_STATE_INVALID) {
      rtos::killTask(this->taskHandle);
    }

    rtos::createTask(
        "Opcontrol", [](void *param) { opcontrol(); }, nullptr);
  }
}

void PidTuning::startTest() {
  if (!this->testing) {
    rtos::killRootTask();
    logger::info("starting test");
    this->taskHandle = rtos::createTask(
        "PID Tuning",
        [](void *param) {
          logger::info("Task created");
          auto &self = *static_cast<PidTuning *>(param);
          self.overshoot = 0;
          self.oscillations = 0;
          self.prevError = INFINITY;
          std::string prevAuton = self.robot.autonomous;
          self.robot.autonomous = self.runName;
          logger::scope("PID test");
          self.testing = true;
          autonomous();
          self.testing = false;
          logger::endScope();
          self.robot.autonomous = prevAuton;
          self.taskHandle = nullptr;
        },
        this);
  } else {
    if (this->taskHandle != nullptr) {
      logger::warn("Ending test early");
      pros::task_state_e_t state = pros::c::task_get_state(this->taskHandle);
      if (state != pros::E_TASK_STATE_DELETED && state != pros::E_TASK_STATE_INVALID) {
        rtos::killTask(this->taskHandle);
      }
      this->taskHandle = nullptr;
    }
    this->testing = false;
  }
}

ControlGroup::ControlGroup(lv_obj_t *screen, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height,
                           double delta, double *value)
    : x(x), y(y), width(width), height(height), increaseBtn(lv_btn_create(screen)), valueLabel(lv_label_create(screen)),
      decreaseBtn(lv_btn_create(screen)), value(value), delta(std::abs(delta)) {
  lv_obj_t *increaseBtnLabel = lv_label_create(this->increaseBtn);
  lv_obj_t *decreaseBtnLabel = lv_label_create(this->decreaseBtn);
  lv_obj_set_align(increaseBtnLabel, LV_ALIGN_CENTER);
  lv_obj_set_align(decreaseBtnLabel, LV_ALIGN_CENTER);

  auto splitHeight = coord(this->height / 3);

  lv_obj_set_pos(this->valueLabel, this->x, coord(this->y + splitHeight));
  lv_obj_set_size(this->valueLabel, this->width, splitHeight);

  lv_obj_set_pos(this->increaseBtn, this->x, this->y);
  lv_obj_set_size(this->increaseBtn, this->width, splitHeight);

  lv_obj_set_pos(this->decreaseBtn, this->x, coord(this->y + splitHeight * 2));
  lv_obj_set_size(this->decreaseBtn, this->width, splitHeight);

  lv_label_set_text(increaseBtnLabel, fmt::string_format("+%.2f", this->delta).c_str());
  lv_label_set_text(decreaseBtnLabel, fmt::string_format("-%.2f", this->delta).c_str());

  lv_obj_add_event_cb(this->increaseBtn, increaseValue, LV_EVENT_CLICKED, this);
  lv_obj_add_event_cb(this->decreaseBtn, decreaseValue, LV_EVENT_CLICKED, this);
}

ControlGroup::~ControlGroup() {
  lv_obj_del_async(this->increaseBtn);
  lv_obj_del_async(this->valueLabel);
  lv_obj_del_async(this->decreaseBtn);
}

void ControlGroup::update() { lv_label_set_text(this->valueLabel, fmt::string_format("%.2f", *this->value).c_str()); }

static void increaseValue(lv_event_t *event) {
  auto &group = *static_cast<ControlGroup *>(event->user_data);
  *group.value += group.delta;
}

static void decreaseValue(lv_event_t *event) {
  auto &group = *static_cast<ControlGroup *>(event->user_data);
  *group.value -= group.delta;
}
} // namespace screen
