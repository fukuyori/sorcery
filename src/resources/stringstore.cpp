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

#include <fstream>

#include "resources/define.hpp"
#include "resources/stringstore.hpp"
#include "common/json.hpp"

Sorcery::StringStore::StringStore(const std::string &filename)
	: _filename{filename} {

	// Load strings from file
	_loaded = _load();
}

auto Sorcery::StringStore::reload() -> void {

	_loaded = _load();
}

auto Sorcery::StringStore::_load() -> bool {

	return _load_file(_filename, true);
}

auto Sorcery::StringStore::load_overlay(const std::string &filename) -> bool {

	return _load_file(filename, false);
}

auto Sorcery::StringStore::_load_file(const std::string &filename,
									  bool clear_existing) -> bool {

	// Attempt to load the Strings File
	if (clear_existing) {
		_strings.clear();
		_strings["NONE"] = STRINGS_NOT_LOADED;
	}

	if (std::ifstream file{filename, std::ifstream::binary}; file.good()) {

		// Iterate through the file
		Json::Value root{};
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
		Json::Reader reader{};
#pragma GCC diagnostic pop
		if (reader.parse(file, root, false)) {
			for (Json::Value::iterator it = root.begin(); it != root.end();
				 ++it) {
				const auto string_key{it.key().asString()};
				const auto string_value{it->asString()};

				if (!string_key.empty())
					_strings[string_key] = string_value;
			}
		} else
			return false;
	} else
		return false;

	return true;
}

auto Sorcery::StringStore::get(std::string_view key) const -> std::string {

	if (_loaded)
		return _strings.contains(std::string{key})
				   ? _strings.at(std::string{key})
				   : KEY_NOT_FOUND;
	else
		return STRINGS_NOT_LOADED;
}
