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
Add-ToPath -path "$PSScriptRoot\nuget\packages\autom8ed.com.tools.srecord"

# Try to detect a J-Link installation
# Check Program Files (x86) first
$seggerPath = $null
if (Test-Path -Path "C:\Program Files (x86)" -PathType Container)
{
	$seggerPath = ((Get-ChildItem -Path "C:\Program Files (x86)") | Where-Object {$_.Name -eq "SEGGER"})
}

if ($seggerPath -eq $null)
{
	if (Test-Path -Path "C:\Program Files" -PathType Container)
	{
		$seggerPath = ((Get-ChildItem -Path "C:\Program Files") | Where-Object {$_.Name -eq "SEGGER"})
	}
}

if ($seggerPath -ne $null)
{
	$jlinkPath = $null

	# found a SEGGER install folder. See if there are any J-Link installs there
	if (($seggerPath -ne $null) -and ($seggerPath.Count -eq 1))
	{
		$jlinkVersions = ((Get-ChildItem -Path $seggerPath.FullName) | Where-Object {$_.Name.StartsWith("JLink_") })

		$recentVersion = $null
		foreach($version in $jlinkVersions)
		{
			if ($recentVersion -eq $null)
			{
				$recentVersion = $version
				continue
			}

			if ($recentVersion -lt $version)
			{
				$recentVersion = $version
			}
		}
		
		if ($recentVersion -ne $null)
		{
			$jlinkPath = $recentVersion.FullName
		}
	}
	elseif ($seggerPath.Count -gt 1)
	{
		Write-Error "Multiple SEGGER folders found in C:\Program Files (x86)"
	}

	if ($jlinkPath -ne $null)
	{
		Write-Host "Using SEGGER install from $jlinkPath"
		Add-ToPath -path $jlinkPath
	}
	else 
	{
		Write-Warning "Could not find JLink install under $seggerPath. Live debugging and MCU Flashing commands will be unavailable."
		Write-Warning "Install J-Link from SEGGER Website"
	}
}
else
{
	Write-Warning "Could not find SEGGER folder. Live debugging and MCU Flashing commands will be unavailable."
	Write-Warning "Install J-Link from SEGGER Website"
}

# Add a new environment variable, GCC_ROOT, which points to the root of the GCC pulled down by NuGet
# This is needed because Cmake toolchain files cannot import variables from other CMakeLists files, 
# and all compiles paths must be absolute paths. Only this environment setup script knows where to find the 
# compiler
$env:GCC_ROOT = "$PSScriptRoot\nuget\packages\autom8ed.com.compilers.arm-none-eabi-gcc"
$env:NANOBOOT_ROOT = "$PSScriptRoot"
$env:NUGET_PACKAGES_ROOT = "$PSScriptRoot\nuget\packages"

Import-Module ".\scripts\Register-BuildEnvironment.psm1" -Force
 