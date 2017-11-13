#include <UTIL.h>
#include <nrf_delay.h>

void UTIL_impl::delay(uint16_t ms)
{
    nrf_delay_ms(1000);
}