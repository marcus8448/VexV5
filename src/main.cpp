#include <cfloat>
#include <string>
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "vexv5/debug.hpp"
#include "vexv5/globals.hpp"
#include "vexv5/opcontrol.hpp"
#include "vexv5/robot.hpp"
#include "vexv5/switches.hpp"

extern "C" {
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
}

/**
 * Called when the robot is first initialized.
 */
void initialize() {
    Task::create(debug_input_task, "Debug Input Task");
    // Task::create(controller_update_task, "Controller Update Task");

    motor_rf.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    motor_rb.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    motor_lf.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    motor_lb.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    lift.set_zero_position(0);
    arm_1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    arm_2.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    arm_hook.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    arm_1.set_zero_position(0);
    arm_2.set_zero_position(0);
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 * Or when the "Force Autonomous" button is presssed on the screen.
 */
void autonomous() {
    select_autonomous();
}

/**
 * Called when the robot is under driver control.
 * Will delegate to autonomous control if the "Force Autonomous" button is pressed.
 */
void opcontrol() {
    if (call_reset_positions()) {
        return;
    }

    std::ofstream* outf = create_record_stream();
    unsigned int digital_speed = 127;

    while (true) {
        drive(erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)),
            erri(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)),
            erri(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X)),
            erri(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)),
            erri(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)),
            erri(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)),
            &digital_speed
        );

        call_serialize_controller_state(outf, controller);
        pros::delay(20);
    }
}
