#pragma once

#include <cstdint>

class GPIO_base {

public:
    virtual bool read(uint16_t pin) = 0;
    virtual void set(uint16_t pin) = 0;
    virtual void clear(uint16_t pin) = 0;

    virtual void configure_output(uint16_t pin) = 0;
    virtual void configure_input(uint16_t pin) = 0;
};