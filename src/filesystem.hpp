#ifndef _FILESYSTEM_HPP_
#define _FILESYSTEM_HPP_

#include <string>
#include <stdio.h>

// https://stackoverflow.com/a/12774387
bool file_exists(std::string name) {
    if (FILE* file = std::fopen(name.c_str(), "r")) {
        std::fclose(file);
        return true;
    } else {
        return false;
    }   
}

#endif // _FILESYSTEM_HPP_
