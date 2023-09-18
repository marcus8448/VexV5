cmake_minimum_required(VERSION 3.22)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_SYSTEM_VERSION 0)
set(TARGET_TRIPLE arm-none-eabi)
set(CMAKE_CROSSCOMPILING TRUE)

set(CMAKE_C_COMPILER_TARGET ${TARGET_TRIPLE})
set(CMAKE_CXX_COMPILER_TARGET ${TARGET_TRIPLE})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

include(CMakeOptions.txt)

# M-Flags
add_compile_options(-mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=softfp -nostdlib)
add_link_options(-mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=softfp -nostdlib)

add_link_options(LINKER:--no-enum-size-warning -z noexecstack)

set(FIRMWARE_DIR ${CMAKE_SOURCE_DIR}/firmware)
link_directories(BEFORE ${FIRMWARE_DIR} ${FIRMWARE_DIR}/extra)
set(LibC ${FIRMWARE_DIR}/libc.a)
set(LibM ${FIRMWARE_DIR}/libm.a)
set(LibPROS ${FIRMWARE_DIR}/libpros.a)
set(LibLVGL ${FIRMWARE_DIR}/liblvgl.a)

project(VexV5 VERSION 1.0.0 LANGUAGES C CXX)

set(FIRMWARE_DIR ${CMAKE_SOURCE_DIR}/firmware)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

file(MAKE_DIRECTORY ${BIN_DIR})

find_program(ProsCLI pros)
find_program(ClangFormat clang-format)
find_program(ClangTidy clang-tidy)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/empty.cpp)
    file(TOUCH ${CMAKE_BINARY_DIR}/empty.cpp)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

# Warnings
add_compile_options(-Wall -Wno-psabi -Wno-shadow-field-in-constructor)

add_compile_options(-g -ffunction-sections -fdata-sections -fdiagnostics-color -funwind-tables -fno-exceptions)
add_compile_definitions(_POSIX_THREADS _UNIX98_THREAD_MUTEX_ATTRIBUTES _POSIX_TIMERS _POSIX_MONOTONIC_CLOCK)

set(ColdLibraries ${LibC} ${LibM} ${LibPROS} ${LibLVGL})
set(HotLinked ${LibC},${LibM},${LibPROS},${LibLVGL})
set(ExportedLibraries ${LibPROS},${LibLVGL})

set(SOURCE_FILES
        src/main.cpp
        src/tasks.cpp
        src/filesystem.cpp
        src/control/input/operator.cpp
        src/debug/error.cpp
        src/debug/logger.cpp
        src/robot/device/device.cpp
        src/robot/device/inertial.cpp
        src/robot/device/motor.cpp
        src/robot/device/optical.cpp
        src/robot/device/pneumatics.cpp
        src/robot/arm.cpp
        src/robot/drivetrain.cpp
        src/robot/intake.cpp
        src/robot/robot.cpp
)

set(HEADER_FILES
        include/filesystem.hpp
        include/format.hpp
        include/main.hpp
        include/tasks.hpp
        include/units.hpp
        include/debug/error.hpp
        include/debug/logger.hpp
        include/control/input/controller.hpp
        include/control/input/operator.hpp
        include/robot/device/device.hpp
        include/robot/device/inertial.hpp
        include/robot/device/motor.hpp
        include/robot/device/optical.hpp
        include/robot/device/pneumatics.hpp
        include/robot/arm.hpp
        include/robot/drivetrain.hpp
        include/robot/intake.hpp
        include/robot/robot.hpp
        include/structure/fixed_queue.hpp
)

if (NOT DISABLE_SCREEN)
    set(SOURCE_FILES ${SOURCE_FILES}
            src/screen/autonomous_select.cpp
            src/screen/chart.cpp
            src/screen/config_screen.cpp
            src/screen/information.cpp
            src/screen/pid_tuning.cpp
            src/screen/screen.cpp
    )

    set(HEADER_FILES ${HEADER_FILES}
            include/screen/autonomous_select.hpp
            include/screen/chart.hpp
            include/screen/colour.hpp
            include/screen/config_screen.hpp
            include/screen/information.hpp
            include/screen/pid_tuning.hpp
            include/screen/screen.hpp
    )
endif()

if (NOT DISABLE_AUTONOMOUS)
    set(SOURCE_FILES ${SOURCE_FILES}
            src/control/autonomous/autonomous.cpp
            src/control/autonomous/score.cpp
            src/control/autonomous/skills.cpp
            src/control/autonomous/winpoint.cpp
    )

    set(HEADER_FILES ${HEADER_FILES}
            include/control/autonomous/autonomous.hpp
            include/control/autonomous/score.hpp
            include/control/autonomous/skills.hpp
            include/control/autonomous/winpoint.hpp
    )
endif()

add_executable(ColdPackage)
target_sources(ColdPackage PRIVATE ${CMAKE_BINARY_DIR}/empty.cpp)

add_executable(HotPackage)
target_sources(HotPackage PRIVATE ${SOURCE_FILES})
target_sources(HotPackage PUBLIC FILE_SET headers TYPE HEADERS FILES ${HEADER_FILES})

add_executable(MonolithPackage)
target_sources(MonolithPackage PRIVATE ${SOURCE_FILES})
target_sources(MonolithPackage PUBLIC FILE_SET headers TYPE HEADERS FILES ${HEADER_FILES})

