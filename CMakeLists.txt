cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
set(CMAKE_EXPORT_COMPILE_COMMANDS)

project(nanoboot VERSION 0.0.1 LANGUAGES C CXX ASM)

MESSAGE("MCU: ${NANOBOOT_MCU}")
MESSAGE("MCU ARCH: ${NANOBOOT_MCU_ARCH}")

# set the root nanoboot folder so other projects can use this as a starting point
set(NANOBOOT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# root of the TESTAPP project
set(TESTAPP_ROOT ${NANOBOOT_ROOT}/code/testapp)
set(1BOOT_ROOT ${NANOBOOT_ROOT}/code/1BOOT)
set(2BOOT_ROOT ${NANOBOOT_ROOT}/code/2BOOT)

# search path for GCC LD files
set(MCU_TOOLCHAIN ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/toolchain)

# search path for GCC LD files
set(GCC_MCU_TOOLCHAIN ${MCU_TOOLCHAIN}/gcc)

# search path for NRF52 include files
set(NRF52_INCLUDE 
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/device
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/drivers_nrf/hal
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/drivers_nrf/delay
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/libraries/util
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/drivers_nrf/nrf_soc_nosd
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/config
    ${MCU_TOOLCHAIN}/cmsis/include
    ${MCU_TOOLCHAIN}
)

# define that we're using an NRF52**AA type chip (512K flash, 64k RAM)
set(DEVICE_TARGET NRF52832_XXAA)

# TODO: uncomment this when 1BOOT and 2BOOT have more meat
add_subdirectory(${1BOOT_ROOT} 1BOOT)
#add_subdirectory(code/2BOOT 2BOOT)
add_subdirectory(${TESTAPP_ROOT} testapp)
