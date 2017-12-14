#pragma once
#include <cstdint>
#include <flash_layout.h>

enum NANO_ERROR {
    E_OK = 0,
    E_INVALID_CRC = 1,
    E_INVALID_LENGTH = 2,
    E_CRC_MISMATCH = 3,
    E_NO_MAGIC = 4,
};

NANO_ERROR TestCRC();
NANO_ERROR ValidateImage(MetadataHeader* image);
uint32_t crc32(uint32_t *message, uint32_t size);
