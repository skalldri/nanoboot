#pragma once

#include <cstdint>

class Util_base {

public:
    virtual void delay(uint16_t ms) = 0;

    virtual void ResetMCU();
};