if (ENABLE_LTO)
    message(NOTICE "LTO is enabled")
    set_property(TARGET HotPackage PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET MonolithPackage PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_include_directories(HotPackage PRIVATE include)
target_link_directories(HotPackage PRIVATE firmware)
target_include_directories(MonolithPackage PRIVATE include)
target_link_directories(MonolithPackage PRIVATE firmware)

if (ProsCLI)
    add_custom_target(Upload DEPENDS HotPackage COMMAND ${ProsCLI} upload --slot 1)
endif()

if (ClangFormat)
    add_custom_target(FormatCode COMMAND ${ClangFormat} -i ${HEADER_FILES} ${SOURCE_FILES} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMENT "Formatting code")
endif()

if (ClangTidy)
    execute_process(COMMAND ${TARGET_TRIPLE}-g++ -print-sysroot OUTPUT_VARIABLE CMAKE_SYSROOT OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${TARGET_TRIPLE}-g++ -print-multi-directory OUTPUT_VARIABLE ARM_MULTI_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${TARGET_TRIPLE}-g++ -dumpversion OUTPUT_VARIABLE ARM_GCC_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)

    add_custom_target(Tidy COMMAND ${ClangTidy} ${HEADER_FILES} ${SOURCE_FILES} --checks=-*,bugprone-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,performance-*,readability-*,-bugprone-easily-swappable-parameters,-readability-function-cognitive-complexity,llvm-header-guard,-readability-identifier-length,-bugprone-macro-parentheses,-modernize-use-trailing-return-type,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers,-cppcoreguidelines-avoid-non-const-global-variables,-cppcoreguidelines-pro-bounds-constant-array-index -- -I${CMAKE_SOURCE_DIR}/include ${CMAKE_CXX_FLAGS} -std=c++20 -Wall -mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=softfp -nostdlib -target ${TARGET_TRIPLE} -isystem${CMAKE_SYSROOT}/include/c++/${ARM_GCC_VERSION} -isystem${CMAKE_SYSROOT}/include/c++/${ARM_GCC_VERSION}/${TARGET_TRIPLE}/${ARM_MULTI_DIR} -isystem${CMAKE_SYSROOT}/include/ WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMENT "Checking code")
endif()

add_dependencies(HotPackage ColdPackage)

target_link_libraries(HotPackage PUBLIC ${ColdLibraries})
target_link_libraries(MonolithPackage PUBLIC ${ColdLibraries})

set_property(TARGET MonolithPackage PROPERTY LINK_DEPENDS ${FIRMWARE_DIR}/v5-common.ld ${FIRMWARE_DIR}/v5.ld)
set_property(TARGET HotPackage PROPERTY LINK_DEPENDS ${FIRMWARE_DIR}/v5-common.ld ${FIRMWARE_DIR}/v5-hot.ld)
set_property(TARGET ColdPackage PROPERTY LINK_DEPENDS ${FIRMWARE_DIR}/v5-common.ld ${FIRMWARE_DIR}/v5.ld)

target_link_options(HotPackage PRIVATE LINKER:-R,$<TARGET_FILE:ColdPackage> -nostartfiles LINKER:-T${FIRMWARE_DIR}/v5-hot.ld,--gc-sections,--start-group,${HotLinked},-lgcc,-lstdc++,--end-group,-T${FIRMWARE_DIR}/v5-common.ld)
target_link_options(ColdPackage PRIVATE LINKER:--gc-keep-exported,--whole-archive,${ExportedLibraries},-lstdc++,--no-whole-archive,-T${FIRMWARE_DIR}/v5.ld,--gc-sections,--start-group,${HotLinked},-lgcc,-lstdc++,--end-group,-T${FIRMWARE_DIR}/v5-common.ld)
target_link_options(MonolithPackage PRIVATE LINKER:-T${FIRMWARE_DIR}/v5.ld,--gc-sections,--whole-archive,-lstdc++,--no-whole-archive,--start-group,${HotLinked},-lgcc,--end-group,-T${FIRMWARE_DIR}/v5-common.ld)

# Strip packages after build
add_custom_command(TARGET ColdPackage POST_BUILD COMMAND ${CMAKE_OBJCOPY} --strip-symbol=install_hot_table --strip-symbol=__libc_init_array $<TARGET_FILE:ColdPackage>) # Must be separate - hot package depends on this
add_custom_command(TARGET ColdPackage POST_BUILD COMMAND ${CMAKE_OBJCOPY} -O binary -R .hot_init $<TARGET_FILE:ColdPackage> ${BIN_DIR}/cold.package.bin BYPRODUCTS ${BIN_DIR}/cold.package.bin)
add_custom_command(TARGET HotPackage POST_BUILD COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:HotPackage> ${BIN_DIR}/hot.package.bin DEPENDS $<TARGET_FILE:HotPackage> BYPRODUCTS ${BIN_DIR}/hot.package.bin)
add_custom_command(TARGET MonolithPackage POST_BUILD COMMAND ${CMAKE_OBJCOPY} -O binary -R .hot_init $<TARGET_FILE:MonolithPackage> ${BIN_DIR}/monolith.bin DEPENDS $<TARGET_FILE:MonolithPackage> BYPRODUCTS ${BIN_DIR}/monolith.bin)
