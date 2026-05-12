# Windows build notes

Sorcery can be built on Windows with Visual Studio 2022 and CMake once the
native dependencies and the ignored `ext/` third-party source directory are
available.

## Prerequisites

- Visual Studio 2022 with the Desktop development with C++ workload
- CMake 3.22 or newer
- vcpkg
- The repository's ignored `ext/` directory containing ImGui, imgui_toggle,
  imgui_sugar, cereal, glm, magic_enum, SimpleIni, and sqlite_modern_cpp

The native libraries are listed in `vcpkg.json`. Configure CMake with the vcpkg
toolchain file, for example:

```powershell
.\scripts\restore-ext.ps1
.\scripts\build-windows.ps1 -SkipRestoreExt
```

`restore-ext.ps1` clones public GitHub repositories into `ext/`, so it needs
direct GitHub access.

If `cmake` is not on `PATH`, `build-windows.ps1` will use the CMake bundled with
Visual Studio when it is installed.

If CMake fails at `find_package(SDL2)`, SDL2 is not installed or the vcpkg
toolchain/prefix path was not passed to CMake. If it fails with `Missing bundled
third-party sources under ext/`, restore the local `ext/` directory before
building.

## Save data

On Windows, save data is stored outside the repository at:

```text
%APPDATA%\Sorcery\save.db3
```

Delete that file to reset the local saved game state. The repository's
`dat/save.db3` file is ignored and should not be committed; the runtime creates
the needed database tables when the save file does not exist.

## Japanese localization

The runtime string table is loaded from `dat/strings.json` as UTF-8. Set
`[Localization] language = ja` in `cfg/config.ini` to overlay translated strings
from `dat/strings.ja.json`; untranslated keys fall back to English. The font
loader includes ImGui's Japanese glyph range, so a Japanese-capable TTF can be
added under `dat/` and selected in `cfg/config.ini`.
