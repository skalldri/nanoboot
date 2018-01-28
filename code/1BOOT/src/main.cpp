#include "flash_layout.h"
#include <crc32.h>
#include <util.h>
#include <gpio.h>
#include <utils.h>
#include <flash.h>

#define LED_PIN 17
#define INTER_BLINK_LENGTH 100
#define TINY_BLINK_LENGTH  80
#define SHORT_BLINK_LENGTH 500
#define LONG_BLINK_LENGTH  1000

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

    Flash flash;
    Sector slot2BOOT = {};
    Sector slotScratch = {};

    for (int i = 0; i < flash.SectorCount(); i++)
    {
        // Search for potential 2BOOT images:
        // - In 2BOOT sectors (these are bootable)
        // - In SCRATCH sectors (these are not bootable)

        MetadataHeader* header = (MetadataHeader*)((flash.Sectors())[i].startAddress);

        // TODO: support multiple Sectors of the same type

        // Bootable 2BOOT sector
        if ((flash.Sectors())[i].type == TWO_BOOT)
        {
            slot2BOOT = (flash.Sectors())[i];
        }
        // SCRATCH sector containing a new 2BOOT image
        else if ((flash.Sectors())[i].type == SCRATCH)
        {
            slotScratch = (flash.Sectors())[i];
        }
    }

    // Check the scratch area to see if it contains a valid image
    NANO_ERROR statusScratch = ValidateImage((MetadataHeader*)(slotScratch.startAddress));

    // Validate the 2BOOT image in flash
    NANO_ERROR status2BOOT = ValidateImage((MetadataHeader*)(slot2BOOT.startAddress));

    // Track what action 1BOOT should take
    NANO_ACTION actionToPerform = FATAL_ERROR;
    
    // If both scratch and 2BOOT are valid, check versions
    if ((statusScratch == E_OK) && (status2BOOT == E_OK))
    {
        NANO_COMPARE compare = ImageVersionCompare((MetadataHeader*)(slotScratch.startAddress), (MetadataHeader*)(slot2BOOT.startAddress));

        if (compare == FIRST_IS_GREATER)
        {
            actionToPerform = BEGIN_UPGRADE;
        }
        else
        {
            actionToPerform = CONTINUE_BOOT;
        }
    }
    else if (statusScratch == E_OK)
    {
        actionToPerform = BEGIN_RECOVERY;
    }
    else if (status2BOOT == E_OK)
    {
        actionToPerform = CONTINUE_BOOT;
    }

    switch (actionToPerform)
    {
        case CONTINUE_BOOT:
        {
            StartImageFromFlash((MetadataHeader*)(slot2BOOT.startAddress));
        }
        break;

        // TODO: distinguish between these cases
        case BEGIN_UPGRADE:
        case BEGIN_RECOVERY:
        {
            // Erase the 2BOOT slot
            flash.Erase(&slot2BOOT);

            uint8_t* source = (uint8_t*)slotScratch.startAddress;

            flash.Write(&slot2BOOT, source, ((MetadataHeader*)(slotScratch.startAddress))->imageSize);

            // Reset the MCU to restart nanoboot
            Util utils;
            utils.ResetMCU();
        }
        break;

        case FATAL_ERROR:
        {
            // Unable to find a valid 2BOOT image... device is bricked and requires JTAG to repair.
            // Blink our SOS pattern to indicate we're hosed
            SOS();
        }
        break;

        // Should be impossible to hit this case. It may indicate memory corruption
        default:
        {
            HCF();
        }
        break;
    }
    
    // Should never get here. Trap in the HCF routine.
    while (true)
    {
        HCF();
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