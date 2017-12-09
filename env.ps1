function Add-ToPath([string] $path)
{
	if (-not (Test-Path -path $path))
	{
		throw "ERROR_NONEXISTENT_PATH"
	}
	
	$env:Path = "$Path;" + $env:Path
}

$WorkspaceRoot = $PSScriptRoot

Add-ToPath -path "$PSScriptRoot\nuget"

# Install all nuget packages
nuget.exe install "$WorkspaceRoot\nuget\packages.config" -ConfigFile "$WorkspaceRoot\nuget\nuget.config" -ExcludeVersion

Add-ToPath -path "$PSScriptRoot\nuget\packages\autom8ed.com.tools.cmake\bin"
Add-ToPath -path "$PSScriptRoot\nuget\packages\autom8ed.com.tools.ninja"

# TODO: Find a way to get this utility into a nuget package
Add-ToPath -path "C:\Program Files (x86)\SEGGER\JLink_V620b"

# Add a new environment variable, GCC_ROOT, which points to the root of the GCC pulled down by NuGet
# This is needed because Cmake toolchain files cannot import variables from other CMakeLists files, 
# and all compiles paths must be absolute paths. Only this environment setup script knows where to find the 
# compiler
$env:GCC_ROOT = "$PSScriptRoot\nuget\packages\autom8ed.com.compilers.arm-none-eabi-gcc"
$env:NANOBOOT_ROOT = "$PSScriptRoot"
$env:NUGET_PACKAGES_ROOT = "$PSScriptRoot\nuget\packages"

Import-Module ".\scripts\Register-BuildEnvironment.psm1" -Force
 