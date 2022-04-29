#include "devices.hpp"
#include <stdio.h>

void reset_positions() {
    controller.set_text(0, 0, "Reset...");
    // arm_hook.move(10);
    arm_1.move(-20);
    arm_2.move(-20);
    lift.move(20);
    delay(500);
    while (arm_hook.get_efficiency() > 0.5 || arm_1.get_efficiency() > 0.5 || arm_2.get_efficiency() > 0.5 || lift.get_efficiency() > 5) {
        delay(200);
    }
    // arm_hook.move(0);
    arm_1.move(0);
    arm_2.move(0);
    lift.move(0);
    controller.set_text(0, 0, "Done!");
    std::cout << "Done reset." << std::endl;
}