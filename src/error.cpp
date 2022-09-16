#include "error.hpp"
#include "logger.hpp"
#include <cerrno>
#include <cstring>

bool check_error() {
    if (errno != 0) {
        logger::info_("Error: ", strerror(errno));
        errno = 0;
        return false;
    }
    return true;
}
