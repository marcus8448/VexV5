#include <cstring>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "vexv5/robot.hpp"

void serialize_controller_state(std::ofstream* outf, Controller controller) {
    if (outf == nullptr) return;
    if (controller.up_pressed() && controller.down_pressed()) {
        while (!pros::usd::is_installed()) {
            controller.set_line(2, 0, "Missing microSD!");
            pros::delay(250);
        }
        outf->flush();
        outf->close();
        
        controller.set_line(0, 0, "Recording Stopped");
        outf = nullptr;
        return;
    }

    if (controller.a_press_length() == 1) {
        *outf << 'a';
    } else if (controller.b_press_length() == 1) {
        *outf << 'b';
    } else if (controller.x_press_length() == 1) {
        *outf << 'x';
    } else if (controller.y_press_length() == 1) {
        *outf << 'y';
    } else if (controller.up_press_length() == 1) {
        *outf << 'u';
    } else if (controller.down_press_length() == 1) {
        *outf << 'd';
    } else if (controller.left_press_length() == 1) {
        *outf << 'l';
    } else if (controller.right_press_length() == 1) {
        *outf << 'r';
    } else if (controller.l1_press_length() == 1) {
        *outf << '!';
    } else if (controller.l2_press_length() == 1) {
        *outf << '@';
    } else if (controller.r1_press_length() == 1) {
        *outf << '#';
    } else if (controller.r2_press_length() == 1) {
        *outf << '$';
    }
    *outf << '*';
    double val;
    signed long long position;
    val = controller.left_stick_x();
    std::memcpy(&position, &val, sizeof(val));
    *outf << position << " ";
    val = controller.left_stick_y();
    std::memcpy(&position, &val, sizeof(val));
    *outf << position << " ";
    val = controller.right_stick_x();
    std::memcpy(&position, &val, sizeof(val));
    *outf << position << " ";
    val = controller.right_stick_y();
    std::memcpy(&position, &val, sizeof(val));
    *outf << position;
}
