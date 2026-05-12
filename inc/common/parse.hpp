#pragma once

#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>

namespace Sorcery {

inline auto PARSE_INT(std::string_view value) -> int {

	try {
		const auto parsed{std::stoll(std::string{value})};
		if (parsed > std::numeric_limits<int>::max())
			return std::numeric_limits<int>::max();
		if (parsed < std::numeric_limits<int>::min())
			return std::numeric_limits<int>::min();
		return static_cast<int>(parsed);
	} catch (const std::out_of_range &) {
		return std::numeric_limits<int>::max();
	}
}

inline auto PARSE_UINT(std::string_view value) -> unsigned int {

	try {
		const auto parsed{std::stoull(std::string{value})};
		if (parsed > std::numeric_limits<unsigned int>::max())
			return std::numeric_limits<unsigned int>::max();
		return static_cast<unsigned int>(parsed);
	} catch (const std::out_of_range &) {
		return std::numeric_limits<unsigned int>::max();
	}
}

}
