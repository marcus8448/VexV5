#ifndef _FILESYSTEM_HPP_
#define _FILESYSTEM_HPP_

#include <string>

// https://stackoverflow.com/a/12774387
bool file_exists(std::string name) {
    if (FILE* file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

#endif // _FILESYSTEM_HPP_
