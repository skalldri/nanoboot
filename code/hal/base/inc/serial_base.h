#pragma once

#include <cstdint>

// TODO: enforce singleton pattern in the serial_base class:
// We should only allow a single instance of this class per hardware block
class Serial_base {

public:
    virtual void Read(char** data) = 0;
    virtual void Write(char* data, uint32_t length) = 0;
    virtual void WriteString(const char* data) = 0;
};