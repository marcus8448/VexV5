#include <cstdio>

// https://stackoverflow.com/a/12774387
bool file_exists(const char *name) {
  if (FILE * file = std::fopen(name, "r")) {
    std::fclose(file);
    return true;
  } else {
    return false;
  }
}
