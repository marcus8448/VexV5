#include "main.h"

#define TIME_PER_INCH 1

void forwards(uint16_t distance, int32_t voltage);
void backwards(uint16_t distance, int32_t voltage);
void right(uint16_t distance, int32_t voltage);
void left(uint16_t distance, int32_t voltage);
void stop();
void move_for(uint16_t distance, int32_t fr, int32_t fl, int32_t br, int32_t bl);

static Motor motor_fr(1);
static Motor motor_fl(11);
static Motor motor_br(10);
static Motor motor_bl(20);
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

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	lcd::initialize();
	lcd::set_text(1, "???");


	lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	lcd::set_text(1, "Autonomous");
	
	forwards(2000, 32);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	lcd::set_text(1, "Opcontrol");

	while (true) {
		if (force_auto) {
			autonomous();
		}

		int joystickRotX = controller.get_analog(ANALOG_RIGHT_X);
		int joystickY = controller.get_analog(ANALOG_LEFT_Y);
		int joystickX = controller.get_analog(ANALOG_LEFT_X);

		motor_fr = -joystickY + joystickRotX + joystickX;
		motor_fl = joystickY + joystickRotX + joystickX;
		motor_br = -joystickY + joystickRotX - joystickX;
		motor_bl = joystickY + joystickRotX - joystickX;

		lcd::set_text(1, "Front Right: " + std::to_string(motor_fr.get_voltage()));
		lcd::set_text(2, "Front Left: " + std::to_string(motor_fl.get_voltage()));
		lcd::set_text(3, "Back Right: " + std::to_string(motor_br.get_voltage()));
		lcd::set_text(4, "Back Left: " + std::to_string(motor_bl.get_voltage()));

		delay(20);
	}
}

void forwards(uint16_t distance, int32_t voltage) {
	move_for(distance, -voltage, voltage, -voltage, voltage);
}

void backwards(uint16_t distance, int32_t voltage) {
	move_for(distance, voltage, -voltage, voltage, -voltage);
}

void right(uint16_t distance, int32_t voltage) {
	move_for(distance, voltage, voltage, -voltage, -voltage);
}

void left(uint16_t distance, int32_t voltage) {
	move_for(distance, -voltage, -voltage, voltage, voltage);
}

void stop() {
	move_for(20, 0, 0, 0 ,0);
}

void move_for(uint16_t distance, int32_t fr, int32_t fl, int32_t br, int32_t bl) {
	motor_fr = fr;
	motor_fl = fl;
	motor_br = br;
	motor_bl = bl;
	delay(distance / TIME_PER_INCH);

	motor_fr = 0;
	motor_fl = 0;
	motor_br = 0;
	motor_bl = 0;
}
