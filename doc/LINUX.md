# Linux build notes

Sorcery is primarily developed on Ubuntu 24 with GCC 14.2. A C++23-capable
compiler, CMake, pkg-config, SDL2, SDL2_image, OpenGL, GLEW, JsonCpp, SQLite3,
Freetype, FFmpeg, libuuid, libbacktrace, and the ignored `ext/` third-party
source directory are required.

## Prerequisites

Install the native development packages for the libraries above using your
distribution package manager. Package names vary by distribution, but the CMake
configuration expects these packages to be discoverable:

- Threads
- SDL2
- SDL2_image
- OpenGL
- GLEW
- JsonCpp
- SQLite3
- pkg-config
- Freetype
- FFmpeg libraries: `libavformat`, `libavcodec`, `libswresample`, `libswscale`,
  and `libavutil`
- `uuid`
- `backtrace`

The project also expects the ignored `ext/` directory to contain the bundled
source dependencies used by the current SDL/ImGui branch, including ImGui,
imgui_toggle, imgui_sugar, cereal, glm, magic_enum, SimpleIni, and
sqlite_modern_cpp.

## Build

```sh
cmake -S . -B build
cmake --build build
```

If CMake reports missing bundled third-party sources under `ext/`, restore the
local `ext/` directory before building.

## Save data

On Linux, save data is stored as `dat/save.db3` under the executable's base
directory. This file is runtime state and should not be committed.
