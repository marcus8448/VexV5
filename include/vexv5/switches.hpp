#ifndef SWITCHES_HPP
#define SWITCHES_HPP

#include "vexv5/robot.hpp"

void select_autonomous();

int call_reset_positions();

std::ofstream* create_record_stream();

void call_serialize_controller_state(std::ofstream* outf, Controller controller);

#endif
