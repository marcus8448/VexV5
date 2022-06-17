#include "debug.hpp"
#include "robot.hpp"
#include "util.hpp"
#include <iostream>

/**
 * Handles debug commands.
 */
void debug_input_task(void* params) {
    auto robot = static_cast<Robot*>(params);

     while (true) {
         std::string command;
         getline(std::cin, command);
         if (command == "robot") {
             printf("Robot: %s\n", indent(robot->describe()).c_str());
         } else if (command == "drivetrain") {
             printf("Arm: %s\n", indent(robot->drivetrain->describe()).c_str());
         } else if (command == "kill") {
             throw;
         }
         std::cout << std::endl;
     }
}
