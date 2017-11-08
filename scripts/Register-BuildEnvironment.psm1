function Register-BuildEnvironment
{
    pushd "$PSScriptRoot\..\build"
    Remove-Item "$PSScriptRoot\..\build\*" -Recurse
    cmake -DCMAKE_TOOLCHAIN_FILE="$PSScriptRoot\..\toolchain\windows10-amd64-arm-none-eabi-gcc.cmake" -G Ninja ..

    popd
}

function Start-GdbServer
{
    
}

function Push-Firmware([string]$appHex)
{
    $jscriptPath = Resolve-Path "$PSScriptRoot\..\toolchain\nrf52dk.jscript"
    cd "$PSScriptRoot\.."
    & 'C:\Program Files (x86)\SEGGER\JLink_V620b\jlink.exe' -autoconnect 1 -speed 4000 -device NRF52832_XXAA -if SWD -CommanderScript $jscriptPath
}