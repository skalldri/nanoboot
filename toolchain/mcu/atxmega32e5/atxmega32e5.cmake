# escape windows paths as needed
file(TO_CMAKE_PATH $ENV{NUGET_PACKAGES_ROOT} NUGET_PACKAGES_ROOT)
file(TO_CMAKE_PATH $ENV{NANOBOOT_ROOT} NANOBOOT_ROOT)

# Set global nanoboot variables needed for all platforms
set(NANOBOOT_MCU atxmega32e5)
set(NANOBOOT_MCU_ARCH avr)

# Add include paths for the AVR SDK
set(NANOBOOT_INCLUDES

)

set(NANOBOOT_SOURCES

)

# include the AVR compiler definitions
# TODO: allow different OS toolchains
include(${NANOBOOT_ROOT}/toolchain/arch/windows10-amd64-avr-gcc.cmake)
