#pragma once

#include <cstdint>

class UTIL_base {

public:
    virtual bool delay(uint16_t ms) = 0;
};