#ifndef RECORDING_HPP
#define RECORDING_HPP

#include <fstream>
#include "vexv5/robot.hpp"

/**
 * Serialize the state of the controller to the output stream
 * \param outf the output stream to write to
 * \param controller the controller
 */
void serialize_controller_state(std::ofstream* outf, Controller controller);

#endif
