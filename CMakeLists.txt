cmake_minimum_required(VERSION 3.20)
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
        include/error.hpp
        include/logger.hpp
        include/robot/controller/operator.hpp
        include/robot/controller/recording.hpp
        include/robot/controller/replay.hpp
        include/reset.hpp
        include/robot/robot.hpp
        include/robot/flywheel.hpp
        include/robot/drivetrain.hpp
        include/screen/screen.hpp
        include/screen/drivetrain_chart.hpp
        include/screen/colour.hpp
        include/screen/flywheel_chart.hpp
        include/screen/autonomous_select.hpp
        include/screen/logs.hpp
        include/screen/lvgl_util.hpp
        include/screen/info.hpp
        include/serial_plugins.hpp
        include/util.hpp
        include/serial/serial.hpp
        src/error.cpp
        src/logger.cpp
        src/main.cpp
        src/robot/controller/operator.cpp
        src/robot/controller/recording.cpp
        src/robot/controller/replay.cpp
        src/reset.cpp
        src/robot/robot.cpp
        src/robot/flywheel.cpp
        src/screen/screen.cpp
        src/serial_plugins.cpp
        src/util.cpp
        src/screen/lvgl_util.cpp
        src/screen/info.cpp
        src/screen/drivetrain_chart.cpp
        src/screen/flywheel_chart.cpp
        src/screen/autonomous_select.cpp
        src/screen/logs.cpp
        src/serial/serial.cpp
        src/robot/drivetrain.cpp
        $<TARGET_OBJECTS:Timestamp>
)

add_custom_target(
        Binaries
        DEPENDS ${BIN_DIR}/hot.package.bin ${BIN_DIR}/cold.package.bin
)

if (NOT (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"))
    add_custom_command(
            OUTPUT ${BIN_DIR}/hot.package.bin ${BIN_DIR}/cold.package.bin
            DEPENDS ${BIN_DIR}/hot.package.elf ${BIN_DIR}/cold.package.elf
            COMMAND ${CMAKE_STRIP} --strip-all --keep-symbol=_PROS_COMPILE_DIRECTORY --keep-symbol=_PROS_COMPILE_TIMESTAMP ${BIN_DIR}/hot.package.elf ${BIN_DIR}/cold.package.elf
            COMMAND ${CMAKE_OBJCOPY} -O binary -R .hot_init --strip-symbol=install_hot_table --strip-symbol=__libc_init_array ${BIN_DIR}/cold.package.elf ${BIN_DIR}/cold.package.bin
            COMMAND ${CMAKE_OBJCOPY} -O binary ${BIN_DIR}/hot.package.elf ${BIN_DIR}/hot.package.bin
    )
else()
    add_custom_command(
            OUTPUT ${BIN_DIR}/hot.package.bin ${BIN_DIR}/cold.package.bin
            DEPENDS ${BIN_DIR}/hot.package.elf ${BIN_DIR}/cold.package.elf
            COMMAND ${CMAKE_OBJCOPY} -O binary -R .hot_init --strip-symbol=install_hot_table --strip-symbol=__libc_init_array ${BIN_DIR}/cold.package.elf ${BIN_DIR}/cold.package.bin
            COMMAND ${CMAKE_OBJCOPY} -O binary ${BIN_DIR}/hot.package.elf ${BIN_DIR}/hot.package.bin
    )
endif()

if (ProsCLI)
add_custom_target(
        Upload
        DEPENDS Binaries
        COMMAND ${ProsCLI} upload
)
endif()

add_dependencies(HotPackage ColdPackage)

set_target_properties(HotPackage ColdPackage Timestamp PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
        PREFIX ""
        SUFFIX ""
)
set_property(TARGET HotPackage PROPERTY OUTPUT_NAME hot.package.elf)
set_property(TARGET ColdPackage PROPERTY OUTPUT_NAME cold.package.elf)
target_compile_options(Timestamp PRIVATE -Wno-unused-const-variable)

target_link_libraries(HotPackage PUBLIC ${LibC} ${LibM} ${LibPROS})

target_link_options(
        HotPackage
        PUBLIC -Wl,-R,${BIN_DIR}/cold.package.elf
        PUBLIC -Wl,-T${FIRMWARE_DIR}/v5-hot.ld,--gc-sections,--start-group,${FIRMWARE_DIR}/libc.a,${FIRMWARE_DIR}/libm.a,${FIRMWARE_DIR}/libpros.a,-lgcc,-lstdc++,--end-group,-T${FIRMWARE_DIR}/v5-common.ld
)

target_link_options(
        ColdPackage
        PUBLIC -Wl,--gc-keep-exported,--whole-archive,${FIRMWARE_DIR}/libpros.a,-lstdc++,--no-whole-archive
        PUBLIC -Wl,-T${FIRMWARE_DIR}/v5.ld,--gc-sections,--start-group,${FIRMWARE_DIR}/libc.a,${FIRMWARE_DIR}/libm.a,${FIRMWARE_DIR}/libpros.a,-lgcc,-lstdc++,--end-group,-T${FIRMWARE_DIR}/v5-common.ld
)
