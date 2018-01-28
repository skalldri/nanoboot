#include <util.h>
#include <nrf_delay.h>
#include <core_cm4.h>

void Util_impl::delay(uint16_t ms)
{
    nrf_delay_ms(ms);
}

void Util_impl::ResetMCU()
{
    NVIC_SystemReset();
}
