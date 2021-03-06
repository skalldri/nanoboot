#include <gpio.h>
#include <nrf_gpio.h>

bool GPIO_impl::read(uint16_t pin)
{
    return (nrf_gpio_pin_read(pin) != 0);
}

void GPIO_impl::set(uint16_t pin)
{
    nrf_gpio_pin_set(pin);
}

void GPIO_impl::clear(uint16_t pin)
{
    nrf_gpio_pin_clear(pin);
}

void GPIO_impl::configure_input(uint16_t pin, GPIO_PULL pull)
{
    switch(pull)
    {
        case PULL_UP:
            nrf_gpio_cfg_input(pin, NRF_GPIO_PIN_PULLUP);
            break;

        case PULL_DOWN:
            nrf_gpio_cfg_input(pin, NRF_GPIO_PIN_PULLDOWN);
            break;

        default:
        case NO_PULL:
            nrf_gpio_cfg_input(pin, NRF_GPIO_PIN_NOPULL);
            break;
    }
}

void GPIO_impl::configure_output(uint16_t pin)
{
    nrf_gpio_cfg_output(pin);
}