#include "flash_layout.h"
#include <util.h>
#include <gpio.h>

#define LED_PIN 17
#define INTER_BLINK_LENGTH 100
#define SHORT_BLINK_LENGTH 500
#define LONG_BLINK_LENGTH  1000

MetadataHeader* slot2BOOT = (MetadataHeader*)(START_ADDRESS_2BOOT);

int main()
{
    // Unable to find a valid 2BOOT image... device is bricked and requires JTAG to repair.
    // Blink our SOS pattern to indicate
    GPIO gpio;
    Util util;

    gpio.configure_output(LED_PIN);

    while (true)
    {
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

    return 0;
}