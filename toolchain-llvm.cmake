set(TARGET_TRIPLE arm-none-eabi)

add_compile_options(-target ${TARGET_TRIPLE} -Wno-unused-command-line-argument)
add_link_options(-target ${TARGET_TRIPLE} -Wno-unused-command-line-argument)

add_link_options("-fuse-ld=bfd")

execute_process(COMMAND ${TARGET_TRIPLE}-g++ -print-sysroot OUTPUT_VARIABLE CMAKE_SYSROOT OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${TARGET_TRIPLE}-g++ -print-multi-directory OUTPUT_VARIABLE ARM_MULTI_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${TARGET_TRIPLE}-g++ -print-libgcc-file-name OUTPUT_VARIABLE ARM_LIB_GCC OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${TARGET_TRIPLE}-g++ -print-file-name=libstdc++.a OUTPUT_VARIABLE ARM_LIB_STDCPP OUTPUT_STRIP_TRAILING_WHITESPACE)
get_filename_component(ARM_LIBS_GCC ${ARM_LIB_GCC} DIRECTORY)
get_filename_component(ARM_LIBS_C ${ARM_LIB_STDCPP} DIRECTORY)
execute_process(COMMAND ${TARGET_TRIPLE}-g++ -dumpversion OUTPUT_VARIABLE ARM_GCC_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories(SYSTEM $<$<COMPILE_LANGUAGE:CXX>:${CMAKE_SYSROOT}/include/c++/${ARM_GCC_VERSION} ${CMAKE_SYSROOT}/include/c++/${ARM_GCC_VERSION}/${TARGET_TRIPLE}/${ARM_MULTI_DIR}>)
link_directories(${ARM_LIBS_GCC} ${ARM_LIBS_C})

set(CMAKE_AR llvm-ar)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_STRIP llvm-strip)
set(CMAKE_OBJCOPY llvm-objcopy)
set(CMAKE_CXX_COMPILER_ID Clang)
set(CMAKE_LINKER clang++)

option(ENABLE_REMARKS "Enable compiler optimization remarks" OFF)

if (ENABLE_REMARKS)
        add_compile_options(-fsave-optimization-record)
endif()

if (ENABLE_LTO)
    # LTO requires lld (which does not work with the linker script)
    message(SEND_ERROR "LTO does not work with clang")
endif()
