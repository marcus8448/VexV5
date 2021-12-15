#include "main.h"
#include "screen.hpp"
#include "definitions.hpp"
#include "robot.hpp"

void setState(ControlState state);
lv_res_t on_force_autonomous_click(struct _lv_obj_t * obj);

static lv_obj_t* state_line = NULL;
static lv_obj_t* motor_rf_line = NULL;
static lv_obj_t* motor_rb_line = NULL;
static lv_obj_t* motor_lf_line = NULL;
static lv_obj_t* motor_lb_line = NULL;

static bool force_autonomous = false;
static ControlState controlState = INITIALIZE;

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
	
	motor_rf.set_brake_mode(MOTOR_BRAKE_BRAKE);
	motor_rb.set_brake_mode(MOTOR_BRAKE_BRAKE);
	motor_lf.set_brake_mode(MOTOR_BRAKE_BRAKE);
	motor_lb.set_brake_mode(MOTOR_BRAKE_BRAKE);
	arm.set_brake_mode(MOTOR_BRAKE_BRAKE);
	arm2.set_brake_mode(MOTOR_BRAKE_BRAKE);

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
	
	// forwards(24, 100);
	// delay(5000);
	// backwards(24, 100);
	turn_right(90, 30);
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
 * Called when the "Force Autonomous" button is pressed.
 */
lv_res_t on_force_autonomous_click(struct _lv_obj_t * obj) {
	force_autonomous = true;
	return LV_RES_OK;
}

/**
 * Sets the state of the robot for display.
 * (Remove?)
 */
void setState(ControlState state) {
	controlState = state;
	set_line(state_line, "State: " + stateName(state));
	print_out("State: " + stateName(state));
}
