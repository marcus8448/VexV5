#ifndef DEBUG_TOOLS_HPP
#define DEBUG_TOOLS_HPP

/**
 * Prints out the given characters to standard output.
 * Output viewable through `pros terminal` on a connected computer.
 * \param line the characters to print
 */
void print(const char* line);

/**
 * Prints out the given string to standard output.
 * Output viewable through `pros terminal` on a connected computer.
 * \param line the string to print
 */
void print(const std::string& line);

/**
 * Prints out the given integer to standard output.
 * Output viewable through `pros terminal` on a connected computer.
 * \param line the integer to print
 */
void print(int line);

/**
 * Prints out the given unsigned integer to standard output.
 * Output viewable through `pros terminal` on a connected computer.
 * \param line the unsigned integer to print
 */
void print(unsigned int line);

/**
 * Prints out the given double to standard output.
 * Output viewable through `pros terminal` on a connected computer.
 * \param line the double to print
 */
void print(double line);

/**
 * Prints out the given long long to standard output.
 * Output viewable through `pros terminal` on a connected computer.
 * \param line the long long to print
 */
void print(long long line);

/**
 * Prints out the given characters to standard error.
 * Output viewable through `pros terminal` on a connected computer.
 * \param line the characters to print
 */
void print_error(const char* line);

/**
 * Prints out the given line to standard error.
 * Output viewable through `pros terminal` on a connected computer.
 * \param line the string to print
 */
void print_error(const std::string& line);

/**
 * Prints out the given integer to standard error.
 * Output viewable through `pros terminal` on a connected computer.
 * \param line the integer to print
 */
void print_error(int line);

/**
 * Prints out the given double to standard error.
 * Output viewable through `pros terminal` on a connected computer.
 * \param line the double to print
 */
void print_error(double line);

/**
 * Prints out the given long long to standard error.
 * Output viewable through `pros terminal` on a connected computer.
 * \param line the long long to print
 */
void print_error(long long line);

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \param return_code the return code of the pros function.
 * \return the same return_code parameter passed.
 */
int erri(int return_code);

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \param return_code the return code of the pros function.
 * \return the same return_code parameter passed.
 */
double errd(double return_code);

/**
 * Handles debug commands/input from standard input.
 */
void debug_input_task();

#endif
