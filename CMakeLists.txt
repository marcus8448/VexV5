cmake_minimum_required(VERSION 3.20)
project(VexV5 VERSION 1.0.0 LANGUAGES C CXX)

set(AUTO_CONFIGURE TRUE CACHE BOOL "auto configure compilers")
set(USE_CLANG FALSE CACHE BOOL "auto configure using clang")
include(toolchain.cmake)

set(FIRMWARE_DIR ${CMAKE_SOURCE_DIR}/firmware)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

file(MAKE_DIRECTORY ${BIN_DIR})

find_program(ProsCLI pros)
find_program(ClangFormat clang-format)
find_program(ClangCheck clang-check)

find_library(LibC NAMES c PATHS ${CMAKE_SOURCE_DIR}/firmware NO_DEFAULT_PATH REQUIRED)
find_library(LibM NAMES m PATHS ${CMAKE_SOURCE_DIR}/firmware NO_DEFAULT_PATH REQUIRED)
find_library(LibPROS NAMES pros PATHS ${CMAKE_SOURCE_DIR}/firmware NO_DEFAULT_PATH REQUIRED)
find_library(LibBase85 NAMES base85 PATHS ${CMAKE_SOURCE_DIR}/firmware NO_DEFAULT_PATH REQUIRED)
find_library(LibBanner NAMES banner PATHS ${CMAKE_SOURCE_DIR}/firmware/extra NO_DEFAULT_PATH)

file(TOUCH ${CMAKE_BINARY_DIR}/empty.cpp)
file(WRITE ${CMAKE_BINARY_DIR}/timestamp.cpp "char const * const _PROS_COMPILE_TIMESTAMP = __DATE__ \" \" __TIME__; char const * const _PROS_COMPILE_DIRECTORY = \"${CMAKE_SOURCE_DIR}\";\n")
add_library(Timestamp STATIC ${CMAKE_BINARY_DIR}/timestamp.cpp)

set(ColdLibraries ${LibC} ${LibM} ${LibPROS} ${LibBase85})
set(HotLinked ${LibC},${LibM},${LibPROS},${LibBase85})

set(SOURCE_FILES
        src/configuration.cpp
        src/error.cpp
        src/logger.cpp
        src/main.cpp
        src/fs/filesystem.cpp
        src/robot/autonomous/autonomous.cpp
        src/robot/autonomous/left_skills.cpp
        src/robot/autonomous/left_winpoint.cpp
        src/robot/autonomous/none.cpp
        src/robot/autonomous/replay.cpp
        src/robot/autonomous/right_skills.cpp
        src/robot/autonomous/right_winpoint.cpp
        src/robot/controller/operator.cpp
        src/robot/controller/autonomous_recording.cpp
        src/robot/device/device.cpp
        src/robot/device/motor.cpp
        src/robot/device/optical.cpp
        src/robot/device/pneumatics.cpp
        src/robot/drivetrain.cpp
        src/robot/expansion.cpp
        src/robot/flywheel.cpp
        src/robot/indexer.cpp
        src/robot/intake.cpp
        src/robot/robot.cpp
        src/screen/autonomous_select.cpp
        src/screen/banner.cpp
        src/screen/config_screen.cpp
        src/screen/drivetrain_chart.cpp
        src/screen/flywheel_chart.cpp
        src/screen/information.cpp
        src/screen/lvgl_util.cpp
        src/screen/screen.cpp
        src/screen/serial_screen.cpp
        src/serial/robot_command.cpp
        src/serial/robot_state.cpp
        src/serial/serial.cpp
        src/serial/serial_connection.cpp
        src/util.cpp
)

set(HEADER_FILES
        include/configuration.hpp
        include/error.hpp
        include/logger.hpp
        include/main.hpp
        include/fs/filesystem.hpp
        include/robot/autonomous/autonomous.hpp
        include/robot/autonomous/left_skills.hpp
        include/robot/autonomous/left_winpoint.hpp
        include/robot/autonomous/none.hpp
        include/robot/autonomous/replay.hpp
        include/robot/autonomous/right_skills.hpp
        include/robot/autonomous/right_winpoint.hpp
        include/robot/controller/controller.hpp
        include/robot/controller/operator.hpp
        include/robot/controller/autonomous_recording.hpp
        include/robot/device/device.hpp
        include/robot/device/motor.hpp
        include/robot/device/optical.hpp
        include/robot/device/pneumatics.hpp
        include/robot/drivetrain.hpp
        include/robot/expansion.hpp
        include/robot/flywheel.hpp
        include/robot/indexer.hpp
        include/robot/intake.hpp
        include/robot/robot.hpp
        include/screen/autonomous_select.hpp
        include/screen/banner.hpp
        include/screen/colour.hpp
        include/screen/config_screen.hpp
        include/screen/drivetrain_chart.hpp
        include/screen/flywheel_chart.hpp
        include/screen/information.hpp
        include/screen/lvgl_util.hpp
        include/screen/screen.hpp
        include/screen/serial_screen.hpp
        include/serial/robot_command.hpp
        include/serial/robot_state.hpp
        include/serial/serial.hpp
        include/serial/serial_connection.hpp
        include/util.hpp
)

