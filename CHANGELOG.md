# Changelog

Changes to the Windows and Japanese fork of Sorcery are recorded here.
Fork releases are tagged `v<version>-ja`; 0.1.0-ja is based on upstream Alpha #1.
The upstream project history remains in the [upstream README](README.md#upstream-readme-unchanged).

This file follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [Unreleased]

## [0.1.0-ja] - 2026-09-24

First release of the fork, labelled "Alpha #1 JA". This is an alpha preview
release; the game is incomplete, and saves and settings may not carry over to
later versions.

### Added

- Japanese string overlay, `language = ja` configuration, and Japanese
  translations for the current English string keys.
- Bundled `Sorcery JP Sans Medium` font, its SIL Open Font License, and a
  reproducible generation script.
- TrueType collection scanning and optional font rasterization and spacing
  settings.
- Japanese font setup and translation review documents.
- Windows Release build and Inno Setup installer scripts, with optional signing.

### Changed

- Ported the fork to the upstream Alpha #1 source layout and MSYS2 UCRT64 /
  GCC build environment.
- Centered Japanese menu choices and adjusted title screen text and
  confirmation dialog sizes.
- Linked the Windows executable as a GUI application so starting it does not
  create a console window.
- Stored settings and saves separately for each user in an all-users Windows
  installation.
- Set the default configuration of installed and portable builds to
  Japanese. Installed copies migrate only untouched English defaults from
  earlier installer builds.

### Fixed

- Preserved English strings when the Japanese overlay fails to load and
  reapplied the overlay after a string reload.
- Created the save directories needed by a fresh build.
- Excluded Windows system DLLs from the runtime dependency copy.
- Continued without music instead of terminating when the audio device
  cannot be opened.

### Removed

The following old fork changes were not carried over to the Alpha #1 base:

- MSVC / vcpkg build files, compatibility headers, vendored `ext/` content,
  and the old Windows build scripts. The current fork uses MSYS2 UCRT64 / GCC.
- The `%APPDATA%\Sorcery\save.db3` save location, superseded by upstream's
  JSON files under `sav/`.
- The old `Chest` skeleton, since upstream now supplies its implementation.
- Separate `doc/WINDOWS.md` and `doc/LINUX.md` build guides, superseded by
  upstream's `doc/COMPILE.md`.
- The old `Game::give_item_to_party` change. Its need will be reconsidered
  once upstream's game library changes reach upstream `master`.

The decision on `doc/WIZARDRY_CODE_REFERENCE_PLAN.md` is deferred; see the
[fork design](docs/FORK_DESIGN.ja.md).
