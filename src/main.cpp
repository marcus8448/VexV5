#include "main.h"

#define TIME_PER_INCH 1

void forwards(uint16_t distance, int32_t voltage);
void backwards(uint16_t distance, int32_t voltage);
void right(uint16_t distance, int32_t voltage);
void left(uint16_t distance, int32_t voltage);
void stop();
void move_for(uint16_t distance, int32_t rf, int32_t rb, int32_t lf, int32_t lb);

template <typename T> T printErr(T value);

static Motor motor_rf(1);
static Motor motor_rb(11);
static Motor motor_lf(10);
static Motor motor_lb(20);
static Controller controller(E_CONTROLLER_MASTER);

static bool force_auto = false;

void on_center_button() {
	force_auto = !force_auto;
	if (force_auto) {
		lcd::set_text(1, "Autonomous");
	} else {
		lcd::set_text(1, "Default");
	}
}

void initialize() {
	lcd::initialize();
	lcd::set_text(1, "???");


	lcd::register_btn1_cb(on_center_button);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	lcd::set_text(1, "Autonomous");
	
	forwards(2000, 32);
}

void opcontrol() {
	lcd::set_text(1, "Opcontrol");

	while (true) {
		if (competition::is_disabled() && force_auto) {
			autonomous();
		}

		int joystickRotX = controller.get_analog(ANALOG_RIGHT_X);
		int joystickY = controller.get_analog(ANALOG_LEFT_Y);
		int joystickX = controller.get_analog(ANALOG_LEFT_X);

		motor_rf = -joystickY + joystickRotX + joystickX;
		motor_rb = joystickY + joystickRotX + joystickX;
		motor_lf = -joystickY + joystickRotX - joystickX;
		motor_lb = joystickY + joystickRotX - joystickX;

		lcd::set_text(1, "Front Right: " + std::to_string(printErr<int32_t>(motor_rf.get_voltage())));
		lcd::set_text(2, "Front Left: "  + std::to_string(printErr<int32_t>(motor_rb.get_voltage())));
		lcd::set_text(3, "Back Right: "  + std::to_string(printErr<int32_t>(motor_lf.get_voltage())));
		lcd::set_text(4, "Back Left: "   + std::to_string(printErr<int32_t>(motor_lb.get_voltage())));

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
		lcd::set_text(7, std::string("Operation Failed: ") + std::string(strerror(errno)));
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
