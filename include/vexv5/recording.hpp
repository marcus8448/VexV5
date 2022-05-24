#ifndef RECORDING_HPP
#define RECORDING_HPP

#include <fstream>

/**
 * Serialize the state of the controller to the output stream
 * \param outf the output stream to write to
 * \param a whether the a button is pressed
 * \param b whether the b button is pressed
 * \param x whether the x button is pressed
 * \param y whether the y button is pressed
 * \param up whether the up button is pressed
 * \param down whether the down button is pressed
 * \param left whether the left button is pressed
 * \param right whether the right button is pressed
 * \param l1 whether the l1 button is pressed
 * \param l2 whether the l2 button is pressed
 * \param r1 whether the r1 button is pressed
 * \param r2 whether the r2 button is pressed
 * \param lx whether the lx button is pressed
 * \param ly whether the ly button is pressed
 * \param rx whether the rx button is pressed
 * \param ry whether the ry button is pressed
 */
void serialize_controller_state(std::basic_ofstream<signed int, std::char_traits<signed int>>* outf, bool a, bool b, bool x, bool y, bool up, bool down, bool left, bool right, bool l1, bool l2, bool r1, bool r2, double lx, double ly, double rx, double ry);

#endif
