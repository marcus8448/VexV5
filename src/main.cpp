#include "main.h"
#include "definitions.hpp"
#include "robot.hpp"
#include "screen.hpp"

lv_res_t on_force_autonomous_click(struct _lv_obj_t* obj);

static bool force_autonomous = false;

/**
 * Called when the robot is first initialized.
 */
void initialize() {
    init_screen(on_force_autonomous_click);

    motor_rf.set_brake_mode(MOTOR_BRAKE_BRAKE);
    motor_rb.set_brake_mode(MOTOR_BRAKE_BRAKE);
    motor_lf.set_brake_mode(MOTOR_BRAKE_BRAKE);
    motor_lb.set_brake_mode(MOTOR_BRAKE_BRAKE);
    arm.set_brake_mode(MOTOR_BRAKE_BRAKE);
    arm.set_zero_position(0);
}

void disabled() {
}

void competition_initialize() {
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 * Or when the "Force Autonomous" button is presssed on the screen.
 */
void autonomous() {
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
    while (true) {
        if (force_autonomous) {
            autonomous();
            force_autonomous = false;
        }

        int joystickL = p_err(controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
        int joystickR = p_err(controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y));
        if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_R1)) && p_err(arm.get_position()) > 0.0) {
            p_err(arm.move(100)); // UP
        } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_R2)) && p_err(arm.get_position()) < 130.0) {
            p_err(arm.move(-100)); // DOWN
        } else {
            p_err(arm.move(0)); // STOP
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
