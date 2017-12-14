
void StartImageFromFlash(MetadataHeader* image)
{
    // Steps to starting an image:
    // - Adjust the VTOR to point to the new image
    // -- For PIC code: copy VTOR to RAM, patch with app offset
    // - Reset the stack pointer to the initial stack value provided by the application
    // - Jump to the app start address
}