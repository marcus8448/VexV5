// #include <cstring>
// #include <fstream>
// #include <iostream>
// #include <iterator>
// #include <map>
// #include <sstream>
// #include <string>
// #include <vector>

// #include "base85/base85.hpp"

// #include "pros/apix.h"
// #include "pros/rtos.hpp"
// #include "serial/serial.hpp"

// #define SERIAL_RESERVED 1
// #define SERIAL_CONNECT 2
// #define SERIAL_RECEIVED 3
// #define SERIAL_RESPONSE 4
// #define SERIAL_DISCONNECT 5
// #define SERIAL_LOGGING 6

// namespace serial {
// static std::map<const uint8_t, PacketHandler *> *packet_handlers = new std::map<const uint8_t, PacketHandler *>;

// enum State { NOT_CONNECTED, AWAITING_RESPONSE, ESTABLISHED };

// State state = NOT_CONNECTED;
// uint32_t lastTime = 0;

// void timeout_hack(void *params) {
//   auto task = static_cast<pros::Task>(params);
//   const uint32_t TIMEOUT_LENGTH = 5000;
//   while (true) {
//     if (state == NOT_CONNECTED) {
//       pros::delay(TIMEOUT_LENGTH);
//     } else {
//       if (pros::millis() - lastTime > TIMEOUT_LENGTH) {
//         state = NOT_CONNECTED;
//         task.remove(); // kill the task.
//         initialize();
//         break;
//       } else {
//         pros::delay(pros::millis() - lastTime + 1);
//       }
//     }
//   }
// }

// /**
//  * Handles debug commands.
//  */
// [[noreturn]] void debug_input_task(void *params) {
//   auto *connection = static_cast<SerialConnection *>(params);
//   for (const auto &[k, v] : *packet_handlers) {
//     v->initialize();
//   }

//   pros::Task(timeout_hack, static_cast<void *>(pros::Task::current()), "Timeout hack");

//   uint16_t cmd;
//   auto cmd_ptr = reinterpret_cast<void *>(&cmd);

//   while (true) {
//     connection->sync_output();
//     lastTime = pros::millis();
//     connection->read_exact(cmd_ptr, sizeof(uint16_t));
//     const char *command = registry->get_name(cmd);
//     switch (state) {
//     case NOT_CONNECTED:
//       if (strcmp(LEGAL_VALUES[SERIAL_CONNECT], command) == 0) {
//         connection->send_exact(SERIAL_RECEIVED);
//         state = AWAITING_RESPONSE;
//       } else {
//         connection->skip_to_end();
//       }
//       break;
//     case AWAITING_RESPONSE:
//       if (strcmp(LEGAL_VALUES[SERIAL_RESPONSE], command) == 0) {
//         lastTime = pros::millis();
//         connection->send_exact(SERIAL_RECEIVED);
//         state = ESTABLISHED;
//       }
//       break;
//     case ESTABLISHED:
//       if (strcmp(LEGAL_VALUES[SERIAL_DISCONNECT], command) == 0) {
//         state = NOT_CONNECTED;
//         connection->skip_to_end();
//       } else {
//         uint16_t len = connection->read_variable(buffer, 512);
//         if (len == 0) {
//           break;
//         }
//         if (plugins->count(cmd)) {
//           plugins->at(cmd)->handle(connection, buffer, len);
//           lastTime = pros::millis();
//         }
//       }
//       break;
//     }
//   }
// }

// void register_packet_handler(const uint8_t id, PacketHandler *handler) {
//   if (id > 85) return;
//   packet_handlers->emplace(id, handler);
// }

// void initialize() {
//   if (buffer == nullptr) {
//     buffer = malloc(512);
//   }
//   pros::Task(debug_input_task, create_serial_connection(), "Debug Input Task");
// }
// } // namespace serial
