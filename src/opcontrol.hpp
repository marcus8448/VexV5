#ifndef _OPCONTROL_HPP_
#define _OPCONTROL_HPP_

#include "robot.hpp"
#include <cfloat>

void drive(bool a, bool b, bool x, bool y, bool up, bool down, bool left, bool right, bool l1, bool l2, bool r1, bool r2, double lx, double ly, double rx, double ry, unsigned int* digital_speed, unsigned int* prev_digital_speed) {
    static int cooldown = 0;
    static double lift_lock_pos = DBL_MAX;
    static double arm_lock_pos_1 = DBL_MAX;
    static double arm_lock_pos_2 = DBL_MAX;
    static double arm_hook_lock_pos = DBL_MAX;

    if (cooldown > 0) {
        cooldown--;
        if (cooldown == 0) {
            controller.clear_line(1);
        }
    }
    if (r1 && r2) {
        lift.move_absolute(lift.get_position(), *digital_speed);
        lift_lock_pos = lift.get_position();
        cooldown = 30;
        controller.set_text(1, 0, "Locked");
    } else {
        if (cooldown == 0) {
            if (r1) {
                if (lift.get_position() >= LIFT_UP_POSITION) {
                    if (lift_lock_pos == DBL_MAX) {
                        lift_lock_pos = lift.get_position();
                    }
                } else {
                    p_err(lift.move(*digital_speed)); // UP
                    lift_lock_pos = DBL_MAX;
                }
            } else if (r2) {
                if (lift.get_position() <= LIFT_DOWN_POSITION) {
                    if (lift_lock_pos == DBL_MAX) {
                        lift_lock_pos = lift.get_position();
                    }
                } else {
                    p_err(lift.move(-*digital_speed)); // DOWN
                    lift_lock_pos = DBL_MAX;
                }
            } else {
                if (lift_lock_pos == DBL_MAX) {
                    p_err(lift.move(0)); // STOP
                }
            }
        }
    }
    if (l1 && l2) {
        move_arm_absolute(arm_1.get_position(), arm_2.get_position(), digital_speed);
        arm_lock_pos_1 = arm_1.get_position();
        arm_lock_pos_2 = arm_2.get_position();
        cooldown = 30;
    } else {
        if (l1) {
            if (arm_position() >= ARM_UP_POSITION) {
                if (arm_lock_pos_1 == DBL_MAX) {
                    arm_lock_pos_1 = arm_1.get_position();
                }
                if (arm_lock_pos_2 == DBL_MAX) {
                    arm_lock_pos_2 = arm_2.get_position();
                }
            } else {
                move_arm(*digital_speed); // UP
                arm_lock_pos_1 = DBL_MAX;
                arm_lock_pos_2 = DBL_MAX;
            }
        
        } else if (l2) {
            if (arm_position() <= ARM_DOWN_POSITION) {
                if (arm_lock_pos_1 == DBL_MAX) {
                    arm_lock_pos_1 = arm_1.get_position();
                }
                if (arm_lock_pos_2 == DBL_MAX) {
                    arm_lock_pos_2 = arm_2.get_position();
                }
            } else {
                move_arm(*digital_speed / -1.4); // DOWN
                arm_lock_pos_1 = DBL_MAX;
                arm_lock_pos_2 = DBL_MAX;
            }
        } else {
            if (arm_lock_pos_1 == DBL_MAX) {
                move_arm(0); // STOP
            }
        }
    }

    if (cooldown == 0) {
        if (left && right) {
            p_err(arm_hook.move_relative(0, *digital_speed));
            arm_hook_lock_pos = p_err(arm_hook.get_target_position());
            cooldown = 30;
            controller.set_text(1, 0, "Locked");
        } else if (cooldown == 0) { 
            if (left) {
                p_err(arm_hook.move(*digital_speed / 1.5)); // OPEN
                arm_hook_lock_pos = DBL_MAX;
            } else if (right) {
                p_err(arm_hook.move(*digital_speed / -1.5)); // SHUT
                arm_hook_lock_pos = DBL_MAX;
            } else if (arm_hook_lock_pos == DBL_MAX) {
                p_err(arm_hook.move(0)); // STOP
            }
        }
    }

    if (up) {
        if (*digital_speed + 1 <= 127) {
            *digital_speed += 1;
        }
    } else if (down) {
        if (*digital_speed - 1 >= 10) {
            *digital_speed -= 1;
        }
    }

    if (cooldown == 0) {
        if (a) {
            arm_up(*digital_speed, false);
            arm_lock_pos_1 = arm_1.get_target_position();
            arm_lock_pos_2 = arm_2.get_target_position();
        } else if (y) {
            arm_prime(*digital_speed, false);
            arm_lock_pos_1 = arm_1.get_target_position();
            arm_lock_pos_2 = arm_2.get_target_position();
        }
    }

    if (x && b) {
        lift_lift(*digital_speed, false);
        lift_lock_pos = lift.get_target_position();
        cooldown = 30;
        controller.set_text(1, 0, "Locked");
    } else if (cooldown == 0) {
        if (x) {
            lift_up(*digital_speed, false);
            lift_lock_pos = lift.get_target_position();
        } else if (b) {
            lift_down(*digital_speed, false);
            lift_lock_pos = lift.get_target_position();
        }
    }

    move_right_motors(p_err(ry));
    move_left_motors(p_err(ly));

    if (arm_lock_pos_1 != DBL_MAX && arm_1.get_target_position() != arm_lock_pos_1) {
        arm_1.move_absolute(arm_lock_pos_1, *digital_speed / 3.0);
    }
    if (arm_lock_pos_2 != DBL_MAX && arm_2.get_target_position() != arm_lock_pos_2) {
        arm_2.move_absolute(arm_lock_pos_2, *digital_speed / 2.0);
    }
    if (lift_lock_pos != DBL_MAX && lift.get_target_position() != lift_lock_pos) {
        lift.move_absolute(lift_lock_pos, *digital_speed / 3.0);
    }

    if (*digital_speed != *prev_digital_speed) {
        *prev_digital_speed = *digital_speed;
        controller.set_text(0, 0, "Digital Speed: " + std::to_string((unsigned int)*digital_speed).append(" "));
    }
}

#endif