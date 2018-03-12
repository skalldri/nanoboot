#include <flash_layout.h>

static const MetadataHeader nanobootHeader __attribute__((used)) __attribute__ ((section (".header"))) = {
    .crc32              = DEBUG_MAGIC_NUMBER,       // Will be inserted by post-processing
    .imageSize          = DEBUG_MAGIC_NUMBER,       // Will be inserted by post-processing
    .platformId         = NANOBOOT_PLATFORM,        // Defined by the MCU Cmake definition
    .sector             = ImageSector::TWO_BOOT,
    .magicNumber        = HEADER_MAGIC_NUMBER,
    .majorVersion       = NANOBOOT_MAJOR,
    .minorVersion       = NANOBOOT_MINOR,
    .revision           = NANOBOOT_REV,
    .dc0Size            = 0,                        // No DC0 section in this image
    .ivtSize            = 512,                      // NRF52 uses 512 bytes for interrupt vector table
                                                    // TODO: investigate shrinking this
    .dc1Size            = 0,                        // No DC1 section in this image
};
