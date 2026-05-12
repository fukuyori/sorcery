// Copyright (C) 2026 Dave Moore
//
// This file is part of Sorcery.
//
// Sorcery is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 2 of the License, or (at your option) any later
// version.
//
// Sorcery is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// Sorcery.  If not, see <http://www.gnu.org/licenses/>.
//
// If you modify this program, or any covered work, by linking or combining
// it with the libraries referred to in README (or a modified version of
// said libraries), containing parts covered by the terms of said libraries,
// the licensors of this program grant you additional permission to convey
// the resulting work.

#include "resources/filestore.hpp"
#include "common/define.hpp"
#include "resources/define.hpp"

#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#else
#include <limits.h>
#include <unistd.h>
#endif

Sorcery::FileStore::FileStore() {

	// Generate and add the file paths
	_base_path = _get_exe_path();
	_file_paths.clear();

	_add_path(CONFIG_DIR, CONFIG_FILE);
#ifdef _WIN32
	if (const auto *appdata = std::getenv("APPDATA"); appdata != nullptr) {
		const auto save_dir{std::filesystem::path{appdata} / "Sorcery"};
		std::filesystem::create_directories(save_dir);
		_file_paths[DATABASE_FILE] = save_dir / DATABASE_FILE;
	} else
		_add_path(DATA_DIR, DATABASE_FILE);
#else
	_add_path(DATA_DIR, DATABASE_FILE);
#endif
	_add_path(DATA_DIR, ITEMS_FILE);
	_add_path(DATA_DIR, LAYOUT_FILE);
	_add_path(DATA_DIR, MAPS_FILE);
	_add_path(DATA_DIR, MONOSPACE_1_APPLE2_FILE);
	_add_path(DATA_DIR, MONOSPACE_1_C64_FILE);
	_add_path(DATA_DIR, MONOSPACE_1_DOS_FILE);
	_add_path(DATA_DIR, MONOSPACE_1_MSX2_FILE);
	_add_path(DATA_DIR, MONOSPACE_5_DOS_FILE);
	_add_path(DATA_DIR, MONOSPACE_5_FMTOWNS_FILE);
	_add_path(DATA_DIR, DEFAULT_MONOSPACE_FONT_FILE);
	_add_path(DATA_DIR, MONSTERS_FILE);
	_add_path(DATA_DIR, PROPORTIONAL_FONT_FILE);
	_add_path(DATA_DIR, STRINGS_FILE);
	_add_path(DATA_DIR, STRINGS_JA_FILE);
	_add_path(DATA_DIR, TEXT_FONT_FILE);
	_add_path(DOCUMENTS_DIR, LICENSE_FILE);
	_add_path(GFX_DIR, BACKGROUND_MAIN_MENU_TEXTURE);
	_add_path(GFX_DIR, BACKGROUNDS_TEXTURE);
	_add_path(GFX_DIR, BANNER_TEXTURE);
	_add_path(GFX_DIR, ICONS_TEXTURE);
	_add_path(GFX_DIR, ITEMS_TEXTURE);
	_add_path(GFX_DIR, KNOWN_CREATURES_TEXTURE);
	_add_path(GFX_DIR, LOGO_TEXTURE);
	_add_path(GFX_DIR, MAPS_TEXTURE);
	_add_path(GFX_DIR, UNKNOWN_CREATURES_TEXTURE);
	_add_path(GFX_DIR, WIREFRAME_TEXTURE);
	_add_path(SFX_DIR, ENGINE_MUSIC);
	_add_path(SFX_DIR, MAINMENU_MUSIC);
	_add_path(SFX_DIR, TOWN_MUSIC);
	_add_path(VFX_DIR, MAINMENU_VIDEO);
}

auto Sorcery::FileStore::get(std::string_view key) const
	-> std::filesystem::path {

	return _file_paths.contains(key) ? _file_paths.at(key)
									 : std::filesystem::path{FILE_NOT_FOUND};
}

auto Sorcery::FileStore::get_path(std::string_view key) const -> std::string {

	return _file_paths.contains(key) ? _file_paths.at(key).string()
									 : FILE_NOT_FOUND;
}

auto Sorcery::FileStore::get_base_path() const -> std::filesystem::path {

	return _base_path;
}

auto Sorcery::FileStore::_add_path(const std::string_view dir,
								   const std::string_view file) -> void {

	const std::filesystem::path file_path{_base_path / dir / file};
	_file_paths[file] = file_path;
}

auto Sorcery::FileStore::_get_exe_path() const -> std::filesystem::path {

#ifdef __linux__

	// On Linux
	{
		char result[PATH_MAX];
		if (const ssize_t count{readlink("/proc/self/exe", result, PATH_MAX)};
			count != -1) {
			return std::filesystem::path{std::string_view{result,
														  static_cast<size_t>(
															  count)}}
				.parent_path();
		} else
			return std::filesystem::current_path();
	}

#elif _WIN32

	// On Windows
	{
		std::wstring path;
		DWORD copied{0};
		do {
			path.resize(path.size() + MAX_PATH);
			copied = GetModuleFileNameW(nullptr, path.data(),
										static_cast<DWORD>(path.size()));
		} while (copied >= path.size());

		if (copied == 0)
			return std::filesystem::current_path();

		path.resize(copied);
		return std::filesystem::path{path}.parent_path();
	}
#else
	return std::filesystem::current_path();
#endif
}
