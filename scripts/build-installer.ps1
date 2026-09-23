[CmdletBinding()]
param(
    [switch] $Sign,
    [switch] $SkipBuild,
    [string] $BuildDir = 'build-release',
    [string] $OutputDir = '',
    [string] $Msys2Root = 'C:\msys64',
    [string] $ISCCPath = '',
    [string] $SignToolPath = '',
    [string] $TimestampUrl = 'http://timestamp.digicert.com'
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Resolve-PathFromRepo([string] $Value) {
    if ([IO.Path]::IsPathRooted($Value)) { return [IO.Path]::GetFullPath($Value) }
    return [IO.Path]::GetFullPath((Join-Path $repo $Value))
}

function Require-File([string] $Path) {
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) { throw "Required file not found: $Path" }
}

function Get-CodeSignArguments([string] $Certificate, [string] $Timestamp) {
    if ($Certificate.Contains('"')) {
        throw 'CODESIGN_CERT must not contain a double quote character.'
    }

    $arguments = @('sign', '/fd', 'SHA256')
    $certificateFile = $false
    try {
        $certificateFile = Test-Path -LiteralPath $Certificate -PathType Leaf
    } catch {
        $certificateFile = $false
    }
    $thumbprint = $Certificate -replace '[\s:]', ''

    if ($certificateFile) {
        $arguments += @('/f', (Resolve-Path -LiteralPath $Certificate).Path)
        if (-not [string]::IsNullOrEmpty($env:CODESIGN_CERT_PASSWORD)) {
            $arguments += @('/p', $env:CODESIGN_CERT_PASSWORD)
        }
    } elseif ($thumbprint -match '^[0-9A-Fa-f]{40}$') {
        $arguments += @('/sha1', $thumbprint)
    } else {
        $arguments += @('/n', $Certificate)
    }

    if (-not [string]::IsNullOrWhiteSpace($Timestamp)) {
        $arguments += @('/tr', $Timestamp, '/td', 'SHA256')
    }
    return ,$arguments
}

function Format-IsccSignCommand([string] $Executable, [string[]] $Arguments) {
    $tokens = @($Executable) + $Arguments
    $formatted = foreach ($token in $tokens) {
        if ($token -match '\s') { '$q' + $token + '$q' } else { $token }
    }
    return (($formatted -join ' ') + ' $f')
}

$repo = Split-Path -Parent $PSScriptRoot
$build = Resolve-PathFromRepo $BuildDir
if (-not $OutputDir) {
    $OutputDir = if ($Sign) {
        'dist\windows-signed\' + [guid]::NewGuid().ToString('N')
    } else {
        'dist\windows'
    }
}
$output = Resolve-PathFromRepo $OutputDir

if (-not $ISCCPath) {
    $command = Get-Command ISCC.exe -ErrorAction SilentlyContinue
    if ($command) { $ISCCPath = $command.Source }
    if (-not $ISCCPath) {
        $ISCCPath = Join-Path ${env:ProgramFiles(x86)} 'Inno Setup 6\ISCC.exe'
    }
}
Require-File $ISCCPath

if ($Sign) {
    $signCertificate = [string] $env:CODESIGN_CERT
    if ([string]::IsNullOrWhiteSpace($signCertificate)) {
        throw '-Sign requires CODESIGN_CERT: a PFX path, SHA-1 thumbprint, or certificate subject name.'
    }
    if (-not $SignToolPath) {
        $command = Get-Command signtool.exe -ErrorAction SilentlyContinue
        if ($command) { $SignToolPath = $command.Source }
    }
    if (-not $SignToolPath) {
        $kitsBin = Join-Path ${env:ProgramFiles(x86)} 'Windows Kits\10\bin'
        if (Test-Path -LiteralPath $kitsBin -PathType Container) {
            $SignToolPath = Get-ChildItem -LiteralPath $kitsBin -Directory |
                Sort-Object Name -Descending |
                ForEach-Object { Join-Path $_.FullName 'x64\signtool.exe' } |
                Where-Object { Test-Path -LiteralPath $_ -PathType Leaf } |
                Select-Object -First 1
        }
    }
    Require-File $SignToolPath
    $signArgs = Get-CodeSignArguments $signCertificate $TimestampUrl
}

if (-not $SkipBuild) {
    & (Join-Path $PSScriptRoot 'build-release.ps1') -BuildDir $build -Msys2Root $Msys2Root
    if (-not $?) { throw 'Release build failed.' }
}

