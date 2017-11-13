# ensure we only get used AFTER someone sets a device string
if(NOT (DEFINED NANOBOOT_MCU))
    message(FATAL_ERROR "You must set a NANOBOOT_MCU before including this toolchain file")
endif()

if(NOT (NANOBOOT_MCU_ARCH STREQUAL avr))
    message(FATAL_ERROR "NANOBOOT_MCU_ARCH must be avr to use the avr-gcc compiler")
endif()

# this one is important
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

# escape windows paths as needed
file(TO_CMAKE_PATH $ENV{NUGET_PACKAGES_ROOT} NUGET_PACKAGES_ROOT)

set(GCC_BIN_DIR ${NUGET_PACKAGES_ROOT}/autom8ed.com.compilers.avr-gcc/bin)

# AVR-GCC requires all these paths as part of the environment path...
set(ENV{PATH} "$ENV{PATH};${NUGET_PACKAGES_ROOT}\\autom8ed.com.compilers.avr-gcc\\bin\\;")
set(ENV{PATH} "$ENV{PATH};${NUGET_PACKAGES_ROOT}\\autom8ed.com.compilers.avr-gcc\\avr\\bin\\;")
set(ENV{PATH} "$ENV{PATH};${NUGET_PACKAGES_ROOT}\\autom8ed.com.compilers.avr-gcc\\libexec\\gcc\\avr\\4.9.2\\;")
set(ENV{PATH} "$ENV{PATH};${NUGET_PACKAGES_ROOT}\\autom8ed.com.compilers.avr-gcc\\libexec\\gcc\\avr\\4.9.2\\install-tools\\;")

# specify the cross compiler
# TODO: use find_program instead of hard paths
set(CMAKE_C_COMPILER   ${GCC_BIN_DIR}/avr-gcc.exe CACHE INTERNAL "GCC")
set(CMAKE_CXX_COMPILER ${GCC_BIN_DIR}/avr-g++.exe CACHE INTERNAL "G++")
set(CMAKE_ASM_COMPILER ${GCC_BIN_DIR}/avr-gcc.exe CACHE INTERNAL "ASM")
set(CMAKE_OBJCOPY      ${GCC_BIN_DIR}/avr-objcopy.exe CACHE INTERNAL "Objcopy")
set(CMAKE_OBJDUMP      ${GCC_BIN_DIR}/avr-objdump.exe CACHE INTERNAL "Objdump")
set(CMAKE_OBJSIZE      ${GCC_BIN_DIR}/avr-size.exe CACHE INTERNAL "Objsize")

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
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)