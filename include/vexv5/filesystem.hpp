#ifndef _FILESYSTEM_HPP_
#define _FILESYSTEM_HPP_

/**
 * Tests if a file exists on the microSD filesystem.
 * All files must exist under "/usd/", otherwise VexOS will block the call.
 * \param name the name of the file
 * \return whether the file exists
 */
bool file_exists(const char* name);

#endif // _FILESYSTEM_HPP_
