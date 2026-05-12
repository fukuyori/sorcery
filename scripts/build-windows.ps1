param(
    [string]$BuildDir = "build-windows",
    [string]$Configuration = "Release",
    [string]$VcpkgRoot = "D:\home\source\git\vcpkg",
    [switch]$SkipRestoreExt,
    [switch]$ConfigureOnly
)

$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSScriptRoot
Set-Location $root

function Find-CMake {
    $cmd = Get-Command cmake -ErrorAction SilentlyContinue
    if ($cmd) {
        return $cmd.Source
    }

    $candidates = @(
        "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
        "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
        "C:\Program Files\CMake\bin\cmake.exe"
    )

    foreach ($candidate in $candidates) {
        if (Test-Path $candidate) {
            return $candidate
        }
    }

    throw "CMake was not found. Install CMake or Visual Studio C++ tools."
}

if (-not $SkipRestoreExt) {
    & (Join-Path $PSScriptRoot "restore-ext.ps1")
}

$cmake = Find-CMake
$toolchain = Join-Path $VcpkgRoot "scripts\buildsystems\vcpkg.cmake"
if (-not (Test-Path $toolchain)) {
    throw "vcpkg toolchain was not found: $toolchain"
}

& $cmake -S . -B $BuildDir "-DCMAKE_TOOLCHAIN_FILE=$toolchain"
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

if (-not $ConfigureOnly) {
    & $cmake --build $BuildDir --config $Configuration
    exit $LASTEXITCODE
}
