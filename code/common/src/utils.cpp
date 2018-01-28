#include <utils.h>

typedef void (*fptr)();

void StartImageFromFlash(MetadataHeader* image)
{
    // Steps to starting an image:
    // - Adjust the VTOR to point to the new image
    // -- For PIC code: copy VTOR to RAM, patch with app offset
    // - Reset the stack pointer to the initial stack value provided by the application
    // - Jump to the app start address

    // Find the vector table using MetadataHeader info

    // Recall flash layout is:
    //  METADATA
    //  DC0
    //  IVT
    //  DC1
    //  REST OF APP
    VectorTableCommon* vectorTable = (VectorTableCommon*)(image->dc0Size + sizeof(MetadataHeader) + (uint32_t)image);
    
    fptr AppEntryPoint = (fptr)vectorTable->resetHandler;

    // TODO: reset stack pointer before jumping into app
    // TODO: use a naked function call to ensure we don't put useless return values on the stack
    
    AppEntryPoint();
}

NANO_COMPARE ImageVersionCompare(MetadataHeader* first, MetadataHeader* second)
{
    NANO_COMPARE result = UNABLE_TO_COMPARE;

    // Major version of first is strictly greater than second. Result is known.
    if (first->majorVersion > second->majorVersion)
    {
        return FIRST_IS_GREATER;
    }
    // Major versions are the same. Dig deeper.
    else if (first->majorVersion >= second->majorVersion)
    {
        if (first->minorVersion > second->minorVersion)
        {
            return FIRST_IS_GREATER;
        }
        else if (first->minorVersion >= second->minorVersion)
        {
            if (first->revision > second->revision)
            {
                return FIRST_IS_GREATER;
            }
            else if (first->revision >= second->revision)
            {
                return EQUAL;
            }
            else 
            {
                return SECOND_IS_GREATER;
            }
        }
        else
        {
            return SECOND_IS_GREATER;
        }
    }
    // Major versions are not equal, and first major version is less than second version. Result is known.
    else
    {
        return SECOND_IS_GREATER;
    }
}