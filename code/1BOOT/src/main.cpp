#include "flash_layout.h"
#include "crc32.h"
#include <util.h>
#include <gpio.h>

#define LED_PIN 17
#define INTER_BLINK_LENGTH 100
#define SHORT_BLINK_LENGTH 500
#define LONG_BLINK_LENGTH  1000

MetadataHeader* slot2BOOT = (MetadataHeader*)(START_ADDRESS_2BOOT);

uint32_t testBlob[] = {0xDEADBEEF, 0x0BADC0DE, 0xC0FEBABE, 0x00000000};

// Function declarations
void SOS();

// Function implementations
int main()
{
    uint32_t crc = crc32((uint8_t*)testBlob, 1 * sizeof(uint32_t));

    if (crc != 0xF9B8929D)
    {
        SOS();
    }

    // Unable to find a valid 2BOOT image... device is bricked and requires JTAG to repair.
    // Blink our SOS pattern to indicate we're hosed
    while (true)
    {
        SOS();
    }

    return 0;
}

// Blink an SOS code on our debug LED
void SOS()
{
    GPIO gpio;
    Util util;

    gpio.configure_output(LED_PIN);

    // 3 short blinks
    for(int i = 0; i < 3; i++)
    {
        gpio.set(LED_PIN);
        util.delay(SHORT_BLINK_LENGTH);
        gpio.clear(LED_PIN);
        util.delay(INTER_BLINK_LENGTH);
    }
    
    // 3 long blinks
    for(int i = 0; i < 3; i++)
    {
        gpio.set(LED_PIN);
        util.delay(LONG_BLINK_LENGTH);
        gpio.clear(LED_PIN);
        util.delay(LONG_BLINK_LENGTH);
    }
    
    // 3 short blinks
    for(int i = 0; i < 3; i++)
    {
        gpio.set(LED_PIN);
        util.delay(SHORT_BLINK_LENGTH);
        gpio.clear(LED_PIN);
        util.delay(INTER_BLINK_LENGTH);
    }

    // Wait 2 seconds between patterns
    util.delay(2000);
}   