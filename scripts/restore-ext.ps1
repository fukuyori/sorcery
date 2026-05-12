param(
    [switch]$Force
)

$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSScriptRoot
$ext = Join-Path $root "ext"
New-Item -ItemType Directory -Force -Path $ext | Out-Null

$repos = @(
    @{ Name = "imgui"; Url = "https://github.com/ocornut/imgui.git" },
    @{ Name = "imgui_toggle"; Url = "https://github.com/cmdwtf/imgui_toggle.git" },
    @{ Name = "imgui_sugar"; Url = "https://github.com/mnesarco/imgui_sugar.git" },
    @{ Name = "imgui_spinner"; Url = "https://github.com/dalerank/imspinner.git" },
    @{ Name = "cereal"; Url = "https://github.com/USCiLab/cereal.git" },
    @{ Name = "glm"; Url = "https://github.com/g-truc/glm.git" },
    @{ Name = "magic_enum"; Url = "https://github.com/Neargye/magic_enum.git" },
    @{ Name = "simpleini"; Url = "https://github.com/brofield/simpleini.git" },
    @{ Name = "sqlite_modern_cpp"; Url = "https://github.com/SqliteModernCpp/sqlite_modern_cpp.git" },
    @{ Name = "stb"; Url = "https://github.com/nothings/stb.git" }
)

foreach ($repo in $repos) {
    $target = Join-Path $ext $repo.Name
    if (Test-Path $target) {
        if ($Force) {
            Remove-Item -LiteralPath $target -Recurse -Force
        } else {
            Write-Host "Skipping existing $($repo.Name)"
            continue
        }
    }

    git clone --depth 1 $repo.Url $target
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to clone $($repo.Url)"
    }
}

$toggleRenderer = Join-Path $ext "imgui_toggle\imgui_toggle_renderer.cpp"
if (Test-Path $toggleRenderer) {
    $content = Get-Content -Raw -LiteralPath $toggleRenderer
    $old = "    _state.KnobInset = ImLerp(_config.Off.KnobInset, _config.On.KnobInset, _animationPercent);"
    $new = @"
    _state.KnobInset = ImOffsetRect(
        ImLerp(_config.Off.KnobInset.Top, _config.On.KnobInset.Top, _animationPercent),
        ImLerp(_config.Off.KnobInset.Left, _config.On.KnobInset.Left, _animationPercent),
        ImLerp(_config.Off.KnobInset.Bottom, _config.On.KnobInset.Bottom, _animationPercent),
        ImLerp(_config.Off.KnobInset.Right, _config.On.KnobInset.Right, _animationPercent));
"@
    if ($content.Contains($old)) {
        Set-Content -LiteralPath $toggleRenderer -Value $content.Replace($old, $new) -NoNewline
    }
}
