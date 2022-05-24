#include "pros/misc.hpp"
#include "pros/rtos.hpp"

#ifndef _ROBOTDATA_EXPORT_HPP_
namespace rdata {
    struct TaskData {
        int delay;
        std::function<double(void)> func;
    };
    
    pros::Task create_export_task(int delay, std::function<double(void)> func);
}
#endif//_ROBOTDATA_EXPORT_HPP_
