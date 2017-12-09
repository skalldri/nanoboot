param([string]$appHex)

$jscriptPath = Resolve-Path "$PSScriptRoot\..\toolchain\mcu\nrf52832\testapp.jlink"
cd "$PSScriptRoot\.."
& 'jlink.exe' -autoconnect 1 -speed 4000 -device NRF52832_XXAA -if SWD -CommanderScript $jscriptPath
