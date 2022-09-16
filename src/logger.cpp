#include "logger.hpp"
#include <iostream>
#include <memory>
#include <vector>

namespace logger {
    static std::vector<const char*> SECTIONS;

    void info(const char* string) {
        std::cout << string;
    }

    void info(const std::string string) {
        std::cout << string;
    }

    void warn(const char* string) {
        std::cout << string;
    }

    void warn(const std::string string) {
        std::cout << string;
    }

    void error(const char* string) {
        std::cout << string; //todo: cerr?
    }

    void error(const std::string string) {
        std::cout << string;
    }
    
    void debug(const char* string) {
        #ifdef DEBUG_LOG
        std::cout << string;
        #endif
    }

    void debug(const std::string string) {
        #ifdef DEBUG_LOG
        std::cout << string;
        #endif
    }

    void push_section(const char* string) {
        SECTIONS.push_back(string);
        std::cout << "== BEGIN " << string << " ==";
    }

    void pop_section() {
        if (SECTIONS.empty()) {
            error("Section stack underflow!");
        }
        std::cout << "=== END " << SECTIONS.back() << " ===";
        SECTIONS.pop_back();
    }
} //namespace logger
