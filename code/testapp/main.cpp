#include <gpio.h>
#include <util.h>

#define LED_PIN 17

GPIO gpio;
Util util;

int main()
{
    gpio.configure_output(LED_PIN);

    while(true)
    {
        gpio.set(LED_PIN);

        util.delay(1000);

        gpio.clear(LED_PIN);

        util.delay(1000);
    }

    return 0;
}