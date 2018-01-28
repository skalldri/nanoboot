#pragma once

#include <cstdint>
#include <flash_layout.h>
#include <utils.h>

struct Sector {
    ImageSector type;
    uint32_t startAddress;
    uint32_t size;
};

class Flash_base {

public:
    
    virtual NANO_ERROR Write(Sector* sector, uint8_t* bytes, uint32_t size) = 0;
    virtual NANO_ERROR Erase(Sector* sector) = 0;
    virtual int  SectorCount() = 0;
    virtual const Sector* const Sectors() = 0;

    // TODO: implement later
    /*
    virtual bool Unlock(Sector* sector) = 0;
    virtual bool Lock(Sector* sector) = 0;
    virtual bool SupportsLocking(Sector* sector) = 0;
    virtual bool IsLocked(Sector* sector) = 0;
    */
};