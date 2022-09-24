cmake_minimum_required(VERSION 3.23)
project(VexV5 VERSION 1.0.0 LANGUAGES C CXX)
include(toolchain.cmake)

set(FIRMWARE_DIR ${CMAKE_SOURCE_DIR}/firmware)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

include_directories(include)

file(TOUCH ${BUILD_DIR}/empty.cpp)
file(WRITE ${BUILD_DIR}/timestamp.cpp "char const * const _PROS_COMPILE_TIMESTAMP = __DATE__ \" \" __TIME__; char const * const _PROS_COMPILE_DIRECTORY = \"${CMAKE_SOURCE_DIR}\";")
add_library(Timestamp ${BUILD_DIR}/timestamp.cpp)

add_executable(ColdPackage ${BUILD_DIR}/empty.cpp)
add_executable(
        HotPackage
        include/config.hpp
        include/constants.hpp
        include/debug.hpp
        include/error.hpp
        include/filesystem.hpp
        include/logger.hpp
        include/recording.hpp
        include/replay.hpp
        include/reset.hpp
        include/robot.hpp
        include/robot_debug.hpp
        include/util.hpp
        src/debug.cpp
        src/error.cpp
        src/filesystem.cpp
        src/logger.cpp
        src/main.cpp
        src/recording.cpp
        src/replay.cpp
        src/reset.cpp
        src/robot.cpp
        src/robot_debug.cpp
        src/util.cpp
        ${Timestamp}
)

add_dependencies(HotPackage ColdPackage)

set_target_properties(
        HotPackage ColdPackage
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR} PREFIX "" SUFFIX ""
)
set_property(TARGET HotPackage PROPERTY OUTPUT_NAME hot.package.elf)
set_property(TARGET ColdPackage PROPERTY OUTPUT_NAME cold.package.elf)

target_link_libraries(HotPackage PUBLIC ${LibC} ${LibM} ${LibPROS})
target_link_libraries(ColdPackage PUBLIC ${LibC} ${LibM} ${LibPROS})

target_link_options(
        HotPackage
        PUBLIC -nostartfiles
        PUBLIC -Wl,-R,${BIN_DIR}/cold.package.elf
        PUBLIC -Wl,-T${FIRMWARE_DIR}/v5-hot.ld
        PUBLIC -Wl,--gc-sections,--start-group,${FIRMWARE_DIR}/libc.a,${FIRMWARE_DIR}/libm.a,-lgcc,-lstdc++,--end-group
        PUBLIC -Wl,-T${FIRMWARE_DIR}/v5-common.ld
)

target_link_options(
        ColdPackage
        PUBLIC -Wl,--gc-keep-exported,--whole-archive,${FIRMWARE_DIR}/libpros.a,-lstdc++,--no-whole-archive
        PUBLIC -Wl,-T${FIRMWARE_DIR}/v5.ld,--gc-sections,--start-group,${FIRMWARE_DIR}/libc.a,${FIRMWARE_DIR}/libm.a,${FIRMWARE_DIR}/libpros.a,-lgcc,-lstdc++,--end-group,-T${FIRMWARE_DIR}/v5-common.ld
)

#add_custom_command(
#        TARGET ColdPackage
#        POST_BUILD
#        COMMAND ${CMAKE_STRIP} -g ${BIN_DIR}/cold.package.elf
#)

add_custom_command(
        TARGET ColdPackage
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary -R .hot_init ${BIN_DIR}/cold.package.elf ${BIN_DIR}/cold.package.bin
)

#add_custom_command(
#        TARGET HotPackage
#        POST_BUILD
#        COMMAND ${CMAKE_STRIP} -g ${BIN_DIR}/hot.package.elf
#)

add_custom_command(
        TARGET HotPackage
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary ${BIN_DIR}/hot.package.elf ${BIN_DIR}/hot.package.bin
)
