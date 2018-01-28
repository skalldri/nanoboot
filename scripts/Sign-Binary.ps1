param([string]$appBin)

# Based on the CRC32 functions at http://www.hackersdelight.org/hdcodetxt/crc.c.txt
function Get-Word($bytes, $startIndex)
{
    [int] $word = ([int]$bytes[$startIndex])
    [int] $word = $word -bor (([int]$bytes[$startIndex + 1]) -shl (1*8))
    [int] $word = $word -bor (([int]$bytes[$startIndex + 2]) -shl (2*8))
    [int] $word = $word -bor (([int]$bytes[$startIndex + 3]) -shl (3*8))

    return $word
}

function crc32($bytes) 
{
    [uint32] $crc = [uint32]::MaxValue
    [uint32] $0xEDB88320 = 3988292384 # have to defined the polynomial as a positive uint32 or the math below will explode

    foreach($valbyte in $bytes)
    {
        [byte] $byte = $valbyte
        [uint32] $crc = $crc -bxor $byte

        for ($j = 7; $j -ge 0; $j--)
        {
            if ($crc -band 1)
            {
                [uint32] $crc = ($crc -shr 1) -bxor ($0xEDB88320)
            }
            else
            {
                [uint32] $crc = ($crc -shr 1)
            }
        }
    }

    return (-bnot $crc)
 }

$bytes = [System.IO.File]::ReadAllBytes($appBin)

if (-not (($bytes.Count % 4) -eq 0))
{
    Write-Error "Binary input file is not word-aligned"
}

Write-Host "ImageSize: $($bytes.Count)"

[uint32] $numBytes = $bytes.Count

$bytes[4] = (($numBytes -shr 3*8) -band 255)
$bytes[5] = (($numBytes -shr 2*8) -band 255)
$bytes[6] = (($numBytes -shr 1*8) -band 255)
$bytes[7] = ($numBytes -band 255)

[uint32] $crc = crc32 -bytes $bytes[4..($bytes.Count-1)]

Write-Host "CRC32: $crc"

$bytes[0] = (($crc -shr 3*8) -band 255)
$bytes[1] = (($crc -shr 2*8) -band 255)
$bytes[2] = (($crc -shr 1*8) -band 255)
$bytes[3] = ($crc -band 255)

[System.IO.File]::WriteAllBytes($appBin, $bytes)