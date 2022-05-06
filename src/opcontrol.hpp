#ifndef _OPCONTROL_HPP_
#define _OPCONTROL_HPP_

#include "robot.hpp"
#include <cfloat>

void move_lift(bool r1, bool r2, bool x, bool b, unsigned int digital_speed) {
    static double lift_lock_pos = DBL_MAX;
    static bool controller_locked = false;
    
    if (r1 && r2) {
        lift_lock_pos = lift.get_position();
        controller_locked = true;
        controller.set_text(1, 0, "Locked");
    } else {
        if (controller_locked && !r1 && !r2) {
            controller_locked = false;
        }

        if (!controller_locked) {
            if (r1) {
                if (lift.get_position() >= LIFT_UP_POSITION) {
                    if (lift_lock_pos == DBL_MAX) {
                        lift_lock_pos = lift.get_position();
                    }
                } else {
                    p_err(lift.move(digital_speed)); // UP
                    lift_lock_pos = DBL_MAX;
                }
            } else if (r2) {
                if (lift.get_position() <= LIFT_DOWN_POSITION) {
                    if (lift_lock_pos == DBL_MAX) {
                        lift_lock_pos = lift.get_position();
                    }
                } else {
                    p_err(lift.move(-digital_speed)); // DOWN
                    lift_lock_pos = DBL_MAX;
                }
            } else {
                if (lift_lock_pos == DBL_MAX) {
                    p_err(lift.move(0)); // STOP
                }
            }
        }
    }

    if (x) {
        lift_lock_pos = LIFT_UP_POSITION;
    } else if (b) {
        lift_lock_pos = LIFT_DOWN_POSITION;
    }
    
    if (lift_lock_pos != DBL_MAX && lift.get_target_position() != lift_lock_pos) {
        lift.move_absolute(lift.get_position(), digital_speed);
    }
}

void move_arm(bool l1, bool l2, bool a, bool y, unsigned int digital_speed) {
    static double arm_lock_pos_1 = DBL_MAX;
    static double arm_lock_pos_2 = DBL_MAX;
    static bool controller_locked = false;

    if (l1 && l2) {
        move_arm_absolute(arm_1.get_position(), arm_2.get_position(), digital_speed);
        arm_lock_pos_1 = arm_1.get_position();
        arm_lock_pos_2 = arm_2.get_position();
        controller_locked = true;
    } else {
        if (controller_locked && !l1 && !l2) {
            controller_locked = false;
        }

        if (!controller_locked) {
            if (l1) {
                // if (arm_position() >= ARM_UP_POSITION) {
                //     if (arm_lock_pos_1 == DBL_MAX) {
                //         arm_lock_pos_1 = arm_1.get_position();
                //     }
                //     if (arm_lock_pos_2 == DBL_MAX) {
                //         arm_lock_pos_2 = arm_2.get_position();
                //     }
                // } else {
                    move_arm(digital_speed); // UP
                    arm_lock_pos_1 = DBL_MAX;
                    arm_lock_pos_2 = DBL_MAX;
                // }
            } else if (l2) {
                if (arm_position() <= ARM_DOWN_POSITION) {
                    if (arm_lock_pos_1 == DBL_MAX) {
                        arm_lock_pos_1 = arm_1.get_position();
                    }
                    if (arm_lock_pos_2 == DBL_MAX) {
                        arm_lock_pos_2 = arm_2.get_position();
                    }
                } else {
                    move_arm(digital_speed / -1.4); // DOWN
                    arm_lock_pos_1 = DBL_MAX;
                    arm_lock_pos_2 = DBL_MAX;
                }
            } else {
                if (arm_lock_pos_1 == DBL_MAX) {
                    move_arm(0); // STOP
                }
            }
        }
    }

    if (a) {
        arm_lock_pos_1 = ARM_UP_POSITION;
        arm_lock_pos_2 = ARM_UP_POSITION;
    } else if (y) {
        arm_lock_pos_1 = ARM_DOWN_POSITION;
        arm_lock_pos_2 = ARM_DOWN_POSITION;
    }

    if (arm_lock_pos_1 != DBL_MAX && arm_1.get_target_position() != arm_lock_pos_1) {
        arm_1.move_absolute(arm_lock_pos_1, digital_speed / 3.0);
    }
    if (arm_lock_pos_2 != DBL_MAX && arm_2.get_target_position() != arm_lock_pos_2) {
        arm_2.move_absolute(arm_lock_pos_2, digital_speed / 2.0);
    }
}

void move_arm_hook(bool left, bool right, unsigned int digital_speed) {
    static double arm_hook_lock_pos = DBL_MAX;
    static int ticks_closed = 0;
    static bool controller_locked = false;

    if (left && right) {
        p_err(arm_hook.move_relative(0, digital_speed));
        arm_hook_lock_pos = p_err(arm_hook.get_target_position());
        controller_locked = true;
        ticks_closed = 0;
        controller.set_text(1, 0, "Locked");
    } else {
        if (controller_locked && !left && !right) {
            controller_locked = false;
        }
        if (!controller_locked) {
            if (left) {
                p_err(arm_hook.move(digital_speed / 1.5)); // OPEN
                arm_hook_lock_pos = DBL_MAX;
                ticks_closed = 0;
            } else if (right) {
                if (arm_hook.get_efficiency() < 0.001) {
                    if (++ticks_closed > 20) {
                        p_err(arm_hook.move_relative(0, digital_speed));
                        ticks_closed = 0;
                        controller_locked = true;
                        controller.set_text(1, 0, "Locked");
                        arm_hook_lock_pos = p_err(arm_hook.get_target_position());
                    }
                } else {
                    if (ticks_closed > 0) ticks_closed--;
                    p_err(arm_hook.move(digital_speed / -1.5)); // SHUT
                }
            } else if (arm_hook_lock_pos == DBL_MAX) {
                p_err(arm_hook.move(0)); // STOP
                ticks_closed = 0;
            }
        }
    }
}

void drive(bool a, bool b, bool x, bool y, bool up, bool down, bool left, bool right, bool l1, bool l2, bool r1, bool r2, double lx, double ly, double rx, double ry, unsigned int* digital_speed) {
    if (up) {
        if (*digital_speed + 1 <= 127) {
            *digital_speed += 1;
        }
    } else if (down) {
        if (*digital_speed - 1 >= 5) {
            *digital_speed -= 1;
        }
    }

    if (up || down) {
        controller.set_text(0, 0, "Dig Spd: " + std::to_string(*digital_speed).append(" "));
    }

    move_lift(r1, r2, x, b, *digital_speed);
    move_arm(l1, l2, a, y, *digital_speed);
    move_arm_hook(left, right, *digital_speed);
    move_right_motors(ry);
    move_left_motors(ly);
}

#endif