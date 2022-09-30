#include "screen/autonomous_select.hpp"
#include "robot.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#pragma clang diagnostic pop

namespace screen {
AutonomousSelect::AutonomousSelect() = default;

void AutonomousSelect::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {}

void AutonomousSelect::initialize(lv_coord_t width, lv_coord_t height) {}

void AutonomousSelect::update(Robot *robot) {}

void AutonomousSelect::destroy(lv_obj_t *screen) {}
} // screen