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

#pragma once

#include "common/cereal.hpp"
#include "types/item.hpp"

#include <vector>

namespace Sorcery {

class Character;
struct Context;
class Game;

// Skeleton for chest encounters (post-combat treasure with optional trap).
// Operations stub the trap identify / disarm / open flow described in
// doc/WIZARDRY_CODE_REFERENCE_PLAN.md; treasure is routed through
// Game::give_item_to_party so the inventory-full case is handled consistently.
class Chest {

	public:
		enum class TrapType {
			NONE = 0,
			POISON_NEEDLE = 1,
			GAS_BOMB = 2,
			CROSSBOW_BOLT = 3,
			EXPLODING_BOX = 4,
			STUNNER = 5,
			TELEPORTER = 6,
			MAGE_BLASTER = 7,
			PRIEST_BLASTER = 8,
			ALARM = 9
		};

		enum class Outcome {
			NONE = 0,
			TRAP_IDENTIFIED = 1,
			TRAP_MISIDENTIFIED = 2,
			TRAP_DISARMED = 3,
			TRAP_DISARM_FAILED = 4,
			TRAP_TRIGGERED = 5,
			OPENED_NO_TRAP = 6,
			TREASURE_AWARDED = 7,
			TREASURE_PARTY_INVENTORY_FULL = 8
		};

		Chest();
		Chest(TrapType trap, std::vector<Item> contents);

		template <class Archive> auto serialize(Archive &archive) -> void {
			archive(_trap, _perceived_trap, _identified, _disarmed, _opened,
					_contents);
		}

		auto trap_type() const -> TrapType;
		auto perceived_trap_type() const -> TrapType;
		auto is_identified() const -> bool;
		auto is_disarmed() const -> bool;
		auto is_opened() const -> bool;
		auto contents() const -> const std::vector<Item> &;

		// Calfo: priest spell with a fixed 95% trap identification chance.
		auto identify_trap_with_calfo(Context &ctx) -> Outcome;

		// Thief / Ninja inspect: rolls d100 against the inspector's
		// IDENTIFY_TRAP ability percentage.
		auto identify_trap_with_inspect(Context &ctx,
										const Character &inspector) -> Outcome;

		// Thief / Ninja disarm: rolls d100 against the disarmer's
		// BASE_DISARM_TRAP. A failed roll may trigger the trap immediately
		// (ACTIVATE_TRAP check).
		auto disarm_trap(Context &ctx, const Character &disarmer) -> Outcome;

		// Open the chest. If an armed trap remains, returns TRAP_TRIGGERED
		// and contents are not awarded. Otherwise each item is routed
		// through Game::give_item_to_party; the first inventory-full case
		// stops and returns TREASURE_PARTY_INVENTORY_FULL with the
		// remaining items still in the chest.
		auto open(Game &game) -> Outcome;

	private:
		TrapType _trap;
		TrapType _perceived_trap;
		bool _identified;
		bool _disarmed;
		bool _opened;
		std::vector<Item> _contents;
};

}
