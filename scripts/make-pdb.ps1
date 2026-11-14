#Requires -Version 5.1
<#
.SYNOPSIS
    Generates a PDB for a MinGW build of Moonshine Runners.

.DESCRIPTION
    The MinGW build embeds DWARF debug information (CMAKE_BUILD_TYPE=RelWithDebInfo).
    cv2pdb converts it to a PDB and patches the executable so that DbgHelp,
    WinDbg and Visual Studio can resolve symbols. The generated PDB is placed
    next to the executable by default, which is where the crash reporter looks
    for it.

.PARAMETER Exe
    Executable to convert (in-place). Example: build-mingw/MechoSoma/moonshine-runners.exe

.PARAMETER Pdb
    Output PDB path. Defaults to <exe>.pdb next to the executable.

.PARAMETER Cv2pdb
    Path to cv2pdb64.exe/cv2pdb.exe. When omitted, the tool is searched in PATH
    and in .tools/cv2pdb; if it is still missing, the pinned release is
    downloaded into .tools/cv2pdb.

.PARAMETER Version
    cv2pdb release to download when no local copy is found. Default: 0.54.

.EXAMPLE
    .\scripts\make-pdb.ps1 -Exe build-mingw\MechoSoma\moonshine-runners.exe
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$Exe,

    [string]$Pdb,

    [string]$Cv2pdb,

    [string]$Version = '0.54'
)

$ErrorActionPreference = 'Stop'

$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$exePath = (Resolve-Path $Exe).Path

if (-not $Pdb) {
    $Pdb = [System.IO.Path]::ChangeExtension($exePath, '.pdb')
}
$Pdb = [System.IO.Path]::GetFullPath($Pdb)

function Find-Cv2pdb([string]$Root) {
    foreach ($name in @('cv2pdb64.exe', 'cv2pdb.exe')) {
        $candidate = Join-Path $Root $name
        if (Test-Path $candidate) { return $candidate }
    }
    return $null
}

if (-not $Cv2pdb) {
    $onPath = Get-Command 'cv2pdb64.exe', 'cv2pdb.exe' -ErrorAction SilentlyContinue |
        Select-Object -First 1
    if ($onPath) {
        $Cv2pdb = $onPath.Source
    }
}

$toolsRoot = Join-Path $repo '.tools\cv2pdb'
if (-not $Cv2pdb) {
    $Cv2pdb = Find-Cv2pdb $toolsRoot
}

if (-not $Cv2pdb) {
    New-Item -ItemType Directory -Force -Path $toolsRoot | Out-Null
    $zipPath = Join-Path $toolsRoot "cv2pdb-$Version.zip"
    $url = "https://github.com/rainers/cv2pdb/releases/download/v$Version/cv2pdb-$Version.zip"

    Write-Host "Downloading cv2pdb $Version from $url ..."
    Invoke-WebRequest -Uri $url -OutFile $zipPath -UseBasicParsing
    Expand-Archive -Path $zipPath -DestinationPath $toolsRoot -Force

    $Cv2pdb = Find-Cv2pdb $toolsRoot
    if (-not $Cv2pdb) {
        throw "cv2pdb executable was not found in $zipPath."
    }
}

Write-Host "Converting debug information: $exePath"
& $Cv2pdb $exePath $exePath $Pdb
if ($LASTEXITCODE -ne 0) {
    throw "cv2pdb failed with exit code $LASTEXITCODE."
}
if (-not (Test-Path $Pdb)) {
    throw "Expected PDB was not created: $Pdb"
}

Write-Host "PDB written to $Pdb"
