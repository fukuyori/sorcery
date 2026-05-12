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

#include "core/system.hpp"
#include "common/define.hpp"
#include "common/enum.hpp"
#include "common/sdl2.hpp"
#include "core/animation.hpp"
#include "core/audioplayer.hpp"
#include "core/database.hpp"
#include "core/macro.hpp"
#include "core/random.hpp"
#include "resources/filestore.hpp"
#include "resources/stringstore.hpp"
#include "types/config.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>

namespace {

auto trace_system(std::string_view message) -> void {

	if (std::getenv("SORCERY_TRACE_STARTUP") != nullptr) {
		std::cerr << "[system] " << message << std::endl;
		std::ofstream log{"sorcery-startup.log", std::ios::app};
		log << "[system] " << message << '\n';
	}
}

}

Sorcery::System::System([[maybe_unused]] int argc,
						[[maybe_unused]] char **argv) {

	// Initialise SDL Audio first as it's a dependency of AudioPlayer, but we
	// initialise the video subsystem in Display as it's not needed until then
	trace_system("SDL_Init audio");
	if (SDL_Init(SDL_INIT_AUDIO) != 0) {
		std::println("Error: {}", SDL_GetError());
	} else {

		// Modules
		trace_system("filestore");
		files = std::make_unique<FileStore>();

		trace_system("config ini");
		_settings = std::make_unique<CSimpleIniA>();
		_settings->SetUnicode();
		_settings->LoadFile(CSTR(files->get_path(CONFIG_FILE)));

		trace_system("config");
		config =
			std::make_unique<Config>(_settings.get(), files->get(CONFIG_FILE));
		trace_system("strings");
		strings = std::make_unique<StringStore>(files->get_path(STRINGS_FILE));

		auto language{config->get("Localization", "language")};
		std::ranges::transform(language.begin(), language.end(),
							   language.begin(), [](unsigned char c) {
								   return std::tolower(c);
							   });
		if (language == "ja" || language == "ja-jp")
			strings->load_overlay(files->get_path(STRINGS_JA_FILE));

		trace_system("random");
		random = std::make_unique<Random>();
		trace_system("animation");
		animation = std::make_unique<Animation>(random.get());
		trace_system("database");
		db = std::make_unique<Database>(CSTR(files->get_path(DATABASE_FILE)));
		trace_system("audio player");
		audio = std::make_unique<AudioPlayer>();
		trace_system("done");
	}
}

Sorcery::System::~System() {}

// Diceroll to String
auto Sorcery::System::dice_roll_to_str(const std::string &message,
									   const int dice, const int roll,
									   const int needed) const -> std::string {

	return std::format("d{:<3}: {:>3}/{:>3}: {}", dice, roll, needed, message);
}

// Timepoint to String
auto Sorcery::System::convert_tp_to_str(
	const std::chrono::time_point<std::chrono::system_clock> tp) const
	-> std::string {

	// Need to do it this way til std::chrono::locate_zone etc is supported
	auto t_t{std::chrono::system_clock::to_time_t(tp)};
	char t_s[std::size("yyyy-mm-dd hh:mm:ss")];
	std::strftime(std::data(t_s), std::size(t_s), "%F %T", std::gmtime(&t_t));
	std::string ts(t_s);

	return ts;
}
