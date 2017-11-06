function Register-BuildEnvironment
{
    pushd "$PSScriptRoot\..\build"
    Remove-Item "$PSScriptRoot\..\build\*" -Recurse
    cmake -DCMAKE_TOOLCHAIN_FILE="$PSScriptRoot\..\toolchain\windows10-amd64-arm-none-eabi-gcc.cmake" -G Ninja ..

    popd
}