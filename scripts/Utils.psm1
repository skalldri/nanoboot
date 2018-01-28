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

# Test that the PowerShell CRC32 functions return the same value as firmware (this test is implemented in the Nanoboot startup code as well)
function Test-CRC32
{
    $bytes = @(0xEF, 0xBE, 0xAD, 0xDE)
    
    $result = crc32 -bytes $bytes
    
    if ($result -ne 0x1A5A601F)
    {
        Write-Error "CRC32 Test Failed. Expected 0x1A5A601F, got $result"
        exit
    }
}