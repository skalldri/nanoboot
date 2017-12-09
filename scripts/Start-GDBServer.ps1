$gdbConfigPath = Resolve-Path "$PSScriptRoot\..\toolchain\mcu\nrf52832\gdbserver.jlink"
& 'jlinkgdbservercl.exe' -autoconnect 1 -speed 4000 -device NRF52832_XXAA -if SWD -port 2331 -xc $gdbConfigPath