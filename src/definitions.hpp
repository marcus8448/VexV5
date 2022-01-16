#ifndef _MISC_DEFINITIONS_H_
#define _MISC_DEFINITIONS_H_

#include <cmath>
#include <string>

#define PI 3.14159265
#define IN_TO_FT 12

enum ControlState {
    INITIALIZE,
    COMPETITION_INITIALIZE,
    AUTONOMOUS,
    OPERATOR_CONTROL,
    DISABLED
};

std::string stateName(ControlState state) {
    switch (state) {
        case INITIALIZE:
            return "Initialize";
        case COMPETITION_INITIALIZE:
            return "Competition Initialize";
        case AUTONOMOUS:
            return "Autonomous";
        case OPERATOR_CONTROL:
            return "Operator Control";
        case DISABLED:
            return "Disabled";
    }
    return "INVALID";
}

lv_res_t always_ok_event(struct _lv_obj_t* obj) {
    return LV_RES_OK;
}

std::string get_errno(int32_t value) {
    if (value == PROS_ERR) {
        return std::string("Operation Failed: ") + std::string(strerror(errno));
    }
    return std::to_string(value);
}

std::string get_errno(float value) {
    if (value == PROS_ERR_F) {
        return std::string("Operation Failed: ") + std::string(strerror(errno));
    }
    return std::to_string(value);
}

std::string get_errno(double value) {
    if (value == PROS_ERR_F) {
        return std::string("Operation Failed: ") + std::string(strerror(errno));
    }
    return std::to_string(value);
}

#endif// _MISC_DEFINITIONS_H_
