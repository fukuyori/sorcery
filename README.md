# Sorcery (Windows / Japanese edition)

[日本語](README.ja.md)

This repository is a fork of <https://github.com/davemoore22/sorcery>: a Japanese
localised version that builds and runs on Windows. The changes from upstream and
the Japanese display setup are described first; the original README follows
verbatim after the horizontal rule.

## Changes from upstream

### Windows build and runtime support

- Added `vcpkg.json` so the native dependencies (SDL2, SDL2_image, GLEW, jsoncpp,
  FreeType, FFmpeg, sqlite3, ...) resolve through vcpkg.
- Made the `find_package` calls in `CMakeLists.txt` work with vcpkg CONFIG-mode
  packages, selecting imported targets such as `SDL2::SDL2`, `GLEW::GLEW` and
  `JsonCpp::JsonCpp` depending on what the environment provides.
- Added `scripts/restore-ext.ps1` (fetches the third-party sources under `ext/`)
  and `scripts/build-windows.ps1` (builds with Visual Studio and CMake).
- Reworked shared headers such as `inc/common/macro.hpp`, `parse.hpp` and
  `json.hpp` so the code compiles with MSVC.
- Data and config paths are now resolved relative to the executable, so the game
  starts from any working directory.
- Save data is stored at `%APPDATA%\Sorcery\save.db3`; the required tables are
  created on startup when the file does not exist. The bundled `dat/save.db3` was
  removed and is now ignored by git.
- Build notes for Windows and Linux are in `doc/WINDOWS.md` and `doc/LINUX.md`.

### Japanese localisation

- Added `dat/strings.ja.json`, translating every key (948) of `dat/strings.json`.
  Class, race, alignment and spell names stay in romaji, following the project
  convention.
- Added overlay loading (`load_overlay`) to `StringStore`. With
  `[Localization] language = ja`, `strings.ja.json` is loaded on top of the
  English table and untranslated keys fall back to English.
- Ability labels in the character detail and summary panels now go through
  string keys (`CHARACTER_*`) so they follow the active language.
- Menu items are centred by measured pixel width via ImGui's
  `SelectableTextAlign` instead of `std::format` width padding, which fixes the
  misalignment of full-width (East Asian) text.

### Fonts

- The font directory is resolved as `dat/` next to the executable.
- `.ttc` (TrueType Collection) files are loaded in addition to `.ttf`.
- New optional `[Font]` keys: `rasterizer_multiply` (synthetic bold) and
  `glyph_extra_advance_x` (extra glyph spacing).
- ImGui's Japanese glyph range is enabled.
- Font names in `config.ini` are matched case-insensitively, because FreeType
  reports the style of some fonts in lower case (`bold`, `regular`).
