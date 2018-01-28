#include <flash.h>
#include <ble_flash.h>

const Sector sectorList[] = {
    {.type = ONE_BOOT, .startAddress = START_ADDRESS_1BOOT,   .size = MAX_SIZE_1BOOT},
    {.type = TWO_BOOT, .startAddress = START_ADDRESS_2BOOT,   .size = MAX_SIZE_2BOOT},
    {.type = APP,      .startAddress = START_ADDRESS_APP,     .size = MAX_SIZE_APP},
    {.type = SCRATCH,  .startAddress = START_ADDRESS_SCRATCH, .size = MAX_SIZE_SCRATCH}
};

const uint32_t sectorListSize = (sizeof(sectorList) / sizeof(sectorList[0]));

NANO_ERROR Flash_impl::Write(Sector* sector, uint8_t* bytes, uint32_t size)
{
    // Sanity checks:
    // - Write is inside the bounds of the Sector
    // - Write has non-zero size
    // - Size is a multiple of 4 bytes

    if ((size % 4) != 0)
    {
        return E_UNALIGNED_ACCESS;
    }

    if (size > sector->size)
    {
        return E_OVERFLOW;
    }

    if (size == 0)
    {
        return E_INVALID_LENGTH;
    }

    // TODO: the Nordic library indicates that flash writes can interefere with 
    //       radio communication.
    //       Re-design this section once we have support for Nordic softdevices,
    //       we should probably use the ble_flash_*** functions in ble_flash.c

    // Turn on flash write enable and wait until the NVMC is ready.
    NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos);
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
    {
        // Do nothing.
    }
    
    uint32_t* target = (uint32_t*) sector->startAddress;
    
    for (int i = 0; i < (size/4); i++)
    {
        // Reassemble the 32-bit value in a word-aligned variable, 
        // in case bytes doesn't line up with a word boundary.
        // No unaligned 32-bit access is allowed.
        uint32_t value = (bytes[(i*4) + 3] << 24) |
                         (bytes[(i*4) + 2] << 16) |
                         (bytes[(i*4) + 1] << 8)  |
                         (bytes[(i*4)]);
        
        // Wait flash write to finish
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
        {
            // Do nothing.
        }

        *target = value;
        target++;
    }

    // Turn off flash write enable and wait until the NVMC is ready.
    NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
    {
        // Do nothing.
    }

    return E_OK;
}

NANO_ERROR Flash_impl::Erase(Sector* sector)
{
    uint32_t page = 0;
    NANO_ERROR result = E_OK;

    result = AddressToPageNumber(sector->startAddress, &page);

    if (result != E_OK)
    {
        return result;
    }

    ble_flash_page_erase(page);

    return E_OK;
}

int Flash_impl::SectorCount()
{
    return sectorListSize;
}

const Sector* const Flash_impl::Sectors()
{
    return sectorList;
}

NANO_ERROR Flash_impl::AddressToPageNumber(uint32_t address, uint32_t* out_page)
{
    if (address > (FLASH_START_ADDRESS + FLASH_SIZE))
    {
        return E_OVERFLOW;
    }

    if ((address % BLE_FLASH_PAGE_SIZE) != 0)
    {
        return E_UNALIGNED_ACCESS;
    }

    *out_page = address / BLE_FLASH_PAGE_SIZE;

    return E_OK;
}