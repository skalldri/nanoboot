#pragma once

#include <gpio_base.h>

class GPIO_impl : public GPIO_base {

public:
    bool read(uint16_t pin);
    void set(uint16_t pin);
    void clear(uint16_t pin);
    
    void configure_output(uint16_t pin);
    void configure_input(uint16_t pin);

};

typedef GPIO_impl GPIO;