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
    GPIO gpio;
    Util util;

    gpio.configure_output(LED_PIN);
    gpio.clear(LED_PIN);

    // Do something fun in 2BOOT for now... PWM the LED (using our internal timer)
    while (true)
    {
        gpio.clear(LED_PIN);

        // Fade up. We only get ms resolution on our basic timer, which limits our pulse-width accuracy
        // Say we want an LED refresh rate of 50 Hz -> 20ms per frame
        // Therefore we have 20 brightness levels: all 20ms illuminated, all 20ms dark, and every 1ms increment in between
        // Each frame is 20ms. Say we want 100ms at each brightness level
        #define PWM_BRIGHTNESS_TIME_MS 100
        #define PWM_FRAME_WIDTH_MS 20

        // Fade in
        for(uint32_t i = 0; i < PWM_FRAME_WIDTH_MS; i++)
        {
            for(uint32_t k = 0; k < PWM_BRIGHTNESS_TIME_MS; k += PWM_FRAME_WIDTH_MS)
            {
                gpio.clear(LED_PIN);
                util.delay(i);
                gpio.set(LED_PIN);
                util.delay(PWM_FRAME_WIDTH_MS - i);
            }
        }

        // Fade out
        for(uint32_t i = 0; i < PWM_FRAME_WIDTH_MS; i++)
        {
            for(uint32_t k = 0; k < PWM_BRIGHTNESS_TIME_MS; k += PWM_FRAME_WIDTH_MS)
            {
                gpio.clear(LED_PIN);
                util.delay(PWM_FRAME_WIDTH_MS - i);
                gpio.set(LED_PIN);
                util.delay(i);
            }
        }
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