add_executable(ColdPackage)
target_sources(ColdPackage PRIVATE ${CMAKE_BINARY_DIR}/empty.cpp)

add_executable(HotPackage)
target_sources(HotPackage PUBLIC FILE_SET headers TYPE HEADERS FILES ${HEADER_FILES})
target_sources(HotPackage PRIVATE ${SOURCE_FILES} $<TARGET_OBJECTS:Timestamp>)

target_include_directories(HotPackage PRIVATE include)
target_link_directories(HotPackage PRIVATE firmware)

if (ProsCLI)
add_custom_target(Upload DEPENDS HotPackage COMMAND ${ProsCLI} upload)
endif()

if (ClangFormat)
message(NOTICE "clang-format found, adding utility target.")
add_custom_target(FormatCode COMMAND cd ${CMAKE_SOURCE_DIR} && ${ClangFormat} -i ${HEADER_FILES} ${SOURCE_FILES})
endif()

if (LibBanner)
target_compile_options(HotPackage PRIVATE -DBANNER_AVAILABLE)
set(ColdLibraries ${ColdLibraries} ${LibBanner})
set(HotLinked ${HotLinked},${LibBanner})
endif()

if (ClangCheck)
message(NOTICE "clang-check found, adding utility target.")
add_custom_target(
        CheckCode
        COMMAND cd ${CMAKE_SOURCE_DIR} && ${ClangCheck} --extra-arg="--target=${TARGET_TRIPLE}" --extra-arg="--sysroot=${ARM_SYSROOT}" --extra-arg="-I${CMAKE_SOURCE_DIR}/include" --extra-arg="-I${ARM_SYSROOT}/include/c++/${ARM_GCC_VERSION}" --extra-arg="-I${ARM_SYSROOT}/include/c++/${ARM_GCC_VERSION}/${TARGET_TRIPLE}/${ARM_MULTI_DIR}" --analyze --analyzer-output-path="${CMAKE_BINARY_DIR}/analysis" -p ${CMAKE_BINARY_DIR} ${HEADER_FILES} ${SOURCE_FILES}
)
endif()

add_dependencies(HotPackage ColdPackage)

target_compile_options(Timestamp PRIVATE -Wno-unused-const-variable -Wno-reserved-identifier -Wno-date-time)
target_link_libraries(HotPackage PUBLIC ${ColdLibraries})

set_property(TARGET HotPackage PROPERTY LINK_DEPENDS ${FIRMWARE_DIR}/v5-common.ld ${FIRMWARE_DIR}/v5-hot.ld)
set_property(TARGET ColdPackage PROPERTY LINK_DEPENDS ${FIRMWARE_DIR}/v5-common.ld ${FIRMWARE_DIR}/v5.ld)

target_link_options(HotPackage PRIVATE -Wl,-R,$<TARGET_FILE:ColdPackage> -nostartfiles -Wl,-T${FIRMWARE_DIR}/v5-hot.ld,--gc-sections,--start-group,${HotLinked},-lgcc,-lstdc++,--end-group,-T${FIRMWARE_DIR}/v5-common.ld)
target_link_options(ColdPackage PRIVATE -Wl,--gc-keep-exported,--whole-archive,${LibPROS},-lstdc++,--no-whole-archive -Wl,-T${FIRMWARE_DIR}/v5.ld,--gc-sections,--start-group,${HotLinked},-lgcc,-lstdc++,--end-group,-T${FIRMWARE_DIR}/v5-common.ld)

# Strip packages after build
add_custom_command(TARGET ColdPackage POST_BUILD COMMAND ${CMAKE_OBJCOPY} --strip-symbol=install_hot_table --strip-symbol=__libc_init_array --strip-symbol=_PROS_COMPILE_DIRECTORY --strip-symbol=_PROS_COMPILE_TIMESTAMP --strip-symbol=_PROS_COMPILE_TIMESTAMP_INT $<TARGET_FILE:ColdPackage>)
add_custom_command(TARGET ColdPackage POST_BUILD COMMAND ${CMAKE_OBJCOPY} -O binary -R .hot_init $<TARGET_FILE:ColdPackage> ${BIN_DIR}/cold.package.bin BYPRODUCTS ${BIN_DIR}/cold.package.bin)
add_custom_command(TARGET HotPackage POST_BUILD COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:HotPackage> ${BIN_DIR}/hot.package.bin BYPRODUCTS ${BIN_DIR}/hot.package.bin)
add_custom_command(TARGET HotPackage POST_BUILD COMMAND ${CMAKE_STRIP} --strip-symbol=_PROS_COMPILE_DIRECTORY --strip-symbol=_PROS_COMPILE_TIMESTAMP --strip-symbol=_PROS_COMPILE_TIMESTAMP_INT $<TARGET_FILE:HotPackage>)
