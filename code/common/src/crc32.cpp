#include <crc32.h>
#include <cstdint>
// These routines are adopted from the CRC32 implementation in "The Hackers Delight".
// They may be freely reproduced and modified without attribution.
// http://www.hackersdelight.org/hdcodetxt/crc.c.txt

// ---------------------------- reverse --------------------------------

// Reverses (reflects) bits in a 32-bit word.
uint32_t reverse(uint32_t x) 
{
   x = ((x & 0x55555555) <<  1) | ((x >>  1) & 0x55555555);
   x = ((x & 0x33333333) <<  2) | ((x >>  2) & 0x33333333);
   x = ((x & 0x0F0F0F0F) <<  4) | ((x >>  4) & 0x0F0F0F0F);
   x = (x << 24) | ((x & 0xFF00) << 8) |
       ((x >> 8) & 0xFF00) | (x >> 24);
   return x;
}

uint32_t crc32(uint8_t *message, uint32_t size) {
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
   for (uint32_t position = 0; position < size / 4; position++)
   {
      crc = crc ^ message[position];
      crc = (crc >> 8) ^ table[crc & 0xFF];
      crc = (crc >> 8) ^ table[crc & 0xFF];
      crc = (crc >> 8) ^ table[crc & 0xFF];
      crc = (crc >> 8) ^ table[crc & 0xFF];
   }

   return ~crc;
}