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
static void testRobot(lv_event_t *event);

PidTuning::PidTuning(robot::Robot &robot, robot::device::PID &pid, std::string runName)
    : pid(pid), runName(std::move(runName)), robot(robot) {}

void PidTuning::initialize(lv_obj_t *screen) {
  this->Kp =
      new ControlGroup(screen, 0, 0, (SCREEN_HEIGHT - BUTTON_SIZE) / 3, SCREEN_HEIGHT - BUTTON_SIZE, 1.0, &pid.kp);
  this->Ki = new ControlGroup(screen, (SCREEN_WIDTH / 2 / 3), 0, (SCREEN_HEIGHT - BUTTON_SIZE) / 3,
                              SCREEN_HEIGHT - BUTTON_SIZE, 1.0, &pid.ki);
  this->Kd = new ControlGroup(screen, SCREEN_WIDTH / 2 / 3 * 2, 0, (SCREEN_HEIGHT - BUTTON_SIZE) / 3,
                              SCREEN_HEIGHT - BUTTON_SIZE, 1.0, &pid.kd);

  this->testBtn = lv_btn_create(screen);

  lv_obj_t *testLabel = lv_label_create(this->testBtn);
  lv_label_set_text(testLabel, "Test");
  lv_obj_set_style_text_font(testLabel, LV_THEME_DEFAULT_FONT_TITLE, LV_STATE_ANY);
  lv_obj_set_align(testLabel, LV_ALIGN_CENTER);

  lv_obj_add_event_cb(this->testBtn, testRobot, LV_EVENT_CLICKED, this);
  lv_obj_set_pos(this->testBtn, SCREEN_WIDTH / 2, SCREEN_HEIGHT - BUTTON_SIZE - 80);
  lv_obj_set_size(this->testBtn, SCREEN_WIDTH / 2, 80);

  this->errorLabel = lv_label_create(screen);
  this->changeLabel = lv_label_create(screen);
  this->oscillationsLabel = lv_label_create(screen);
  this->overshootLabel = lv_label_create(screen);

  lv_obj_set_pos(this->errorLabel, SCREEN_WIDTH / 2, 0);
  lv_obj_set_pos(this->changeLabel, SCREEN_WIDTH / 2, 16);
  lv_obj_set_pos(this->oscillationsLabel, SCREEN_WIDTH / 2, 32);
  lv_obj_set_pos(this->overshootLabel, SCREEN_WIDTH / 2, 48);

  lv_label_set_text(this->errorLabel, "Error: ?");
  lv_label_set_text(this->changeLabel, "Change: ?");
  lv_label_set_text(this->overshootLabel, "Overshoot: ?");
  lv_label_set_text(this->oscillationsLabel, "Oscillations: ?");
}

void PidTuning::update() {
  this->Kp->update();
  this->Ki->update();
  this->Kd->update();

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

void PidTuning::cleanup() {
  delete this->Kp;
  delete this->Ki;
  delete this->Kd;

  this->Kp = nullptr;
  this->Ki = nullptr;
  this->Kd = nullptr;

  lv_obj_del_async(this->testBtn);
  lv_obj_del_async(this->errorLabel);
  lv_obj_del_async(this->changeLabel);
  lv_obj_del_async(this->overshootLabel);
  lv_obj_del_async(this->oscillationsLabel);

  this->testBtn = nullptr;
  this->errorLabel = nullptr;
  this->changeLabel = nullptr;
  this->overshootLabel = nullptr;
  this->oscillationsLabel = nullptr;

  if (this->taskHandle != nullptr) {
    this->testing = false;
    pros::task_state_e_t state = pros::c::task_get_state(this->taskHandle);
    if (state != pros::E_TASK_STATE_DELETED && state != pros::E_TASK_STATE_INVALID) {
      rtos::killTask(this->taskHandle);
    }
    this->taskHandle = nullptr;

    rtos::createTask(
        "Opcontrol", [](void *param) { opcontrol(); }, nullptr);
  }
}

ControlGroup::ControlGroup(lv_obj_t *screen, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height,
                           double delta, double *value)
    : x(x), y(y), width(width), height(height), value(value), delta(std::abs(delta)) {
  this->valueLabel = lv_label_create(screen);
  this->increaseBtn = lv_btn_create(screen);
  this->decreaseBtn = lv_btn_create(screen);

  lv_obj_t *increaseBtnLabel = lv_label_create(this->increaseBtn);
  lv_obj_t *decreaseBtnLabel = lv_label_create(this->decreaseBtn);
  lv_obj_set_align(increaseBtnLabel, LV_ALIGN_CENTER);
  lv_obj_set_align(decreaseBtnLabel, LV_ALIGN_CENTER);

  lv_coord_t splitHeight = this->height / 3;

  lv_obj_set_pos(this->valueLabel, this->x, this->y + splitHeight);
  lv_obj_set_size(this->valueLabel, this->width, splitHeight);

  lv_obj_set_pos(this->increaseBtn, this->x, this->y);
  lv_obj_set_size(this->increaseBtn, this->width, splitHeight);

  lv_obj_set_pos(this->decreaseBtn, this->x, this->y + splitHeight * 2);
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

  this->increaseBtn = nullptr;
  this->valueLabel = nullptr;
  this->decreaseBtn = nullptr;
}

void ControlGroup::update() { lv_label_set_text(this->valueLabel, fmt::string_format("%.2f", *this->value).c_str()); }

static void increaseValue(lv_event_t *event) {
  auto group = static_cast<ControlGroup *>(event->user_data);
  *group->value += group->delta;
}

static void decreaseValue(lv_event_t *event) {
  auto group = static_cast<ControlGroup *>(event->user_data);
  *group->value -= group->delta;
}

static void testRobot(lv_event_t *event) {
  auto inst = static_cast<PidTuning *>(event->user_data);
  if (!inst->testing) {
    info("starting test");
    rtos::killRootTask();
    info("starting test!");
    inst->taskHandle = rtos::createTask(
        "PID Tuning",
        [](void *param) {
          info("Task created");
          auto inst = static_cast<PidTuning *>(param);
          inst->overshoot = 0;
          inst->oscillations = 0;
          inst->prevError = INFINITY;
          std::string prevAuton = inst->robot.autonomous;
          inst->robot.autonomous = inst->runName;
          scopePush("PID test");
          inst->testing = true;
          autonomous();
          inst->testing = false;
          scopePop();
          inst->robot.autonomous = prevAuton;
          inst->taskHandle = nullptr;
        },
        inst);
  } else {
    if (inst->taskHandle != nullptr) {
      info("kill task");
      pros::task_state_e_t state = pros::c::task_get_state(inst->taskHandle);
      if (state != pros::E_TASK_STATE_DELETED && state != pros::E_TASK_STATE_INVALID) {
        rtos::killTask(inst->taskHandle);
      }
      inst->taskHandle = nullptr;
    }
    inst->testing = false;
  }
}
} // namespace screen
