cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
set(CMAKE_EXPORT_COMPILE_COMMANDS)

project(nanoboot VERSION 0.1.0 LANGUAGES C CXX ASM)

MESSAGE("MCU: ${NANOBOOT_MCU}")
MESSAGE("MCU ARCH: ${NANOBOOT_MCU_ARCH}")

# set the root nanoboot folder so other projects can use this as a starting point
set(NANOBOOT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# root of the TESTAPP project
set(COMMON_ROOT ${NANOBOOT_ROOT}/code/common)
set(TESTAPP_ROOT ${NANOBOOT_ROOT}/code/testapp)
set(1BOOT_ROOT ${NANOBOOT_ROOT}/code/1BOOT)
set(2BOOT_ROOT ${NANOBOOT_ROOT}/code/2BOOT)

# add the nanoboot HAL and the device-specific HAL
set(NANOBOOT_SOURCES 
    ${NANOBOOT_SOURCES}
    ${NANOBOOT_ROOT}/code/hal/${NANOBOOT_MCU}/src/gpio.cpp
    ${NANOBOOT_ROOT}/code/hal/${NANOBOOT_MCU}/src/util.cpp
    ${NANOBOOT_ROOT}/code/hal/${NANOBOOT_MCU}/src/flash.cpp
)

set(NANOBOOT_INCLUDES
    ${NANOBOOT_INCLUDES}
    ${NANOBOOT_ROOT}/code/hal/base/inc
    ${NANOBOOT_ROOT}/code/common/inc
    ${NANOBOOT_ROOT}/code/hal/${NANOBOOT_MCU}/inc
)

set(NANOBOOT_DEFINES
    ${NANOBOOT_DEFINES}
    NANOBOOT_MCU=${NANOBOOT_MCU}
    NANOBOOT_MCU_ARCH=${NANOBOOT_MCU_ARCH}
    NANOBOOT_PLATFORM=${NANOBOOT_PLATFORM}
    NANOBOOT_MAJOR=${PROJECT_VERSION_MAJOR}
    NANOBOOT_MINOR=${PROJECT_VERSION_MINOR}
    NANOBOOT_REV=${PROJECT_VERSION_PATCH}
)

set(NANOBOOT_LINK_FLAGS
    ${NANOBOOT_LINK_FLAGS}
    -L${NANOBOOT_ROOT}/code/hal/${NANOBOOT_MCU}/lib
)

add_subdirectory(${1BOOT_ROOT} 1BOOT)
add_subdirectory(${2BOOT_ROOT} 2BOOT)
add_subdirectory(${TESTAPP_ROOT} testapp)
