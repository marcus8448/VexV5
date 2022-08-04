#include "robot_debug.hpp"
#include "pros/motors.hpp"
#include <cstddef>
#include <cstring>

const char* ROBOT_STATE = "R_ST";
const char* ROBOT_DEBUG = "RDBG";

RobotStatePlugin::RobotStatePlugin(Robot* robot): robot(robot) {
}

void RobotStatePlugin::initialize(std::streambuf* out, std::streambuf* in) {
    this->raw_out = out;
}

void RobotStatePlugin::clear_state() {
    this->raw_out = nullptr;
}

void RobotStatePlugin::disconnected() {
    this->raw_out = nullptr;
}

typedef double floating;

void serialize_motor(unsigned char* buffer, pros::Motor* motor) {
    floating src;
    src = motor->get_target_position();
    std::memcpy(&buffer[sizeof(floating) * 0], &src, sizeof(floating));
    src = motor->get_actual_velocity();
    std::memcpy(&buffer[sizeof(floating) * 1], &src, sizeof(floating));
    src = motor->get_position();
    std::memcpy(&buffer[sizeof(floating) * 2], &src, sizeof(floating));
    src = motor->get_efficiency();
    std::memcpy(&buffer[sizeof(floating) * 3], &src, sizeof(floating));
    src = motor->get_power();
    std::memcpy(&buffer[sizeof(floating) * 4], &src, sizeof(floating));
    src = motor->get_temperature();
    std::memcpy(&buffer[sizeof(floating) * 5], &src, sizeof(floating));
    src = motor->get_torque();
    std::memcpy(&buffer[sizeof(floating) * 6], &src, sizeof(floating));
    int isrc;
    isrc = motor->get_target_velocity();
    std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 0], &isrc, sizeof(int));
    isrc = motor->get_current_draw();
    std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 1], &isrc, sizeof(int));
    isrc = motor->get_voltage();
    std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 2], &isrc, sizeof(int));
    isrc = motor->get_current_limit();
    std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 3], &isrc, sizeof(int));
    isrc = motor->get_voltage_limit();
    std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 4], &isrc, sizeof(int));
}

bool RobotStatePlugin::handle(char *type) {
    const int CONTROLLER_SIZE = 3 + (sizeof(float) * 4);
    const int MOTOR_SIZE = (sizeof(floating) * 7) + (sizeof(int) * 5);
    const int SIZE = CONTROLLER_SIZE + (MOTOR_SIZE * 4);
    static unsigned char buffer[SIZE] = {0};

    if (type == ROBOT_STATE) {
        if (this->robot->controller->a_pressed()) buffer[0] |= 0b00000001;
        if (this->robot->controller->b_pressed()) buffer[0] |= 0b00000010;
        if (this->robot->controller->x_pressed()) buffer[0] |= 0b00000100;
        if (this->robot->controller->y_pressed()) buffer[0] |= 0b00001000;
        if (this->robot->controller->up_pressed()) buffer[0] |= 0b00010000;
        if (this->robot->controller->down_pressed()) buffer[0] |= 0b00100000;
        if (this->robot->controller->left_pressed()) buffer[0] |= 0b01000000;
        if (this->robot->controller->right_pressed()) buffer[0] |= 0b10000000;
        if (this->robot->controller->l1_pressed()) buffer[1] |= 0b00000001;
        if (this->robot->controller->l2_pressed()) buffer[1] |= 0b00000010;
        if (this->robot->controller->r1_pressed()) buffer[1] |= 0b00000100;
        if (this->robot->controller->r2_pressed()) buffer[1] |= 0b00001000;
        buffer[2] = this->robot->controller->get_digital_speed();
        float src;
        src = (float)this->robot->controller->left_stick_x();
        std::memcpy(&buffer[3 + sizeof(float) * 0], &src, sizeof(float));
        src = (float)this->robot->controller->left_stick_y();
        std::memcpy(&buffer[3 + sizeof(float) * 1], &src, sizeof(float));
        src = (float)this->robot->controller->right_stick_x();
        std::memcpy(&buffer[3 + sizeof(float) * 2], &src, sizeof(float));
        src = (float)this->robot->controller->right_stick_y();
        std::memcpy(&buffer[3 + sizeof(float) * 3], &src, sizeof(float));

        serialize_motor(&buffer[CONTROLLER_SIZE + MOTOR_SIZE * 0], this->robot->drivetrain->rightFront);
        serialize_motor(&buffer[CONTROLLER_SIZE + MOTOR_SIZE * 1], this->robot->drivetrain->leftFront);
        serialize_motor(&buffer[CONTROLLER_SIZE + MOTOR_SIZE * 2], this->robot->drivetrain->rightBack);
        serialize_motor(&buffer[CONTROLLER_SIZE + MOTOR_SIZE * 3], this->robot->drivetrain->leftBack);
        raw_out->sputn(reinterpret_cast<char*>(buffer), SIZE);
    }
}

RobotCommandsPlugin::RobotCommandsPlugin(Robot* robot): robot(robot) {
}

void RobotCommandsPlugin::initialize(std::streambuf* out, std::streambuf* in) {
    this->raw_out = out;
    this->raw_in = in;
}

void RobotCommandsPlugin::clear_state() {
    this->raw_out = nullptr;
    this->raw_in = nullptr;
}

void RobotCommandsPlugin::disconnected() {
    this->raw_out = nullptr;
    this->raw_in = nullptr;
}

bool RobotCommandsPlugin::handle(char *type) {
    static char sizebuf[1];

    if (type == ROBOT_DEBUG) {
        this->raw_in->sgetn(sizebuf, 1);
        int len = static_cast<int>(sizebuf[0]);
        char* buf = new char[len];
        this->raw_in->sgetn(sizebuf, len);
        for(int i = 0; buf[i]; i++) buf[i] = tolower(buf[i]); 
        std::vector<std::string> vec;
        do {
            buf = strtok(buf, " ");
            vec.push_back(std::string(buf));
        } while (buf != nullptr);
        if (vec.size() == 0) return;
        
        if (vec.size() > 5) {
            if (vec[0] == "set") {
                if (vec[1] == "drivetrain") {
                    if (vec[2] == "right_front") {
                        if (vec[3] == "target_velocity") {

                        } else if (vec[3] == "target_voltage") {

                        } else if (vec[3] == "target_position") {

                        } else if (vec[3] == "relative_target_position") {

                        }
                    } else if (vec[2] == "right_back") {

                    } else if (vec[2] == "left_front") {

                    } else if (vec[2] == "left_back") {

                    }
                }
            }
        }
    }
}
