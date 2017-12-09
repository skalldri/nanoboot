function Register-BuildEnvironment([string] $platform)
{
    $toolchain = ""

    $supportedPlatforms = Find-SupportedPlatforms

    # Nordic NRF52, ARM Cortex-M4
    if ($supportedPlatforms -contains $platform)
    {
        # TODO: rework ARM toolchain for generic mode
        $toolchain = "$PSScriptRoot\..\toolchain\mcu\$platform\$platform.cmake"
    }
    else 
    {
        Write-Host -ForegroundColor Red "ERROR: Must specify a platform"
        Write-Host -ForegroundColor Yellow "Available platforms: "
        Write-Host $supportedPlatforms
        return
    }

    pushd "$PSScriptRoot\..\build"
    Remove-Item "$PSScriptRoot\..\build\*" -Recurse

    cmake -DCMAKE_TOOLCHAIN_FILE="$toolchain" -G Ninja ..

    popd
}

function Find-SupportedPlatforms()
{
    $supportedPlatforms = @()
    $mcuFolders = Get-ChildItem -path "$PSScriptRoot\..\toolchain\mcu"

    foreach($mcuFolder in $mcuFolders)
    {
        $mcuCmake = $mcuFolder.FullName + "\" + $mcuFolder.Name + ".cmake"
        if(Test-Path -Path $mcuCmake -PathType Leaf)
        {
            $supportedPlatforms += $mcuFolder.Name
        }
        else 
        {
            Write-Host -ForegroundColor Yellow "MCU folder without corresponding MCU CMake file. MCU Platform will be ignored."
            Write-Host -ForegroundColor Yellow "MCU:  " $mcuFolder.Name    
            Write-Host -ForegroundColor Yellow "Path: " $mcuFolder.FullName
        }
    }

    return $supportedPlatforms
}