#pragma once

#include <flash_base.h>

class Flash_impl : public Flash_base {

public:
    NANO_ERROR Write(Sector* sector, uint8_t* bytes, uint32_t size);
    NANO_ERROR Erase(Sector* sector);
    int  SectorCount();
    const Sector* const Sectors();

private:
    NANO_ERROR AddressToPageNumber(uint32_t address, uint32_t* out_page);

};

typedef Flash_impl Flash;