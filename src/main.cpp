#include "main.h"
#include "screen.hpp"
#include "definitions.hpp"

#define TIME_PER_INCH 1

void forwards(uint16_t distance, int32_t voltage);
void backwards(uint16_t distance, int32_t voltage);
void right(uint16_t distance, int32_t voltage);
void left(uint16_t distance, int32_t voltage);
void stop();
void move_for(uint16_t distance, int32_t rf, int32_t rb, int32_t lf, int32_t lb);
void setState(ControlState state);
lv_res_t on_force_autonomous_click(struct _lv_obj_t * obj);

static Motor motor_rf(10, true);
static Motor motor_rb(20, true);
static Motor motor_lf(1);
static Motor motor_lb(11);
static Motor arm(2, E_MOTOR_GEARSET_36);
static Motor arm2(9, E_MOTOR_GEARSET_36, true);

static Controller controller(E_CONTROLLER_MASTER);

static lv_obj_t* state_line = NULL;
static lv_obj_t* motor_rf_line = NULL;
static lv_obj_t* motor_rb_line = NULL;
static lv_obj_t* motor_lf_line = NULL;
static lv_obj_t* motor_lb_line = NULL;

static bool force_autonomous = false;
static ControlState controlState = INITIALIZE;

/**
 * Called when the "Force Autonomous" button is pressed.
 */
lv_res_t on_force_autonomous_click(struct _lv_obj_t * obj) {
	force_autonomous = true;
	lv_obj_set_click(obj, false);
	return LV_RES_OK;
}

/**
 * Updates the screen at a rate of 10fps.
 * Used for autoscroll because scrolling by hand doesn't work half the time.
 */
 void screen_update_task(void* param) {
	while (true) {
		update_screen();
		delay(100); //10 updates per second as we don't care *that* much about the screen.
	}
 }

/**
 * Updates the motor debug info.
 * Will print out errno values if an error occours.
 */
 void update_motor_info() {
	set_line(motor_rf_line, "Right Front: " + std::to_string(print_error<int32_t>(motor_rf.get_voltage())));
	set_line(motor_rb_line, "Right Back: "  + std::to_string(print_error<int32_t>(motor_rb.get_voltage())));
	set_line(motor_lf_line, "Left Front: "  + std::to_string(print_error<int32_t>(motor_lf.get_voltage())));
	set_line(motor_lb_line, "Left Back: "   + std::to_string(print_error<int32_t>(motor_lb.get_voltage())));
 }

/**
 * Called when the robot is first initialized.
 */
void initialize() {
	init_screen(on_force_autonomous_click);
	state_line = create_static_line("State: <uninitialized>");
	motor_rf_line = create_static_line("RF volatage: <uninitialized>");
	motor_rb_line = create_static_line("RB volatage: <uninitialized>");
	motor_lf_line = create_static_line("LF volatage: <uninitialized>");
	motor_lb_line = create_static_line("LB volatage: <uninitialized>");
    Task task(screen_update_task);

	setState(ControlState::INITIALIZE);
}


void disabled() {
	setState(ControlState::DISABLED);
}

void competition_initialize() {
	setState(ControlState::COMPETITION_INITIALIZE);
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 * Or when the "Force Autonomous" button is presssed on the screen.
 */
void autonomous() {
	setState(ControlState::AUTONOMOUS);
	
	forwards(2000, 32);
}

/**
 * Called when the robot is under driver control.
 * Will delegate to autonomous control if the "Force Autonomous" button is pressed.
 */
void opcontrol() {
	setState(ControlState::OPERATOR_CONTROL);

	while (true) {
		if (force_autonomous) {
			autonomous();
			force_autonomous = false;
			lv_obj_set_click(force_auto_button, true);

			setState(ControlState::OPERATOR_CONTROL);
		}

		int joystickL = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
		int joystickR = controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);
		if (controller.get_digital(E_CONTROLLER_DIGITAL_R1)) {
			arm.move(-127);
			arm2.move(-127);
		} else if (controller.get_digital(E_CONTROLLER_DIGITAL_R2)) {
			arm.move(127);
			arm2.move(127);
		} else {
			arm.move(0);
			arm2.move(0);
		}
		
		motor_rf.move(joystickR);
		motor_rb.move(joystickR);
		motor_lf.move(joystickL);
		motor_lb.move(joystickL);

		update_motor_info();

		delay(20);
	}
}

/**
 * Moves the robot forwards in inches.
 * @param distance The distance to move in inches.
 */
void forwards(uint16_t distance, int32_t voltage) {
	move_for(distance / TIME_PER_INCH, voltage, voltage, voltage, voltage);
}

/**
 * Moves the robot backwards in inches.
 * @param distance The distance to move in inches.
 */
void backwards(uint16_t distance, int32_t voltage) {
	move_for(distance / TIME_PER_INCH, -voltage, -voltage, -voltage, -voltage);
}

/**
 * Turns the robot right.
 * @param angle The distance to turn in degrees.
 */
void turn_right(uint16_t angle, int32_t voltage) {
	move_for(angle, -voltage, -voltage, voltage, voltage);
}

/**
 * Turns the robot left.
 * @param angle The distance to turn in degrees.
 */
void turn_left(uint16_t angle, int32_t voltage) {
	move_for(angle, -voltage, voltage, -voltage, -voltage);
}

/**
 * Stops the robot.
 */
void stop() {
	move_for(20, 0, 0, 0, 0);
}

/**
 * Powers the motors for a specific amount of time.
 * @param rf The power level for the motor on the front right.
 * @param rb The power level for the motor on the back right.
 * @param lf The power level for the motor on the front left.
 * @param lb The power level for the motor on the back left.
 */
void move_for(uint16_t time, int32_t rf, int32_t rb, int32_t lf, int32_t lb) {
	motor_rf = rf;
	motor_rb = rb;
	motor_lf = lf;
	motor_lb = lb;

	delay(time);

	motor_rf = 0;
	motor_rb = 0;
	motor_lf = 0;
	motor_lb = 0;
}

void setState(ControlState state) {
	controlState = state;
	set_line(state_line, "State: " + stateName(state));
}
