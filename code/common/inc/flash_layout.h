#pragma once

#include <cstdint>
#include <sys\param.h>
#include "assert.h"

#if (!defined(NANOBOOT_PLATFORM))
    #error "NANOBOOT_PLATFORM is not defined. Please define a value for NANOBOOT_PLATFORM in the MCU's CMAKE file"
#endif

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

// The metadata header which is detected by nanoboot to identify valid images
#pragma pack(push, 1)
struct MetadataHeader {
    uint32_t crc32;
    uint32_t imageSize;
    uint32_t platformId : 28;
    uint32_t sector : 4;
    uint32_t magicNumber;
    uint32_t majorVersion;
    uint32_t minorVersion;
    uint32_t revision;
    uint32_t dc0Size;
    uint32_t ivtSize;
    uint32_t dc1Size;
    uint32_t reserved[22];
};
#pragma pack(pop)
static_assert(sizeof(MetadataHeader) == 128, "MetadataHeader must be 128 bytes");

#define HEADER_MAGIC_NUMBER 0x4F4E414E

enum ImageSector {
    ZERO_RESERVED = 0,
    ONE_BOOT      = 1,
    TWO_BOOT      = 2,
    APP           = 3,
    MAX_RESERVED  = 0xF
};

// The interrupt vector table which is common to all Cortex-M devices
struct VectorTableCommon {
    uint32_t stack;
    uint32_t resetHandler;                      // 1
    uint32_t nmiHandler;                        // 2
    uint32_t hardFaultHandler;                  // 3
    uint32_t memoryManagerFaultHandler;         // 4
    uint32_t busFaultHandler;                   // 5
    uint32_t usageFaultHandler;                 // 6
    uint32_t reserved[4];                       // 7, 8, 9, 10
    uint32_t svCallHandler;                     // 11
    uint32_t debugHandler;                      // 12
    uint32_t reserved2;                         // 13
    uint32_t pendSvHandler;                     // 14
    uint32_t systickHandler;                    // 15
};

extern int __FLASH_START_ADDRESS;
extern int __FLASH_SIZE;
extern int __RAM_START_ADDRESS;
extern int __RAM_SIZE;
extern int __MAX_SIZE_1BOOT;
extern int __START_ADDRESS_1BOOT;
extern int __MAX_SIZE_2BOOT;
extern int __START_ADDRESS_2BOOT;
extern int __MAX_SIZE_APP;
extern int __START_ADDRESS_APP;
extern int __MAX_SIZE_SCRATCH;
extern int __START_ADDRESS_SCRATCH;

#define FLASH_START_ADDRESS     ((uint32_t)(&__FLASH_START_ADDRESS))
#define FLASH_SIZE              ((uint32_t)(&__FLASH_SIZE))

#define MAX_SIZE_1BOOT          ((uint32_t)(&__MAX_SIZE_1BOOT))
#define START_ADDRESS_1BOOT     ((uint32_t)(&__START_ADDRESS_1BOOT))

#define MAX_SIZE_2BOOT          ((uint32_t)(&__MAX_SIZE_2BOOT))
#define START_ADDRESS_2BOOT     ((uint32_t)(&__START_ADDRESS_2BOOT))

#define MAX_SIZE_APP            ((uint32_t)(&__MAX_SIZE_APP))
#define START_ADDRESS_APP       ((uint32_t)(&__START_ADDRESS_APP))

#define MAX_SIZE_SCRATCH        ((uint32_t)(&__MAX_SIZE_SCRATCH))
#define START_ADDRESS_SCRATCH   ((uint32_t)(&__START_ADDRESS_SCRATCH))

#define IMAGE_MAX_SIZE          (MAX(MAX_SIZE_2BOOT, MAX_SIZE_APP))
#define IMAGE_MIN_SIZE          (sizeof(MetadataHeader))
