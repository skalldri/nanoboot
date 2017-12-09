#include <GPIO.h>
#include <nrf_gpio.h>

bool GPIO_impl::read(uint16_t pin)
{
    return false;
}

void GPIO_impl::set(uint16_t pin)
{
    nrf_gpio_pin_set(pin);
}

void GPIO_impl::clear(uint16_t pin)
{
    nrf_gpio_pin_clear(pin);
}

void GPIO_impl::configure_input(uint16_t pin)
{
    
}

void GPIO_impl::configure_output(uint16_t pin)
{
    nrf_gpio_cfg_output(pin);
}