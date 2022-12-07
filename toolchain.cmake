set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(TARGET_TRIPLE arm-none-eabi)

set(CMAKE_C_COMPILER_TARGET ${TARGET_TRIPLE})
set(CMAKE_CXX_COMPILER_TARGET ${TARGET_TRIPLE})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MFLAGS "-mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=softfp -g")
set(CFLAGS "-D_POSIX_THREADS -D_UNIX98_THREAD_MUTEX_ATTRIBUTES")
set(WARNFLAGS "-Wall -Wno-psabi -Wno-shadow-field-in-constructor")
set(GCCFLAGS "-ffunction-sections -fdata-sections -fdiagnostics-color -funwind-tables")

execute_process(COMMAND ${TARGET_TRIPLE}-g++ -print-sysroot OUTPUT_VARIABLE ARM_SYSROOT OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${TARGET_TRIPLE}-g++ -print-multi-directory OUTPUT_VARIABLE ARM_MULTI_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${TARGET_TRIPLE}-g++ -print-libgcc-file-name OUTPUT_VARIABLE ARM_LIB_GCC OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND dirname ${ARM_LIB_GCC} OUTPUT_VARIABLE ARM_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${TARGET_TRIPLE}-g++ -dumpversion OUTPUT_VARIABLE ARM_GCC_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)

set(CMAKE_CXX_FLAGS "${MFLAGS} ${CFLAGS} ${WARNFLAGS} ${GCCFLAGS}")
set(CMAKE_C_FLAGS "${MFLAGS} ${CFLAGS} ${WARNFLAGS} ${GCCFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -Wl,--no-enum-size-warning -z noexecstack")

if (ARM_SYSROOT STREQUAL "" OR TRUE)
    set(CMAKE_AR ${TARGET_TRIPLE}-ar)
    set(CMAKE_C_COMPILER ${TARGET_TRIPLE}-gcc)
    set(CMAKE_CXX_COMPILER ${TARGET_TRIPLE}-g++)
    set(CMAKE_STRIP ${TARGET_TRIPLE}-strip)
    set(CMAKE_OBJCOPY ${TARGET_TRIPLE}-objcopy)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
else()
    set(CLANG_TARGET "-target ${TARGET_TRIPLE} --sysroot ${ARM_SYSROOT}")
    set(CLANG_CXX_TARGET "${CLANG_TARGET}")

    set(CMAKE_AR llvm-ar)
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
    set(CMAKE_STRIP llvm-strip)
    set(CMAKE_OBJCOPY llvm-objcopy)
    set(CMAKE_C_LINK_FLAGS "-fuse-ld=bfd ${CLANG_TARGET}")
    set(CMAKE_CXX_LINK_FLAGS "-fuse-ld=bfd ${CLANG_CXX_TARGET}")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --start-no-unused-arguments")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_CXX_TARGET}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CLANG_TARGET}")
    link_directories(${ARM_LIBS})
    include_directories(SYSTEM $<$<COMPILE_LANGUAGE:CXX>:${ARM_SYSROOT}/include/c++/${ARM_GCC_VERSION} ${ARM_SYSROOT}/include/c++/${ARM_GCC_VERSION}/${TARGET_TRIPLE}/${ARM_MULTI_DIR}>)
endif()

set(CMAKE_LINKER ${CMAKE_CXX_COMPILER})

find_library(LibC NAMES c PATHS ${CMAKE_SOURCE_DIR}/firmware NO_DEFAULT_PATH REQUIRED)
find_library(LibM NAMES m PATHS ${CMAKE_SOURCE_DIR}/firmware NO_DEFAULT_PATH REQUIRED)
find_library(LibPROS NAMES pros PATHS ${CMAKE_SOURCE_DIR}/firmware NO_DEFAULT_PATH REQUIRED)
find_library(LibBase85 NAMES base85 PATHS ${CMAKE_SOURCE_DIR}/firmware NO_DEFAULT_PATH REQUIRED)

find_program(ProsCLI pros)
find_program(ClangFormat clang-format)
find_program(ClangCheck clang-check)

set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
