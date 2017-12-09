#pragma once
#include <cstdint>
#include "assert.h"

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

struct MetadataHeader {
    uint32_t crc32;
    uint32_t platformId : 28;
    uint32_t sector : 4;
    uint32_t imageSize;
    uint32_t majorVersion;
    uint32_t minorVersion;
    uint32_t revision;
    uint32_t dc0StartAddress;
    uint32_t dc0Size;
    uint32_t ivtStartAddress;
    uint32_t ivtSize;
    uint32_t dc1StartAddress;
    uint32_t dc1Size;
    uint32_t appStartAddress;
    uint32_t appSize;
    uint32_t appEntryPoint;
    uint32_t reserved[17];
};

static_assert(sizeof(MetadataHeader) == 128, "MetadataHeader must be 128 bytes");

extern int __FLASH_START_ADDRESS;
extern int __FLASH_SIZE;
extern int __RAM_START_ADDRESS;
extern int __RAM_SIZE;
extern int __MAX_SIZE_1BOOT;
extern int __START_ADDRESS_1BOOT;
extern int __MAX_SIZE_2BOOT;

#define FLASH_START_ADDRESS     ((uint32_t)(&__FLASH_START_ADDRESS))
#define FLASH_SIZE              ((uint32_t)(&__FLASH_SIZE))
#define MAX_SIZE_1BOOT          ((uint32_t)(&__MAX_SIZE_1BOOT))
#define START_ADDRESS_1BOOT     ((uint32_t)(&__START_ADDRESS_1BOOT))
#define MAX_SIZE_2BOOT          ((uint32_t)(&__MAX_SIZE_2BOOT))
#define START_ADDRESS_2BOOT     (START_ADDRESS_1BOOT + MAX_SIZE_1BOOT)
#define APP_MAX_SIZE            ((FLASH_SIZE - MAX_SIZE_1BOOT - MAX_SIZE_2BOOT) / 2)
#define APP_START_ADDRESS       (2BOOT_START_ADDRESS + 2BOOT_MAX_SIZE)
