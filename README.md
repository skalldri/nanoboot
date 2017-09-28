# nanoboot
Smaller than Das U-Boot, Nanoboot is a tiny 2-stage bootloader for ARM Cortex-M Devices.

The initial Nanoboot implementation is for the NRF52810 microcontroller.

The bootloader will implement the fastboot protocol. Fastboot does not natively support Bluetooth, 
so this repo will fork the native fastboot client and add Bluetooth support.

The bootloader will be written in C++. It will compile using GCC. The primary supported IDE will be Visual Studio Code.
