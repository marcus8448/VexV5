set(TARGET_TRIPLE arm-none-eabi)

set(CMAKE_AR ${TARGET_TRIPLE}-ar)
set(CMAKE_C_COMPILER ${TARGET_TRIPLE}-gcc)
set(CMAKE_CXX_COMPILER ${TARGET_TRIPLE}-g++)
set(CMAKE_STRIP ${TARGET_TRIPLE}-strip)
set(CMAKE_OBJCOPY ${TARGET_TRIPLE}-objcopy)
set(CMAKE_CXX_COMPILER_ID GNU)

set(CMAKE_LINKER ${CMAKE_CXX_COMPILER})
set(LINKER bfd)
