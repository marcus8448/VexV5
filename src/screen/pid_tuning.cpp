#include "screen/pid_tuning.hpp"

#include "debug/logger.hpp"
#include "main.hpp"
#include "tasks.hpp"

namespace screen {
static void increaseValue(lv_event_t *event);
static void decreaseValue(lv_event_t *event);

void PidTuning::update() {
  this->Kp.update();
  this->Ki.update();
  this->Kd.update();

  double error = this->pid.getError();

  if (this->testing) {
    if (this->prevError != std::numeric_limits<double>::infinity()) {
      if (this->prevError < 0 != error < 0) {
        lv_label_set_text_fmt(this->oscillationsLabel.get(), "Oscillations: %i", this->oscillations++);
      }

      if (this->oscillations == 1) {
        this->overshoot = std::max(this->overshoot, std::abs(error));
        lv_label_set_text_fmt(this->overshootLabel.get(), "Overshoot: %.4f", this->overshoot);
      }

      lv_label_set_text_fmt(this->changeLabel.get(), "Change: %.4f", error - this->prevError);
    }
  }

  lv_label_set_text_fmt(this->errorLabel.get(), "Error: %.4f", error);
  lv_label_set_text_fmt(this->headingLabel.get(),
                    "Heading: %.2f", this->robot.drivetrain.imu.getHeading());
  lv_label_set_text_fmt(this->leftPower.get(),
                    "PowerL: %.1f", this->robot.drivetrain.leftPID.output);
  lv_label_set_text_fmt(this->rightPower.get(), "PowerR: %.1f", this->pid.output);
  this->prevError = error;
}

PidTuning::PidTuning(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height,
                     robot::device::PID &pid, std::string_view runName)
    : Screen(robot, width, height), pid(pid),
      Kp(screen, 0, 0, coord((height - BUTTON_SIZE) / 3), coord(height - BUTTON_SIZE), 0.5, &pid.kp),
      Ki(screen, coord(width / 2 / 3), 0, coord((height - BUTTON_SIZE) / 3), coord(height - BUTTON_SIZE), 0.1, &pid.ki),
      Kd(screen, coord(width / 2 / 3 * 2), 0, coord((height - BUTTON_SIZE) / 3), coord(height - BUTTON_SIZE), 0.5,
         &pid.kd),
      testBtn(lv_btn_create(screen)), errorLabel(lv_label_create(screen)), changeLabel(lv_label_create(screen)),
      oscillationsLabel(lv_label_create(screen)), overshootLabel(lv_label_create(screen)),
      headingLabel(lv_label_create(screen)), leftPower(lv_label_create(screen)), rightPower(lv_label_create(screen)),
      runName(runName) {

  lv_obj_t *testLabel = lv_label_create(this->testBtn.get());
  lv_label_set_text_static(testLabel, "Test");
  lv_obj_set_style_text_font(testLabel, LV_THEME_DEFAULT_FONT_TITLE, LV_STATE_ANY);
  lv_obj_set_align(testLabel, LV_ALIGN_CENTER);

  lv_obj_add_event_cb(this->testBtn.get(), SCREEN_CB(PidTuning, startTest), LV_EVENT_CLICKED, this);
  lv_obj_set_pos(this->testBtn.get(), coord(width / 2), coord(height - BUTTON_SIZE - 80));
  lv_obj_set_size(this->testBtn.get(), coord(width / 2), 80);

  lv_obj_set_pos(this->errorLabel.get(), coord(width / 2), 0);
  lv_obj_set_pos(this->changeLabel.get(), coord(width / 2), 16);
  lv_obj_set_pos(this->oscillationsLabel.get(), coord(width / 2), 32);
  lv_obj_set_pos(this->overshootLabel.get(), coord(width / 2), 48);
  lv_obj_set_pos(this->headingLabel.get(), coord(width / 2), 64);
  lv_obj_set_pos(this->leftPower.get(), coord(width / 2), 80);
  lv_obj_set_pos(this->rightPower.get(), coord(width / 2), 96);

  lv_label_set_text_static(this->errorLabel.get(), "Error: ?");

  lv_label_set_text_static(this->changeLabel.get(), "Change: ?");
  lv_label_set_text_static(this->overshootLabel.get(), "Overshoot: ?");
  lv_label_set_text_static(this->oscillationsLabel.get(), "Oscillations: ?");
  lv_label_set_text_static(this->headingLabel.get(), "Heading: ?");
  lv_label_set_text_static(this->leftPower.get(), "PowerL: ?");
  lv_label_set_text_static(this->rightPower.get(), "PowerR: ?");
}

// PidTuning::~PidTuning() {
//   if (this->taskHandle != nullptr) {
//     this->testing = false;
//     pros::task_state_e_t state = pros::c::task_get_state(this->taskHandle);
//     if (state != pros::E_TASK_STATE_DELETED && state != pros::E_TASK_STATE_INVALID) {
//       rtos::killTask(this->taskHandle);
//     }
//
//     rtos::createTask(
//         "Opcontrol", [](void *param) { opcontrol(); }, nullptr);
//   }
// }

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
          self.prevError = std::numeric_limits<double>::infinity();
          std::string prevAuton = self.robot.autonomous;
          self.robot.autonomous = self.runName;
          logger::scope("PID test");
          self.testing = true;
          autonomous();
          self.testing = false;
          logger::endScope();
          self.robot.autonomous = prevAuton;
          self.taskHandle = nullptr;
          self.robot.drivetrain.brake();
        },
        this);
  } else {
    if (this->taskHandle != nullptr) {
      if (const pros::task_state_e_t state = pros::c::task_get_state(this->taskHandle);
          state != pros::E_TASK_STATE_DELETED && state != pros::E_TASK_STATE_INVALID) {
        logger::warn("Ending test early");
        rtos::killTask(this->taskHandle);
        this->robot.drivetrain.brake();
        rtos::createTask(
            "Opcontrol", [](void *param) { opcontrol(); }, nullptr);
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
  lv_obj_t *increaseBtnLabel = lv_label_create(this->increaseBtn.get());
  lv_obj_t *decreaseBtnLabel = lv_label_create(this->decreaseBtn.get());
  lv_obj_set_align(increaseBtnLabel, LV_ALIGN_CENTER);
  lv_obj_set_align(decreaseBtnLabel, LV_ALIGN_CENTER);

  auto splitHeight = coord(this->height / 3);

  lv_obj_set_pos(this->valueLabel.get(), this->x, coord(this->y + splitHeight));
  lv_obj_set_size(this->valueLabel.get(), this->width, splitHeight);

  lv_obj_set_pos(this->increaseBtn.get(), this->x, this->y);
  lv_obj_set_size(this->increaseBtn.get(), this->width, splitHeight);

  lv_obj_set_pos(this->decreaseBtn.get(), this->x, coord(this->y + splitHeight * 2));
  lv_obj_set_size(this->decreaseBtn.get(), this->width, splitHeight);

  lv_label_set_text_fmt(increaseBtnLabel, "+%.3f", this->delta);
  lv_label_set_text_fmt(decreaseBtnLabel, "-%.3f", this->delta);

  lv_obj_add_event_cb(this->increaseBtn.get(), increaseValue, LV_EVENT_CLICKED, this);
  lv_obj_add_event_cb(this->decreaseBtn.get(), decreaseValue, LV_EVENT_CLICKED, this);
}

void ControlGroup::update() {
  lv_label_set_text_fmt(this->valueLabel.get(), "%.3f", *this->value);
}

static void increaseValue(lv_event_t *event) {
  const auto &group = *static_cast<ControlGroup *>(event->user_data);
  *group.value += group.delta;
}

static void decreaseValue(lv_event_t *event) {
  const auto &group = *static_cast<ControlGroup *>(event->user_data);
  *group.value -= group.delta;
}
} // namespace screen
