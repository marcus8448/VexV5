#include <cstring>
#include <filesystem>
#include <fstream>

#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "vexv5/globals.hpp"

void serialize_controller_state(std::ofstream* outf, bool a, bool b, bool x, bool y, bool up, bool down, bool left, bool right, bool l1, bool l2, bool r1, bool r2, double lx, double ly, double rx, double ry) {
    if (outf == nullptr) return;
    if (up && down) {
        while (!usd::is_installed()) {
            controller.set_text(2, 0, "Missing microSD!");
            delay(250);
        }
        (*outf).flush();
        (*outf).close();
        controller.set_text(0, 0, "Recording Stopped");
        outf = nullptr;
    }

    static bool prev_a, prev_b, prev_x, prev_y, prev_up, prev_down, prev_left, prev_right, prev_l1, prev_l2, prev_r1, prev_r2;

    if (prev_a != a) {
        prev_a = a;
        *outf << 'a';
    } else if (prev_b != (prev_b = a)) {
        prev_b = b;
        *outf << 'b';
    } else if (prev_x != x) {
        prev_x = x;
        *outf << 'x';
    } else if (prev_y != y) {
        prev_y = y;
        *outf << 'y';
    } else if (prev_up != up) {
        prev_up = up;
        *outf << 'u';
    } else if (prev_down != down) {
        prev_down = down;
        *outf << 'd';
    } else if (prev_left != left) {
        prev_left = left;
        *outf << 'l';
    } else if (prev_right != right) {
        prev_right = right;
        *outf << 'r';
    } else if (prev_l1 != l1) {
        prev_l1 = l1;
        *outf << '!';
    } else if (prev_l2 != l2) {
        prev_l2 = l2;
        *outf << '@';
    } else if (prev_r1 != r1) {
        prev_r1 = r1;
        *outf << '#';
    } else if (prev_r2 != r2) {
        prev_r2 = r2;
        *outf << '$';
    }
    *outf << '*';
    signed long long position;
    std::memcpy(&position, &lx, sizeof(lx));
    *outf << position;
    std::memcpy(&position, &ly, sizeof(ly));
    *outf << position;
    std::memcpy(&position, &rx, sizeof(rx));
    *outf << position;
    std::memcpy(&position, &ry, sizeof(ry));
    *outf << position;
}
