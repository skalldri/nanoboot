param([string]$appBin)

Import-Module "$PSScriptRoot\Utils.psm1" -Force

Test-CRC32

$appBin = Resolve-Path -Path $appBin

$bytes = [System.IO.File]::ReadAllBytes($appBin)

if (-not (($bytes.Count % 4) -eq 0))
{
    Write-Error "Binary input file is not word-aligned"
}

Write-Host "ImageSize: $($bytes.Count)"

[uint32] $numBytes = $bytes.Count

$bytes[7] = (($numBytes -shr 3*8) -band 255)
$bytes[6] = (($numBytes -shr 2*8) -band 255)
$bytes[5] = (($numBytes -shr 1*8) -band 255)
$bytes[4] = ($numBytes -band 255)

[uint32] $crc = crc32 -bytes $bytes[4..($bytes.Count-1)]

Write-Host "CRC32: $crc"

$bytes[3] = (($crc -shr 3*8) -band 255)
$bytes[2] = (($crc -shr 2*8) -band 255)
$bytes[1] = (($crc -shr 1*8) -band 255)
$bytes[0] = ($crc -band 255)

[System.IO.File]::WriteAllBytes($appBin + ".ni.bin", $bytes)