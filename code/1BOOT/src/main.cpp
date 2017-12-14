#include "flash_layout.h"
#include <crc32.h>
#include <util.h>
#include <gpio.h>

#define LED_PIN 17
#define INTER_BLINK_LENGTH 100
#define TINY_BLINK_LENGTH  80
#define SHORT_BLINK_LENGTH 500
#define LONG_BLINK_LENGTH  1000

MetadataHeader* slot2BOOT = (MetadataHeader*)(START_ADDRESS_2BOOT);
MetadataHeader* slotScratch = (MetadataHeader*)(START_ADDRESS_SCRATCH);

// Function declarations
void SOS();
void HCF();

// Function implementations
int main()
{
    // TODO: Implement testing framework
    if(TestCRC() != E_OK)
    {
        HCF();
    }

    // Validate the 2BOOT image in flash
    NANO_ERROR status2BOOT = ValidateImage(slot2BOOT);

    // Check the scratch area to see if it contains a 2BOOT image
    NANO_ERROR statusScratch = ValidateImage(slot2BOOT);

    // If the scratch area contains a valid image, 
    // we should try to copy it into the correct sector
    if (statusScratch == E_OK)
    {
        
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

// HCF = Halt And Catch Fire
// This function will form the core of the Nanoboot critical fault handling
// This function traps the processor in an infinite loop, blinking the LED
// at a constant rate. The device must be rest to escape the loop.
void HCF()
{
    GPIO gpio;
    Util util;

    gpio.configure_output(LED_PIN);

    // 3 quick blinks
    while(true)
    {
        gpio.set(LED_PIN);
        util.delay(TINY_BLINK_LENGTH);
        gpio.clear(LED_PIN);
        util.delay(TINY_BLINK_LENGTH);
    }
}   