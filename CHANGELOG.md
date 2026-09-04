# Changelog

All notable changes to this fork of <https://github.com/davemoore22/sorcery>
are documented here. The format is based on
[Keep a Changelog](https://keepachangelog.com/en/1.1.0/). There are no version
numbers yet, so entries are grouped by date.

Upstream changes merged into this fork are not listed individually; see the
upstream repository's history for those.

## [Unreleased]

### Added

- `README.ja.md`: Japanese version of the fork-specific README sections.
- `dat/OFL-mplus.txt`: SIL Open Font License 1.1 text for M+ FONTS.
- `CHANGELOG.md` (this file).
- Commented-out Japanese settings in `cfg/config.ini` (`[Font]` M+ font names
  and `[Localization] language = ja`) so they can be restored after the build
  overwrites the config file.

### Changed

- `README.md` is now written in English and starts with the fork notice, the
  changes from upstream and the Japanese display setup; the upstream README
  follows verbatim after a horizontal rule.
- Merge of upstream `sdl/imgui` (up to `94506135`, 2026-07-04) in progress.

### Fixed

- Font names in `config.ini` are matched case-insensitively. FreeType reports
  the style of some fonts in lower case (for example `M+ 2m bold`), so
  `M+ 2m Bold` in the config silently fell back to ImGui's built-in font and
  Japanese text rendered as `?`.

## 2026-05-12

### Added

- Windows build and runtime support:
  - `vcpkg.json` for the native dependencies.
  - `scripts/restore-ext.ps1` and `scripts/build-windows.ps1`.
  - CMake support for vcpkg CONFIG-mode packages and imported targets
    (`SDL2::SDL2`, `GLEW::GLEW`, `JsonCpp::JsonCpp`, ...).
  - MSVC-compatible shared headers (`inc/common/macro.hpp`, `parse.hpp`,
    `json.hpp`).
  - Save data stored at `%APPDATA%\Sorcery\save.db3`; tables are created on
    startup when the file does not exist.
- Japanese localisation:
  - `dat/strings.ja.json` covering all 948 keys of `dat/strings.json`. Class,
    race, alignment and spell names stay in romaji.
  - `StringStore::load_overlay`: with `[Localization] language = ja` the
    Japanese table is loaded over the English one; untranslated keys fall
    back to English.
- Fonts:
  - `.ttc` (TrueType Collection) files are loaded in addition to `.ttf`.
  - Optional `[Font]` keys `rasterizer_multiply` (synthetic bold) and
    `glyph_extra_advance_x` (extra glyph spacing).
  - ImGui's Japanese glyph range enabled for all loaded fonts.
- Gameplay:
  - `Game::give_item_to_party`, an item award helper that handles the
    party-inventory-full case.
  - `Chest` type skeleton with trap type and outcome enums and stubs for
    Calfo / inspect identification, disarm and open.
- Documentation:
  - `doc/WINDOWS.md` and `doc/LINUX.md` build notes.
  - `doc/WIZARDRY_CODE_REFERENCE_PLAN.md`, including a spell behaviour audit
    checklist (Latumapic, Manifo, Loktofeit, Haman, Mahaman, Montino,
    Mamorlis, Mabadi).

### Changed

- Data and config paths are resolved relative to the executable instead of the
  current working directory.
- Ability labels in the character detail and summary panels use `CHARACTER_*`
  string keys so they follow the active language.
- Menu items are centred by measured pixel width (ImGui `SelectableTextAlign`)
  instead of `std::format` width padding.

### Removed

- Bundled `dat/save.db3`; the file is now ignored by git.

### Fixed

- Misaligned menu items when the labels contain full-width (East Asian) text.
- Font directory lookup when the game is launched from a different working
  directory.
