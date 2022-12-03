cmake_minimum_required(VERSION 3.20)
project(VexV5 VERSION 1.0.0 LANGUAGES C CXX)
include(toolchain.cmake)

set(FIRMWARE_DIR ${CMAKE_SOURCE_DIR}/firmware)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

include_directories(include)

file(TOUCH ${BUILD_DIR}/empty.cpp)
file(WRITE ${BUILD_DIR}/timestamp.cpp "char const * const _PROS_COMPILE_TIMESTAMP = __DATE__ \" \" __TIME__; char const * const _PROS_COMPILE_DIRECTORY = \"${CMAKE_SOURCE_DIR}\";\n")
add_library(Timestamp ${BUILD_DIR}/timestamp.cpp)
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS include/*.hpp)
list(FILTER HEADER_FILES EXCLUDE REGEX "(include/pros/*|include/display/*)")

add_executable(ColdPackage ${BUILD_DIR}/empty.cpp)
add_executable(
        HotPackage
        ${HEADER_FILES}
        ${SOURCE_FILES}
        $<TARGET_OBJECTS:Timestamp>
)


if (ProsCLI)
add_custom_target(
        Upload
        DEPENDS HotPackage
        COMMAND ${ProsCLI} upload
)
endif()

if (ClangFormat)
message(NOTICE "Clang-format found, adding utility target.")
add_custom_target(
        FormatCode
        COMMAND cd ${CMAKE_SOURCE_DIR} && ${ClangFormat} -i ${HEADER_FILES} ${SOURCE_FILES}
)
endif()

add_dependencies(HotPackage ColdPackage)

target_compile_options(Timestamp PRIVATE -Wno-unused-const-variable -Wno-reserved-identifier -Wno-date-time)

target_link_libraries(HotPackage PUBLIC ${LibC} ${LibM} ${LibPROS} ${LibBase85})

set_property(TARGET HotPackage PROPERTY LINK_DEPENDS ${FIRMWARE_DIR}/v5-common.ld ${FIRMWARE_DIR}/v5-hot.ld)
set_property(TARGET ColdPackage PROPERTY LINK_DEPENDS ${FIRMWARE_DIR}/v5-common.ld ${FIRMWARE_DIR}/v5.ld)

target_link_options(
        HotPackage
        PUBLIC -Wl,-R,$<TARGET_FILE:ColdPackage>
        PUBLIC -nostartfiles -Wl,-T${FIRMWARE_DIR}/v5-hot.ld,--gc-sections,--start-group,${LibC},${LibM},${LibPROS},${LibBase85},-lgcc,-lstdc++,--end-group,-T${FIRMWARE_DIR}/v5-common.ld
)

target_link_options(
        ColdPackage
        PUBLIC -Wl,--gc-keep-exported,--whole-archive,${LibPROS},-lstdc++,--no-whole-archive
        PUBLIC -Wl,-T${FIRMWARE_DIR}/v5.ld,--gc-sections,--start-group,${LibC},${LibM},${LibPROS},${LibBase85},-lgcc,-lstdc++,--end-group,-T${FIRMWARE_DIR}/v5-common.ld
)

add_custom_command(
        TARGET ColdPackage
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} --strip-symbol=install_hot_table --strip-symbol=__libc_init_array --strip-symbol=_PROS_COMPILE_DIRECTORY --strip-symbol=_PROS_COMPILE_TIMESTAMP --strip-symbol=_PROS_COMPILE_TIMESTAMP_INT $<TARGET_FILE:ColdPackage>
)

add_custom_command(
        TARGET ColdPackage
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary -R .hot_init $<TARGET_FILE:ColdPackage> ${BIN_DIR}/cold.package.bin
)

add_custom_command(
        TARGET HotPackage
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:HotPackage> ${BIN_DIR}/hot.package.bin
)

add_custom_command(
        TARGET HotPackage
        POST_BUILD
        COMMAND ${CMAKE_STRIP} --strip-symbol=_PROS_COMPILE_DIRECTORY --strip-symbol=_PROS_COMPILE_TIMESTAMP --strip-symbol=_PROS_COMPILE_TIMESTAMP_INT $<TARGET_FILE:HotPackage>
)
