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