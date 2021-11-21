#include "main.h"
#include "screen.h"

#define TIME_PER_INCH 1

void forwards(uint16_t distance, int32_t voltage);
void backwards(uint16_t distance, int32_t voltage);
void right(uint16_t distance, int32_t voltage);
void left(uint16_t distance, int32_t voltage);
void stop();
void move_for(uint16_t distance, int32_t rf, int32_t rb, int32_t lf, int32_t lb);
void setState(ControlState state);

template <typename T> T printErr(T value);

static Motor motor_rf(1);
static Motor motor_rb(11);
static Motor motor_lf(10);
static Motor motor_lb(20);
static Controller controller(E_CONTROLLER_MASTER);
static ControlState controlState = INITIALIZE;

static bool force_auto = false;

void on_center_button() {
	force_auto = !force_auto;
	if (force_auto) {
		setState(ControlState::AUTONOMOUS);
	}	
}

void initialize() {
	init_screen();
	// lcd::initialize();
	setState(ControlState::INITIALIZE);
	// lcd::register_btn1_cb(on_center_button);
}

void disabled() {
	setState(ControlState::DISABLED);
}

void competition_initialize() {
	setState(ControlState::COMPETITION_INITIALIZE);

}

void autonomous() {
	setState(ControlState::AUTONOMOUS);
	
	forwards(2000, 32);
}

void opcontrol() {
	setState(ControlState::OPERATOR_CONTROL);

	while (true) {
		if (competition::is_disabled() && force_auto) {
			autonomous();
			setState(ControlState::OPERATOR_CONTROL);
		}

		int joystickRotX = controller.get_analog(ANALOG_RIGHT_X);
		int joystickY = controller.get_analog(ANALOG_LEFT_Y);
		int joystickX = controller.get_analog(ANALOG_LEFT_X);

		motor_rf.move(-joystickY + joystickRotX + joystickX);
		motor_rb.move(joystickY + joystickRotX + joystickX);
		motor_lf.move(-joystickY + joystickRotX - joystickX);
		motor_lb.move(joystickY + joystickRotX - joystickX);

		// lcd::set_text(1, "Front Right: " + std::to_string(printErr<int32_t>(motor_rf.get_voltage())));
		// lcd::set_text(2, "Front Left: "  + std::to_string(printErr<int32_t>(motor_rb.get_voltage())));
		// lcd::set_text(3, "Back Right: "  + std::to_string(printErr<int32_t>(motor_lf.get_voltage())));
		// lcd::set_text(4, "Back Left: "   + std::to_string(printErr<int32_t>(motor_lb.get_voltage())));

		delay(20);
	}
}

void forwards(uint16_t distance, int32_t voltage) {
	move_for(distance, voltage, voltage, voltage, voltage);
}

void backwards(uint16_t distance, int32_t voltage) {
	move_for(distance, -voltage, -voltage, -voltage, -voltage);
}

void right(uint16_t distance, int32_t voltage) {
	move_for(distance, voltage, voltage, -voltage, -voltage);
}

void left(uint16_t distance, int32_t voltage) {
	move_for(distance, -voltage, -voltage, voltage, voltage);
}

void stop() {
	move_for(20, 0, 0, 0, 0);
}

template <typename T> T printErr(T value) {
	if (value == PROS_ERR || value == PROS_ERR_F) {
		// lcd::set_text(7, std::string("Operation Failed: ") + std::string(strerror(errno)));
	}
	return value;
}

void move_for(uint16_t distance, int32_t rf, int32_t rb, int32_t lf, int32_t lb) {
	motor_rf = rf;
	motor_rb = rb;
	motor_lf = lf;
	motor_lb = lb;
	delay(distance / TIME_PER_INCH);

	motor_rf = 0;
	motor_rb = 0;
	motor_lf = 0;
	motor_lb = 0;
}

std::string stateName(ControlState state) {
	switch (state) {
		case INITIALIZE:
			return "Initialize";
		case COMPETITION_INITIALIZE:
			return "Competition Initialize";
		case AUTONOMOUS:
			return "Autonomous";
		case OPERATOR_CONTROL:
			return "Operator Control";
		case DISABLED:
			return "Disabled";
	}
	return "INVALID";
}

void setState(ControlState state) {
	controlState = state;
	// lcd::set_text(0, "State: " + stateName(state));
}

lv_res_t always_ok_event(struct _lv_obj_t * obj) {
    return LV_RES_OK;
}

void init_screen() {
	lv_obj_t * list = lv_list_create(lv_scr_act(), NULL);
	lv_cont_set_fit(list, true, true); 
	lv_obj_t * btn = lv_list_add(list, NULL, "Test", always_ok_event);
	lv_cont_set_fit(btn, true, true);

    // lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);
    // lv_obj_set_pos(btn, 10, 10);
    // lv_obj_set_size(btn, 120, 50);
    // lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, always_ok_event);

    // lv_obj_t * label = lv_label_create(btn, NULL);
    // lv_label_set_text(label, "Button");
    // lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
}