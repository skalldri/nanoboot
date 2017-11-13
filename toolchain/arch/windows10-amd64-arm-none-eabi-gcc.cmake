# ensure we only get used AFTER someone sets a device string
if(NOT (DEFINED NANOBOOT_MCU))
    message(FATAL_ERROR "You must set a NANOBOOT_MCU before including this toolchain file")
endif()

if(NOT (NANOBOOT_MCU_ARCH STREQUAL arm))
    message(FATAL_ERROR "NANOBOOT_MCU_ARCH must be arm to use the arm-none-eabi-gcc compiler")
endif()

# this one is important
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# escape windows paths as needed
file(TO_CMAKE_PATH $ENV{GCC_ROOT} GCC_ROOT)

set(GCC_BIN_DIR ${GCC_ROOT}/bin)

# specify the cross compiler
# TODO: use find_program instead of hard paths
set(CMAKE_C_COMPILER   ${GCC_BIN_DIR}/arm-none-eabi-gcc.exe CACHE INTERNAL "GCC")
set(CMAKE_CXX_COMPILER ${GCC_BIN_DIR}/arm-none-eabi-g++.exe CACHE INTERNAL "G++")
set(CMAKE_ASM_COMPILER ${GCC_BIN_DIR}/arm-none-eabi-gcc.exe CACHE INTERNAL "ASM")
set(CMAKE_OBJCOPY      ${GCC_BIN_DIR}/arm-none-eabi-objcopy.exe CACHE INTERNAL "Objcopy")
set(CMAKE_OBJDUMP      ${GCC_BIN_DIR}/arm-none-eabi-objdump.exe CACHE INTERNAL "Objdump")
set(CMAKE_OBJSIZE      ${GCC_BIN_DIR}/arm-none-eabi-size.exe CACHE INTERNAL "Objsize")

# where is the target environment 
set(CMAKE_FIND_ROOT_PATH $ENV{NANOBOOT_ROOT})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# output executable extension
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

# when Cmake tries to build the test library, make it a static library 
# (compiling an ELF would be hard for this platform since it needs a linker script)
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(NANOBOOT_COMPILE_FLAGS
    ${NANOBOOT_COMPILE_FLAGS}
    "-mthumb"
    "-mcpu=cortex-m4"
    "-mfloat-abi=hard"
    "-mfpu=fpv4-sp-d16"
)

set(NANOBOOT_LINK_FLAGS
    ${NANOBOOT_LINK_FLAGS}
    "-mthumb"
    "-mcpu=cortex-m4"
    "-mfloat-abi=hard"
    "-mfpu=fpv4-sp-d16"
)