#include <util.h>
#include <nrf_delay.h>

void Util_impl::delay(uint16_t ms)
{
    nrf_delay_ms(ms);
}