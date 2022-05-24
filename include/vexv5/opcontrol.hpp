#ifndef OPCONTROL_HPP
#define OPCONTROL_HPP

/**
 * Handles operator input to move the lift up/down.
 * \param r1 whether the r1 button is pressed
 * \param r2 whether the r2 button is pressed
 * \param x whether the x button is pressed
 * \param b whether the b button is pressed
 * \param digital_speed the speed to move the lift
 */
void move_lift(bool r1, bool r2, bool x, bool b, unsigned int digital_speed);

/**
 * Handles operator input to move the arm up/down.
 * \param l1 whether the l1 button is pressed
 * \param l2 whether the l2 button is pressed
 * \param a whether the a button is pressed
 * \param y whether the a button is pressed
 * \param digital_speed the speed to move the arm
 */
void move_arm(bool l1, bool l2, bool a, bool y, unsigned int digital_speed);

/**
 * Handles operator input to open/shut the arm hook.
 * \param left whether the left arrow button is pressed
 * \param right whether the right arrow button is pressed
 * \param digital_speed the speed to open/close the arm hook
 */
void move_arm_hook(bool left, bool right, unsigned int digital_speed);

/**
 * Handles operator input to drive the robot.
 * 
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
 * \param digital_speed the speed to move motors controlled by buttons
 */
void drive(bool a, bool b, bool x, bool y, bool up, bool down, bool left, bool right, bool l1, bool l2, bool r1, bool r2, double lx, double ly, double rx, double ry, unsigned int* digital_speed);

#endif
