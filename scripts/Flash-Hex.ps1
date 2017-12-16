param([string]$jlinkFile)

$jscriptPath = Resolve-Path $jlinkFile
cd "$PSScriptRoot\.."
& 'jlink.exe' -autoconnect 1 -speed 4000 -device NRF52832_XXAA -if SWD -CommanderScript $jscriptPath
