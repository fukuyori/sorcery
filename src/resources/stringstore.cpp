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

#include "resources/stringstore.hpp"
#include "resources/define.hpp"

#include <fstream>
#include <iostream>
#include <json/reader.h>
#include <json/value.h>
#include <string>
#include <string_view>
#include <utility>

Sorcery::StringStore::StringStore(const std::filesystem::path &filename)
	: _filename{filename} {

	// Load strings from file
	_loaded = _load_file(_filename, true);
}

auto Sorcery::StringStore::reload() -> void {

	_loaded = _load_file(_filename, true);
	if (_loaded && !_overlay_filename.empty() && !_load_file(_overlay_filename, false))
		std::cerr << "Unable to reload string overlay: " << _overlay_filename << '\n';
}

auto Sorcery::StringStore::load_overlay(const std::filesystem::path &filename) -> bool {

	_overlay_filename = filename;
	return _loaded && _load_file(filename, false);
}

auto Sorcery::StringStore::_load_file(const std::filesystem::path &filename, bool clear_existing) -> bool {

	std::ifstream file{filename};

	if (!file) {
		if (clear_existing)
			_strings.clear();
		return false;
	}

	Json::CharReaderBuilder builder{};
	Json::Value root{};
	std::string errors{};

	if (!Json::parseFromStream(builder, file, &root, &errors)) {
		if (clear_existing)
			_strings.clear();
		return false;
	}

	if (!root.isObject()) {
		if (clear_existing)
			_strings.clear();
		return false;
	}

	std::map<std::string, std::string, std::less<>> entries;
	for (const auto &key : root.getMemberNames()) {
		if (!root[key].isString()) {
			if (clear_existing)
				_strings.clear();
			return false;
		}
		entries[key] = root[key].asString();
	}

	if (clear_existing) {
		_strings = std::move(entries);
		_strings.try_emplace("NONE", STRINGS_NOT_LOADED);
	} else {
		for (auto &[key, value] : entries)
			_strings.insert_or_assign(std::move(key), std::move(value));
	}

	return true;
}

auto Sorcery::StringStore::get(const std::string_view key) const -> std::string {

	if (!_loaded)
		return STRINGS_NOT_LOADED;

	const auto it{_strings.find(std::string{key})};

	return it != _strings.end() ? it->second : KEY_NOT_FOUND;
}
