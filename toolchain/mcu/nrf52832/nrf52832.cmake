# escape windows paths as needed
file(TO_CMAKE_PATH $ENV{NUGET_PACKAGES_ROOT} NUGET_PACKAGES_ROOT)
file(TO_CMAKE_PATH $ENV{NANOBOOT_ROOT} NANOBOOT_ROOT)

# Set global nanoboot variables needed for all platforms
set(NANOBOOT_MCU nrf52832)
set(NANOBOOT_MCU_ARCH arm)

# define that we're using an NRF52**AA type chip (512K flash, 64k RAM)
set(NANOBOOT_DEFINES 
    ${NANOBOOT_DEFINES}
    NRF52832_XXAA
)

# Add include paths for the Nordic SDK
set(NANOBOOT_INCLUDES
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/device
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/drivers_nrf/hal
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/drivers_nrf/delay
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/libraries/util
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/drivers_nrf/nrf_soc_nosd
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/config
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/toolchain
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/toolchain/gcc
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/toolchain/cmsis/include
)

set(NANOBOOT_SOURCES
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/toolchain/system_nrf52.c
    ${NANOBOOT_ROOT}/sdks/nordic/nRF5_SDK_14.0.0_3bcc1f7/components/toolchain/gcc/gcc_startup_nrf52.S
)

# include the AVR compiler definitions
# TODO: allow different OS toolchains
include(${NANOBOOT_ROOT}/toolchain/arch/windows10-amd64-arm-none-eabi-gcc.cmake)
