#pragma once

#include <util_base.h>

class Util_impl : public Util_base {

public:
    void delay(uint16_t ms);

    void ResetMCU();
};

typedef Util_impl Util;