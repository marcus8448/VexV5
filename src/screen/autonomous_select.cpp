#include "screen/autonomous_select.hpp"
#include "robot/robot.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#pragma GCC diagnostic pop

namespace screen {
AutonomousSelect::AutonomousSelect() = default;

void AutonomousSelect::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {}

void AutonomousSelect::initialize(lv_coord_t width, lv_coord_t height) {}

void AutonomousSelect::update(robot::Robot *robot) {}

void AutonomousSelect::destroy(lv_obj_t *screen) {}
} // namespace screen