#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

/**
 * Tests if a file exists on the microSD filesystem.
 * All files must exist under "/usd/", otherwise VexOS will block the call.
 * \param name the name of the file
 * \return whether the file exists
 */
bool file_exists(const char *name);

#endif//FILESYSTEM_HPP
