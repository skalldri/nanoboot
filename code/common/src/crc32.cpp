#include <crc32.h>
#include <cstdint>
#include "flash_layout.h"

// These routines are adopted from the CRC32 implementation in "The Hackers Delight".
// They may be freely reproduced and modified without attribution.
// http://www.hackersdelight.org/hdcodetxt/crc.c.txt

// Validated against http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
// with default settings. Must correct input byte pattern for ARM's little-endian byte ordering.
uint32_t crc32(uint32_t *message, uint32_t size) {
   uint32_t crc, mask;
   static uint32_t table[256];

   /* Set up the table, if necessary. */

   if (table[1] == 0) {
      for (uint16_t byte = 0; byte <= 255; byte++) {
         crc = byte;
         for (uint8_t j = 0; j < 8; ++j) {    // Do eight times.
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
         }
         table[byte] = crc;
      }
   }

   /* Through with table setup, now calculate the CRC. */

   crc = 0xFFFFFFFF;
   for (uint32_t position = 0; position < (size / 4); position++)
   {
      crc = crc ^ message[position];
      crc = (crc >> 8) ^ table[crc & 0xFF];
      crc = (crc >> 8) ^ table[crc & 0xFF];
      crc = (crc >> 8) ^ table[crc & 0xFF];
      crc = (crc >> 8) ^ table[crc & 0xFF];
   }

   return ~crc;
}

// This function tests that the CRC32 algorithm is working correctly.
// It can be used to unit-test the CRC32 implementation, should it ever get swapped out
NANO_ERROR TestCRC()
{
    uint32_t testBlob = 0xDEADBEEF;
    
    // Testing out CRC32 implementation
    uint32_t crc = crc32(&testBlob, sizeof(uint32_t));

    // Known CRC32 value
    if (crc != 0x1A5A601F)
    {
        return E_CRC_MISMATCH;
    }

    return E_OK;
}

// Function which will perform integrity checks on a chunk of memory.
// Returns E_OK if the image passes integrity checks
// Returns other NANO_ERROR codes if the image does not pass validation
NANO_ERROR ValidateImage(MetadataHeader* image)
{
    // Is our magic value present? Bail quickly if no.
    if (image->magicNumber != HEADER_MAGIC_NUMBER)
    {
        return E_NO_MAGIC;
    }

    // Verify the CRC is not all 0x00 or all 0xFF
    if (image->crc32 == 0xFFFFFFFF || image->crc32 == 0x00000000)
    {
        return E_INVALID_CRC;
    }

    // Validate image size. Our requirements are:
    // - Must be smaller than MAX(MAX_SIZE_2BOOT, MAX_SIZE_APP)
    // - Must be larger than sizeof(MetadataHeader)
    if (image->imageSize >= IMAGE_MAX_SIZE ||
        image->imageSize < IMAGE_MIN_SIZE)
    {
        return E_INVALID_LENGTH;
    }

    // The CRC and the image length look OK
    // We're going to validate imageLength - 4 bytes because we exclude the CRC from it's own calculation
    uint32_t bytesToCrc = image->imageSize - sizeof(uint32_t);
    uint32_t* startingLocation = (uint32_t*)(((uint8_t*)(&(image->crc32))) + sizeof(image->crc32));

    uint32_t crcResult = crc32(startingLocation, bytesToCrc);

    if (crcResult != image->crc32)
    {
        return E_CRC_MISMATCH;
    }

    return E_OK;
}