[CmdletBinding()]
param(
    [string] $BuildDir = 'build-release',
    [string] $Msys2Root = 'C:\msys64',
    [int] $Jobs = [Environment]::ProcessorCount
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$repo = Split-Path -Parent $PSScriptRoot
$build = if ([IO.Path]::IsPathRooted($BuildDir)) {
    [IO.Path]::GetFullPath($BuildDir)
} else {
    [IO.Path]::GetFullPath((Join-Path $repo $BuildDir))
}
$ucrtBin = Join-Path $Msys2Root 'ucrt64\bin'
$msysBin = Join-Path $Msys2Root 'usr\bin'
$cmake = Join-Path $ucrtBin 'cmake.exe'
$ninja = Join-Path $ucrtBin 'ninja.exe'
$compiler = Join-Path $ucrtBin 'g++.exe'

foreach ($tool in @($cmake, $ninja, $compiler)) {
    if (-not (Test-Path -LiteralPath $tool -PathType Leaf)) {
        throw "Required MSYS2 UCRT64 tool not found: $tool"
    }
}
if ($Jobs -lt 1) { throw 'Jobs must be at least 1.' }

$env:PATH = "$ucrtBin;$msysBin;$env:PATH"

& $cmake -S $repo -B $build -G Ninja '-DCMAKE_BUILD_TYPE=Release' "-DCMAKE_CXX_COMPILER=$compiler"
if ($LASTEXITCODE -ne 0) { throw "Release configuration failed: $LASTEXITCODE" }

& $cmake --build $build --target sorcery --parallel $Jobs
if ($LASTEXITCODE -ne 0) { throw "Release build failed: $LASTEXITCODE" }

$dist = Join-Path $build 'dist'
foreach ($relative in @('sorcery.exe', 'cfg\config.ini', 'dat\strings.json',
                        'dat\strings.ja.json', 'dat\fonts\sorcery-jp-sans-medium.ttf')) {
    if (-not (Test-Path -LiteralPath (Join-Path $dist $relative) -PathType Leaf)) {
        throw "Release payload is incomplete: $relative"
    }
}

$bytes = [IO.File]::ReadAllBytes((Join-Path $dist 'sorcery.exe'))
$peOffset = [BitConverter]::ToInt32($bytes, 0x3c)
$subsystem = [BitConverter]::ToUInt16($bytes, $peOffset + 0x5c)
if ($subsystem -ne 2) { throw "Expected a Windows GUI executable; PE subsystem is $subsystem." }

Write-Output "Release payload: $dist"
