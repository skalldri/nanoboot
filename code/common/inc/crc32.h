#pragma once
#include <cstdint>
#include <flash_layout.h>
#include <utils.h>

NANO_ERROR TestCRC();
NANO_ERROR ValidateImage(MetadataHeader* image);
uint32_t crc32(uint32_t *message, uint32_t size);
