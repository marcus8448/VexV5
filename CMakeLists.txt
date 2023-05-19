cmake_minimum_required(VERSION 3.22)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_SYSTEM_VERSION 0)
set(TARGET_TRIPLE arm-none-eabi)

set(CMAKE_C_COMPILER_TARGET ${TARGET_TRIPLE})
set(CMAKE_CXX_COMPILER_TARGET ${TARGET_TRIPLE})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

include(CMakeOptions.txt)

if (NOT DEFINED CMAKE_SHARED_LIBRARY_SUFFIX)
    if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        set(CMAKE_SHARED_LIBRARY_SUFFIX dll)
    elseif (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
        set(CMAKE_SHARED_LIBRARY_SUFFIX dylib)
    else()
        set(CMAKE_SHARED_LIBRARY_SUFFIX so)
    endif()
endif()

set(MFLAGS "-mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=softfp -nostdlib")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS} ${MFLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS} ${MFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-enum-size-warning -z noexecstack")

set(CMAKE_LINKER ${CMAKE_CXX_COMPILER})

set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(FIRMWARE_DIR ${CMAKE_SOURCE_DIR}/firmware)
link_directories(BEFORE ${FIRMWARE_DIR} ${FIRMWARE_DIR}/extra)
set(LibC ${FIRMWARE_DIR}/libc.a)
set(LibM ${FIRMWARE_DIR}/libm.a)
set(LibPROS ${FIRMWARE_DIR}/libpros.a)
set(LibBase85 ${FIRMWARE_DIR}/libbase85.a)

message(STATUS "Using toolchain: ${CMAKE_TOOLCHAIN_FILE}")

project(VexV5 VERSION 1.0.0 LANGUAGES C CXX)

set(FIRMWARE_DIR ${CMAKE_SOURCE_DIR}/firmware)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

file(MAKE_DIRECTORY ${BIN_DIR})

find_program(ProsCLI pros)
find_program(ClangFormat clang-format)
find_program(ClangCheck clang-check)

find_library(LibC NAMES c REQUIRED)
find_library(LibM NAMES m REQUIRED)
find_library(LibPROS NAMES pros REQUIRED)
find_library(LibBase85 NAMES base85 REQUIRED)

file(TOUCH ${CMAKE_BINARY_DIR}/empty.cpp)
file(WRITE ${CMAKE_BINARY_DIR}/timestamp.cpp "char const * const _PROS_COMPILE_TIMESTAMP = __DATE__ \" \" __TIME__; char const * const _PROS_COMPILE_DIRECTORY = \"${CMAKE_SOURCE_DIR}\";\n")
add_library(Timestamp STATIC ${CMAKE_BINARY_DIR}/timestamp.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(WARNING_FLAGS "-Wall -Wno-psabi -Wno-shadow-field-in-constructor")
set(COMPILE_FLAGS "-g -ffunction-sections -fdata-sections -fdiagnostics-color -funwind-tables -fno-exceptions")
add_compile_definitions(_POSIX_THREADS _UNIX98_THREAD_MUTEX_ATTRIBUTES)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS} ${WARNING_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMPILE_FLAGS} ${WARNING_FLAGS}")

set(ColdLibraries ${LibC} ${LibM} ${LibPROS} ${LibBase85})
set(HotLinked ${LibC},${LibM},${LibPROS},${LibBase85})

set(SOURCE_FILES
        src/main.cpp
        src/control/autonomous/autonomous.cpp
        src/control/autonomous/score.cpp
        src/control/autonomous/skills.cpp
        src/control/autonomous/replay.cpp
        src/control/autonomous/winpoint.cpp
        src/control/input/autonomous_outline.cpp
        src/control/input/operator.cpp
        src/control/input/raw_recording.cpp
        src/control/input/raw_replay.cpp
        src/debug/error.cpp
        src/debug/logger.cpp
        src/filesystem.cpp
        src/robot/device/device.cpp
        src/robot/device/gyro.cpp
        src/robot/device/inertial.cpp
        src/robot/device/motor.cpp
        src/robot/device/optical.cpp
        src/robot/device/pneumatics.cpp
        src/robot/pid/tbh.cpp
        src/robot/pid/vex.cpp
        src/robot/drivetrain.cpp
        src/robot/robot.cpp
        src/screen/autonomous_select.cpp
        src/screen/config_screen.cpp
        src/screen/drivetrain_chart.cpp
        src/screen/information.cpp
        src/screen/lvgl_util.cpp
        src/screen/screen.cpp
        src/serial/robot_command.cpp
        src/serial/robot_state.cpp
        src/serial/serial.cpp
        src/serial/serial_connection.cpp
)

set(HEADER_FILES
        include/main.hpp
        include/debug/error.hpp
        include/debug/logger.hpp
        include/filesystem.hpp
        include/control/autonomous/autonomous.hpp
        include/control/autonomous/score.hpp
        include/control/autonomous/skills.hpp
        include/control/autonomous/replay.hpp
        include/control/autonomous/winpoint.hpp
        include/control/input/controller.hpp
        include/control/input/autonomous_outline.hpp
        include/control/input/operator.hpp
        include/control/input/raw_recording.hpp
        include/control/input/raw_replay.hpp
        include/robot/device/device.hpp
        include/robot/device/gyro.hpp
        include/robot/device/inertial.hpp
        include/robot/device/motor.hpp
        include/robot/device/optical.hpp
        include/robot/device/pneumatics.hpp
        include/robot/pid/tbh.hpp
        include/robot/pid/pid.hpp
        include/robot/drivetrain.hpp
        include/robot/robot.hpp
        include/screen/autonomous_select.hpp
        include/screen/colour.hpp
        include/screen/config_screen.hpp
        include/screen/drivetrain_chart.hpp
        include/screen/information.hpp
        include/screen/lvgl_util.hpp
        include/screen/screen.hpp
        include/serial/robot_command.hpp
        include/serial/robot_state.hpp
        include/serial/serial.hpp
        include/serial/serial_connection.hpp
        include/units.hpp
)

if (ENABLE_TEMPORARY_CODE)
    set(SOURCE_FILES "${SOURCE_FILES} src/temporary.hpp")
endif()

add_executable(ColdPackage)
target_sources(ColdPackage PRIVATE ${CMAKE_BINARY_DIR}/empty.cpp)

add_executable(HotPackage)
target_sources(HotPackage PUBLIC FILE_SET headers TYPE HEADERS FILES ${HEADER_FILES})
target_sources(HotPackage PRIVATE ${SOURCE_FILES} $<TARGET_OBJECTS:Timestamp>)

target_include_directories(HotPackage PRIVATE include)
target_link_directories(HotPackage PRIVATE firmware)

if (ProsCLI)
add_custom_target(Upload DEPENDS HotPackage COMMAND ${ProsCLI} upload --slot 1)
endif()

if (ClangFormat)
message(STATUS "clang-format found, adding utility target.")
add_custom_target(FormatCode COMMAND cd ${CMAKE_SOURCE_DIR} && ${ClangFormat} -i ${HEADER_FILES} ${SOURCE_FILES})
endif()

add_dependencies(HotPackage ColdPackage)

target_compile_options(Timestamp PRIVATE -Wno-unused-const-variable -Wno-reserved-identifier -Wno-date-time)
target_link_libraries(HotPackage PUBLIC ${ColdLibraries})

set_property(TARGET HotPackage PROPERTY LINK_DEPENDS ${FIRMWARE_DIR}/v5-common.ld ${FIRMWARE_DIR}/v5-hot.ld)
set_property(TARGET ColdPackage PROPERTY LINK_DEPENDS ${FIRMWARE_DIR}/v5-common.ld ${FIRMWARE_DIR}/v5.ld)
set(TARGET_FILE)
target_link_options(HotPackage PRIVATE -Wl,-R,$<TARGET_FILE:ColdPackage> -nostartfiles -Wl,-T${FIRMWARE_DIR}/v5-hot.ld,--gc-sections,--start-group,${HotLinked},-lgcc,-lstdc++,--end-group,-T${FIRMWARE_DIR}/v5-common.ld)
target_link_options(ColdPackage PRIVATE -Wl,--gc-keep-exported,--whole-archive,${LibPROS},-lstdc++,--no-whole-archive -Wl,-T${FIRMWARE_DIR}/v5.ld,--gc-sections,--start-group,${HotLinked},-lgcc,-lstdc++,--end-group,-T${FIRMWARE_DIR}/v5-common.ld)

# Strip packages after build
add_custom_command(TARGET ColdPackage POST_BUILD COMMAND ${CMAKE_OBJCOPY} --strip-symbol=install_hot_table --strip-symbol=__libc_init_array --strip-symbol=_PROS_COMPILE_DIRECTORY --strip-symbol=_PROS_COMPILE_TIMESTAMP --strip-symbol=_PROS_COMPILE_TIMESTAMP_INT $<TARGET_FILE:ColdPackage>)
add_custom_command(TARGET ColdPackage POST_BUILD COMMAND ${CMAKE_OBJCOPY} -O binary -R .hot_init $<TARGET_FILE:ColdPackage> ${BIN_DIR}/cold.package.bin BYPRODUCTS ${BIN_DIR}/cold.package.bin)
add_custom_command(TARGET HotPackage POST_BUILD COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:HotPackage> ${BIN_DIR}/hot.package.bin BYPRODUCTS ${BIN_DIR}/hot.package.bin)
add_custom_command(TARGET HotPackage POST_BUILD COMMAND ${CMAKE_STRIP} --strip-symbol=_PROS_COMPILE_DIRECTORY --strip-symbol=_PROS_COMPILE_TIMESTAMP --strip-symbol=_PROS_COMPILE_TIMESTAMP_INT $<TARGET_FILE:HotPackage>)
