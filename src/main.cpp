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
lv_res_t on_button_click(struct _lv_obj_t * obj);

static Motor motor_rf(1);
static Motor motor_rb(11);
static Motor motor_lf(10);
static Motor motor_lb(20);
static Controller controller(E_CONTROLLER_MASTER);
static ControlState controlState = INITIALIZE;

static lv_obj_t* state_line = NULL;
static lv_obj_t* motor_rf_line = NULL;
static lv_obj_t* motor_rb_line = NULL;
static lv_obj_t* motor_lf_line = NULL;
static lv_obj_t* motor_lb_line = NULL;

static bool force_auto = false;

lv_res_t on_button_click(struct _lv_obj_t * obj) {
	force_auto = !force_auto;
	return LV_RES_OK;
}

 void concurrent_screen_update(void* param) {
	while (true) {
		update_screen();
		delay(100);
	}
 }


void initialize() {
	init_screen(on_button_click);
	state_line = create_static_line("State: <uninitialized>");
	motor_rf_line = create_static_line("RF volatage: <uninitialized>");
	motor_rb_line = create_static_line("RB volatage: <uninitialized>");
	motor_lf_line = create_static_line("LF volatage: <uninitialized>");
	motor_lb_line = create_static_line("LB volatage: <uninitialized>");
    Task task(concurrent_screen_update);

	setState(ControlState::INITIALIZE);
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
		if (force_auto) {
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

		set_line(motor_rf_line, "Right Front: " + std::to_string(print_error<int32_t>(motor_rf.get_voltage())));
		set_line(motor_rb_line, "Right Back: "  + std::to_string(print_error<int32_t>(motor_rb.get_voltage())));
		set_line(motor_lf_line, "Left Front: "  + std::to_string(print_error<int32_t>(motor_lf.get_voltage())));
		set_line(motor_lb_line, "Left Back: "   + std::to_string(print_error<int32_t>(motor_lb.get_voltage())));

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
	set_line(state_line, "State: " + stateName(state));
}
