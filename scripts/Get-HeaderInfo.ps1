param([string]$appBin)

Import-Module "$PSScriptRoot\Utils.psm1" -Force

$appBin = Resolve-Path -Path $appBin

$bytes = [System.IO.File]::ReadAllBytes($appBin)

# Header starts with a CRC:
$word = Get-Word -bytes $bytes -startIndex 0
Write-Host "CRC32: $word"

$word = Get-Word -bytes $bytes -startIndex 4
Write-Host "ImageSize: $word"

$word = Get-Word -bytes $bytes -startIndex 8
Write-Host "Platform ID + Sector: $word"

$word = Get-Word -bytes $bytes -startIndex 12
Write-Host "Magic Number: $word"

$word = Get-Word -bytes $bytes -startIndex 16
Write-Host "Major Version: $word"

$word = Get-Word -bytes $bytes -startIndex 20
Write-Host "Minor Version: $word"

$word = Get-Word -bytes $bytes -startIndex 24
Write-Host "Revision: $word"

$word = Get-Word -bytes $bytes -startIndex 28
Write-Host "DC0 Size: $word"

$word = Get-Word -bytes $bytes -startIndex 32
Write-Host "IVT Size: $word"

$word = Get-Word -bytes $bytes -startIndex 36
Write-Host "DC1 Size: $word"
