#include "main.h"
#include "definitions.hpp"
#include "robot.hpp"
#include "screen.hpp"

void setState(ControlState state);
lv_res_t on_force_autonomous_click(struct _lv_obj_t* obj);

static bool force_autonomous = false;

/**
 * Updates the motor debug info.
 * Will print out errno values if an error occours.
 */
void update_motor_info() {
    static lv_obj_t* motor_rf_line = create_static_line("RFV: <uninitialized>");
    static lv_obj_t* motor_rb_line = create_static_line("RBV: <uninitialized>");
    static lv_obj_t* motor_lf_line = create_static_line("LFV: <uninitialized>");
    static lv_obj_t* motor_lb_line = create_static_line("LBV: <uninitialized>");
    static lv_obj_t* arm_line = create_static_line("AP: <uninitialized>");
    static lv_obj_t* arm2_line = create_static_line("A2P: <uninitialized>");

    set_line(motor_rf_line, "RFV: " + get_errno(motor_rf.get_actual_velocity()));
    set_line(motor_rb_line, "RBV: " + get_errno(motor_rb.get_actual_velocity()));
    set_line(motor_lf_line, "LFV: " + get_errno(motor_lf.get_actual_velocity()));
    set_line(motor_lb_line, "LBV: " + get_errno(motor_lb.get_actual_velocity()));
    set_line(arm_line, "AP: " + get_errno(arm.get_actual_velocity()));
    set_line(arm2_line, "A2P: " + get_errno(arm2.get_actual_velocity()));
}

/**
 * Updates the screen at a rate of 10fps.
 * Used for autoscroll because scrolling by hand doesn't work half the time.
 */
[[noreturn]] void screen_update_task() {
    while (true) {
        update_screen();
        update_motor_info();
        delay(100);// 10 updates per second as we don't care *that* much about the screen.
    }
}

/**
 * Called when the robot is first initialized.
 */
void initialize() {
    init_screen(on_force_autonomous_click);
    Task::create(screen_update_task, TASK_PRIORITY_DEFAULT - 2, TASK_STACK_DEPTH_DEFAULT, "Screen Update");

    motor_rf.set_brake_mode(MOTOR_BRAKE_BRAKE);
    motor_rb.set_brake_mode(MOTOR_BRAKE_BRAKE);
    motor_lf.set_brake_mode(MOTOR_BRAKE_BRAKE);
    motor_lb.set_brake_mode(MOTOR_BRAKE_BRAKE);
    arm.set_brake_mode(MOTOR_BRAKE_BRAKE);
    arm2.set_brake_mode(MOTOR_BRAKE_BRAKE);
    arm.set_zero_position(0);
    arm2.set_zero_position(0);

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

    for (int i = 0; i < 3; i++) {
        arm_down();
        forwards(5 * IN_TO_FT, 100);
        arm_lift(100, true);
        backwards(5 * IN_TO_FT, 100);
        arm_up(100, true);
    }
}

/**
 * Called when the robot is under driver control.
 * Will delegate to autonomous control if the "Force Autonomous" button is pressed.
 */
[[noreturn]] void opcontrol() {
    setState(ControlState::OPERATOR_CONTROL);

    while (true) {
        if (force_autonomous) {
            autonomous();
            force_autonomous = false;

            setState(ControlState::OPERATOR_CONTROL);
        }

        int joystickL = p_err(controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
        int joystickR = p_err(controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y));
        if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_R1)) && p_err(arm.get_position()) > 0.0) {
            p_err(arm.move(100)); // UP
            p_err(arm2.move(100));
        } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_R2)) && p_err(arm.get_position()) < 130.0) {
            p_err(arm.move(-100)); // DOWN
            p_err(arm2.move(-100));
        } else {
            p_err(arm.move(0)); // STOP
            p_err(arm2.move(0));
        }
        print(arm.get_position());

        p_err(motor_rf.move(joystickR));
        p_err(motor_rb.move(joystickR));
        p_err(motor_lf.move(joystickL));
        p_err(motor_lb.move(joystickL));

        delay(20);
    }
}

/**
 * Called when the "Force Autonomous" button is pressed.
 */
lv_res_t on_force_autonomous_click(struct _lv_obj_t* obj) {
    force_autonomous = true;
    return LV_RES_OK;
}

/**
 * Sets the state of the robot for display.
 * (Remove?)
 */
void setState(ControlState state) {
    static lv_obj_t* state_line = create_static_line("State: <uninitialized>");
    set_line(state_line, "State: " + stateName(state));
    print("State: " + stateName(state));
}