- M+ FONTS (M+ FONTS PROJECT / Coji Morishita, SIL Open Font License 1.1,
  <https://github.com/rayshan/mplus-fonts>) can be used as the Japanese font; the
  full licence text is bundled as `dat/OFL-mplus.txt`.

### Gameplay

- Added `Game::give_item_to_party`, an item award helper that handles the
  party-inventory-full case.
- Added a `Chest` type skeleton: trap type and outcome enums plus stubs for
  Calfo / inspect identification, disarm and open.

### Documentation

- Added `doc/WIZARDRY_CODE_REFERENCE_PLAN.md`: an implementation plan based on
  the Wizardry.Code reference, including a spell behaviour audit checklist
  (Latumapic, Manifo, Loktofeit, Haman, Mahaman, Montino, Mamorlis, Mabadi).

## Japanese display setup

Japanese display needs a font with Japanese glyphs and a change to
`cfg/config.ini`.

### 1. Place the fonts

Download M+ FONTS (SIL OFL 1.1, full text in `dat/OFL-mplus.txt`) from
<https://github.com/rayshan/mplus-fonts/tree/master/fonts> and put these three
files under `dat/`:

- `mplus-1p-regular.ttf`
- `mplus-2m-bold.ttf`
- `mplus-2p-bold.ttf`

### 2. Edit the config file

`cfg/config.ini` already contains the Japanese settings as commented-out lines.
Remove the leading `;` from those lines and delete (or comment out) the original
English lines.

Before:

```ini
[Font]
size = 24
; 日本語表示用 (dat/ に mplus-*.ttf を配置した場合):
; monospace = M+ 2m Bold
; proportional = M+ 2p Bold
; text = M+ 1p Regular
monospace = Wizardry 5 DOS Regular
proportional = Marcellus Regular
text = ProggyVector Regular

[Localization]
; 日本語表示用:
; language = ja
language = en
```

After:

```ini
[Font]
size = 24
monospace = M+ 2m Bold
proportional = M+ 2p Bold
text = M+ 1p Regular

[Localization]
language = ja
```

Font names are matched case-insensitively.

### 3. Note: the build overwrites the config file

The post-build step copies the repository's `cfg/` and `dat/` into the build
directory (for example `build-windows-ninja/cfg/` and `build-windows-ninja/dat/`).
If you edited `config.ini` in the build directory directly, every build replaces
it with the repository copy, so back it up first or edit the repository's
`cfg/config.ini` instead.

---

The original README from upstream (<https://github.com/davemoore22/sorcery>,
branch `sdl/imgui`) follows verbatim.

# Sorcery

An open source remake/remaster initially of the classic CRPG Wizardry, Proving Grounds of the Mad Overlord, then hopefully of the Sequels II (The Diamond Knight), III (Legacy of Llylgamyn), and V (Heart of the Maelstrom). Currently under heavy development with a release hopefully later in 2026.

Written using ImGui/SDL2/OpenGL and C++

**(Updated 4th July 2026)**

![](/promo/screen7.png)
![](/promo/screen25.png)
![](/promo/screen23.png)
![](/promo/screen20.png)
![](/promo/screen18.png)
![](/promo/screen15.png)

## Progress Update

* ~~Load/Save/New Game~~ (DONE)
* ~~Town/Castle Menus~~ (DONE)
* ~~Character Creation~~ (DONE)
* ~~Character Editing~~ (DONE)
* ~~Party/Expedition Management~~ (DONE)
* ~~Maze Generation and Navigation~~ (DONE)
* ~~Dungeon Features~~ (DONE)
* ~~Automapping~~ (DONE)
* ~~Keyboard/Mouse/Gamepad Control~~ (DONE)
* ~~Context-Sensitive-Help~~ (DONE)
* ~~Gilgamesh's Tavern~~ (DONE)
* ~~Adventurer's Inn~~ (DONE)
* ~~Levelling up/Changing Class~~ (DONE)
* ~~Temple~~ (DONE)
* ~~Wipe Handling~~ (DONE)
* ~~Dungeon Events~~ (DONE)
* ~~Compendium~~ (DONE)
* ~~Music~~ (DONE)
* ~~Automap~~ (DONE)
* **Inventory and Item Manipulation (DOING)**
* **Special Item Powers and Handling (DOING)**
* **Boltac's Equipment Shop (DOING)**
* Creatures and NPCs (TO DO)
* Magic and Combat (TO DO)
* Victory Handling (TO DO)
* Strict (Legacy) Mode (TO DO)
* Achivements (TO DO)
* Optional Textured/Rendered 3D View (TO DO)

## Wishlist

* Sequels (II, III, V)
* Procedurally-Generated Infinite Content (like Diablo)
* Modding
* Plugin/3D Graphics

## What next? (as of June 2026)

Currently adding Item Usage/Invocation, as well as buying, selling, uncursing, and identifying at Boltac's. Normal inventory management has been mosty completed!

Initially I am planning to remake Wizardry 1, then adding 2 and 3, and then subsequently providing infinite procedurally-generated content like that of Diablo for example. In the meantime, check out Snarfu's bugfix and recompilation of the original Wizardry for Apple II at <https://github.com/snafaru>.

## Compilation

I'm currently developing solely on Linux (Ubuntu 26) so compilation instructions are currently Linux-specific. Windows instructions and releases will of course be supported at a later date.

Full compliation instructions for Linux are in the doc/COMPILE text file, but in brief use Cmake and optionally VSCode with CMakeTools. C++23 compiler support is required (I'm currently using GCC 14.2).

All code is released under the GPL - see the doc/LICENSE text file for full details.

## Contact and Help

All comments and criticism and help welcome. You can find me as *Typhon* on the C++ Help and DungeonCrawlers.org Discord Servers, or my email address is on the Splash Screen of the Game.

## Credits

Graphics used (all free/open source) are by:

* Torio - <https://www.pixiv.net/en/users/5887541>
* Various - <https://game-icons.net/>

Music used (Creative Commonds) is by:

* Kevin MacLeod - <https://incompetech.com/music/royalty-free/music.html>

## Videos

Watch some (older) gameplay:

[![Compendium Browsing](https://img.youtube.com/vi/TSj7Rw3l3Iw/maxresdefault.jpg)](https://youtu.be/TSj7Rw3l3Iw)
[![Entering the Dungeon](https://img.youtube.com/vi/AQ9LhK0ta8A/maxresdefault.jpg)](https://youtu.be/AQ9LhK0ta8A)
[![Dungeon Events](https://img.youtube.com/vi/RG25iYrPBlg/maxresdefault.jpg)](https://youtu.be/RG25iYrPBlg)

See other gameplay videos at <https://www.youtube.com/channel/UCEkmGGFyxu5vH6xAHej3Dag>

## Screenshots

![](/promo/screen1.png)
![](/promo/screen24.png)
![](/promo/screen22.png)
![](/promo/screen16.png)
![](/promo/screen21.png)
![](/promo/screen17.png)
![](/promo/screen5.png)
![](/promo/screen14.png)
![](/promo/screen8.png)
![](/promo/screen13.png)
![](/promo/screen12.png)
![](/promo/screen11.png)
![](/promo/screen9.png)
![](/promo/screen10.png)
![](/promo/screen6.png)
![](/promo/screen2.png)
![](/promo/screen3.png)
![](/promo/screen4.png)
