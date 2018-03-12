#pragma once

#include <flash_layout.h>

enum NANO_ERROR {
    E_OK = 0,
    E_INVALID_CRC,
    E_INVALID_LENGTH,
    E_CRC_MISMATCH,
    E_NO_MAGIC,
    E_UNALIGNED_ACCESS,
    E_OVERFLOW,
    E_UNDERFLOW,
};

enum NANO_ACTION {
    CONTINUE_BOOT,
    BEGIN_RECOVERY,
    BEGIN_UPGRADE,
    FATAL_ERROR
};

enum NANO_COMPARE {
    FIRST_IS_GREATER,
    EQUAL,
    SECOND_IS_GREATER,
    UNABLE_TO_COMPARE
};

void StartImageFromFlash(MetadataHeader* image);
NANO_COMPARE ImageVersionCompare(MetadataHeader* first, MetadataHeader* second);