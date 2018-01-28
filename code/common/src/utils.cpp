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