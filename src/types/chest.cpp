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

#include "types/chest.hpp"
#include "common/enum.hpp"
#include "core/context.hpp"
#include "types/character.hpp"
#include "types/game.hpp"

Sorcery::Chest::Chest()
	: _trap{TrapType::NONE},
	  _perceived_trap{TrapType::NONE},
	  _identified{false},
	  _disarmed{false},
	  _opened{false},
	  _contents{} {
}

Sorcery::Chest::Chest(TrapType trap, std::vector<Item> contents)
	: _trap{trap},
	  _perceived_trap{TrapType::NONE},
	  _identified{false},
	  _disarmed{false},
	  _opened{false},
	  _contents{std::move(contents)} {
}

auto Sorcery::Chest::trap_type() const -> TrapType {

	return _trap;
}

auto Sorcery::Chest::perceived_trap_type() const -> TrapType {

	return _perceived_trap;
}

auto Sorcery::Chest::is_identified() const -> bool {

	return _identified;
}

auto Sorcery::Chest::is_disarmed() const -> bool {

	return _disarmed;
}

auto Sorcery::Chest::is_opened() const -> bool {

	return _opened;
}

auto Sorcery::Chest::contents() const -> const std::vector<Item> & {

	return _contents;
}

auto Sorcery::Chest::identify_trap_with_calfo(Context &ctx) -> Outcome {

	constexpr unsigned int calfo_accuracy_pct{95};

	_identified = true;
	const auto roll{ctx.get_random(Enums::System::Random::D100)};
	if (roll <= calfo_accuracy_pct) {
		_perceived_trap = _trap;
		return Outcome::TRAP_IDENTIFIED;
	}

	_perceived_trap = TrapType::NONE;
	return Outcome::TRAP_MISIDENTIFIED;
}

auto Sorcery::Chest::identify_trap_with_inspect(Context &ctx,
												const Character &inspector)
	-> Outcome {

	using enum Enums::Character::Ability;

	const auto &abilities{inspector.abilities()};
	const auto it{abilities.find(IDENTIFY_TRAP)};
	const int chance{it != abilities.end() ? it->second : 0};

	_identified = true;
	const auto roll{ctx.get_random(Enums::System::Random::D100)};
	if (static_cast<int>(roll) <= chance) {
		_perceived_trap = _trap;
		return Outcome::TRAP_IDENTIFIED;
	}

	_perceived_trap = TrapType::NONE;
	return Outcome::TRAP_MISIDENTIFIED;
}

auto Sorcery::Chest::disarm_trap(Context &ctx, const Character &disarmer)
	-> Outcome {

	using enum Enums::Character::Ability;

	if (_trap == TrapType::NONE) {
		_disarmed = true;
		return Outcome::TRAP_DISARMED;
	}
	if (_disarmed)
		return Outcome::TRAP_DISARMED;

	const auto &abilities{disarmer.abilities()};

	const auto disarm_it{abilities.find(BASE_DISARM_TRAP)};
	const int disarm_chance{disarm_it != abilities.end() ? disarm_it->second
														 : 0};
	const auto disarm_roll{ctx.get_random(Enums::System::Random::D100)};
	if (static_cast<int>(disarm_roll) <= disarm_chance) {
		_disarmed = true;
		return Outcome::TRAP_DISARMED;
	}

	const auto trigger_it{abilities.find(ACTIVATE_TRAP)};
	const int trigger_chance{trigger_it != abilities.end() ? trigger_it->second
														   : 100};
	const auto trigger_roll{ctx.get_random(Enums::System::Random::D100)};
	if (static_cast<int>(trigger_roll) <= trigger_chance)
		return Outcome::TRAP_TRIGGERED;

	return Outcome::TRAP_DISARM_FAILED;
}

auto Sorcery::Chest::open(Game &game) -> Outcome {

	if (_opened)
		return Outcome::OPENED_NO_TRAP;

	if (!_disarmed && _trap != TrapType::NONE)
		return Outcome::TRAP_TRIGGERED;

	_opened = true;
	if (_contents.empty())
		return Outcome::OPENED_NO_TRAP;

	for (auto it = _contents.begin(); it != _contents.end();) {
		const auto result{game.give_item_to_party(*it)};
		if (result == Game::GiveItemToPartyResult::GIVEN)
			it = _contents.erase(it);
		else
			return Outcome::TREASURE_PARTY_INVENTORY_FULL;
	}

	return Outcome::TREASURE_AWARDED;
}
