#pragma once

// This file defines a set of constants that identify the flash properties of a device
// These symbols are device specific:
//
// - FLASH_SIZE : the total size of the device flash
// - MAX_SIZE_1BOOT : the maximum size of a 1BOOT image
// - START_ADDRESS_1BOOT : the start address for 1BOOT (normally 0x00000000 on Cortex-M)
// - MAX_SIZE_2BOOT : the maximum size of a 2BOOT image
// - START_ADDRESS_2BOOT : the start address for 2BOOT (normally 1BOOT_START_ADDRESS + 1BOOT_MAX_SIZE)
// - APP_MAX_SIZE : the maximum size of an APP image
// - APP_START_ADDRESS : the maximum size of an APP image
//
// This file also defines a set of simple functions which must be implemented for each platform


#if defined(NRF52832_XXAA)

    #define FLASH_SIZE              0x80000
    #define MAX_SIZE_1BOOT          0x1000
    #define START_ADDRESS_1BOOT     0x00000000
    #define MAX_SIZE_2BOOT          0x1000
    #define START_ADDRESS_2BOOT     (START_ADDRESS_1BOOT + MAX_SIZE_1BOOT)
    #define APP_MAX_SIZE            ((FLASH_SIZE - MAX_SIZE_1BOOT - MAX_SIZE_2BOOT) / 2)
    #define APP_START_ADDRESS       (2BOOT_START_ADDRESS + 2BOOT_MAX_SIZE)

// Add more devices here
#else
    #error "No device defined"
#endif