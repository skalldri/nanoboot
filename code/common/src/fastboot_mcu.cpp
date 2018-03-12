#include <fastboot_mcu.h>
#include <serial.h>
#include <util.h>
#include <string.h>

// TODO: move to a nanoboot configuration file
#define FASTBOOT_UART_DEVICE 0

// This is the main "fastboot" program. It's turned into a function here so that it can be included in different software components.
// After calling Fastboot(), the MCU is guaranteed to either reset or continue into the main application.
void Fastboot()
{
    Serial comPort(FASTBOOT_UART_DEVICE);
    Util utils;

    while(true)
    {
        comPort.WriteString("NANOBOOT TEST");
        utils.delay(1000);
    }
}