$cache = Join-Path $build 'CMakeCache.txt'
Require-File $cache
if (-not (Select-String -LiteralPath $cache -Pattern '^CMAKE_BUILD_TYPE:STRING=Release$' -Quiet)) {
    throw "Build directory is not configured for Release: $build"
}
$dist = Join-Path $build 'dist'
foreach ($relative in @('sorcery.exe', 'cfg\config.ini', 'dat\strings.ja.json',
                        'dat\fonts\sorcery-jp-sans-medium.ttf', 'sav\game.json')) {
    Require-File (Join-Path $dist $relative)
}

$versionLine = Select-String -LiteralPath (Join-Path $repo 'CMakeLists.txt') `
    -Pattern '^project\(sorcery VERSION ([0-9]+\.[0-9]+\.[0-9]+)\)' | Select-Object -First 1
if (-not $versionLine) { throw 'Unable to read the Sorcery version from CMakeLists.txt.' }
$version = $versionLine.Matches[0].Groups[1].Value
$installer = Join-Path $output "Sorcery-JA-$version-Setup.exe"
if ($Sign -and (Test-Path -LiteralPath $installer)) {
    throw "A signed output path must be empty: $installer"
}

$stage = Join-Path $repo ("build\installer-staging\" + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $stage -Force | Out-Null
Copy-Item -Path (Join-Path $dist '*') -Destination $stage -Recurse -Force
Copy-Item -LiteralPath (Join-Path $repo 'installer\installed.flag') -Destination $stage
Require-File (Join-Path $stage 'sorcery.exe')

# Installed builds start in Japanese. Keep the previous English default as a
# reference so an untouched per-user config from an older installer can migrate.
$stagedConfig = Join-Path $stage 'cfg\config.ini'
$legacyConfig = Join-Path $stage 'cfg\config.legacy-en.ini'
Copy-Item -LiteralPath $stagedConfig -Destination $legacyConfig
$configText = [IO.File]::ReadAllText($stagedConfig)
foreach ($pair in @(
    @('monospace = Wizardry 5 DOS Regular', 'monospace = Sorcery JP Sans Medium'),
    @('proportional = Marcellus Regular', 'proportional = Sorcery JP Sans Medium'),
    @('text = ProggyVector Regular', 'text = Sorcery JP Sans Medium'),
    @('language = en', 'language = ja')
)) {
    if ([regex]::Matches($configText, [regex]::Escape($pair[0])).Count -ne 1) {
        throw "Unexpected Release config default: $($pair[0])"
    }
    $configText = $configText.Replace($pair[0], $pair[1])
}
[IO.File]::WriteAllText($stagedConfig, $configText, [Text.UTF8Encoding]::new($false))

if ($Sign) {
    & $SignToolPath @signArgs (Join-Path $stage 'sorcery.exe')
    if ($LASTEXITCODE -ne 0) { throw "Signing the staged executable failed: $LASTEXITCODE" }
    & $SignToolPath verify /pa (Join-Path $stage 'sorcery.exe')
    if ($LASTEXITCODE -ne 0) { throw 'The staged executable signature did not verify.' }
}

New-Item -ItemType Directory -Path $output -Force | Out-Null
$isccArgs = @('/Qp', "/DPayloadDir=$stage", "/DAppVersion=$version", "/O$output")
if ($Sign) {
    $callback = Format-IsccSignCommand $SignToolPath $signArgs
    $isccArgs += '/DSignBuild=1'
    $isccArgs += "/Ssorcerysign=$callback"
}
$isccArgs += (Join-Path $repo 'installer\sorcery.iss')
& $ISCCPath @isccArgs
if ($LASTEXITCODE -ne 0) { throw "Inno Setup compilation failed: $LASTEXITCODE" }

Require-File $installer
if ($Sign) {
    & $SignToolPath verify /pa $installer
    if ($LASTEXITCODE -ne 0) { throw 'The installer signature did not verify.' }
}
$hash = (Get-FileHash -LiteralPath $installer -Algorithm SHA256).Hash.ToLowerInvariant()
Set-Content -LiteralPath "${installer}.sha256" -Value "$hash  $(Split-Path -Leaf $installer)" -Encoding ascii
Write-Output "Installer: $installer"
Write-Output "Staged payload: $stage"
Write-Output "SHA-256: $hash"
