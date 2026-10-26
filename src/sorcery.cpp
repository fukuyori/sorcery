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

#include <SDL.h> // Required by SDL defines

#include "core/application.hpp" // for Application
#include <memory>				// for make_unique
#ifdef _WIN32
#include <cstdio> // for freopen, stdout, stderr
#include <io.h>	  // for _fileno, _get_osfhandle
#endif

#ifdef _WIN32
/// @brief Attach a standard stream to NUL when it has no destination
/// @param stream The stream to check
/// @return
static auto attach_to_nul_if_missing(std::FILE *stream) -> void {

	// A GUI executable started without a console has no stdout/stderr, and std::println throws when it cannot
	// write, which terminates the game
	const auto fd{_fileno(stream)};
	if (fd < 0 || _get_osfhandle(fd) < 0)
		std::freopen("NUL", "w", stream);
}
#endif

/// @brief Executablele Entry Point
/// @param argc Command Line Argument Count
/// @param argv Command Line Arguments
/// @return An exit code
auto main(int argc, char *argv[]) -> int {

#ifdef _WIN32
	attach_to_nul_if_missing(stdout);
	attach_to_nul_if_missing(stderr);
#endif

	// Create an Application Instance
	auto app{std::make_unique<Sorcery::Application>(argc, argv)};

	// Start it
	const auto exit_code{app->start()};

	// Stop it
	app->stop();

	// Quit
	return exit_code;
}