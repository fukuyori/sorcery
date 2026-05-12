# Wizardry.Code reference plan

This document records candidate Sorcery work items informed by
`snafaru/Wizardry.Code`, especially the v3.2 README and the
`list-of-fixes-Wizardry.Code` fix catalogue.

The reference repository is an Apple II / UCSD Pascal project. Treat it as a
behavioural reference, not as source to copy into Sorcery.

Reference:

- https://github.com/snafaru/Wizardry.Code
- https://github.com/snafaru/Wizardry.Code/tree/main/list-of-fixes-Wizardry.Code

## Principles

- Use the reference repository to identify expected Wizardry behaviour,
  historical bugs, and quality-of-life improvements.
- Re-implement behaviour in Sorcery's C++ codebase using local data structures,
  settings, and UI patterns.
- Keep legacy-compatible behaviour behind existing or new strict/recommended
  mode settings when the change affects balance or original quirks.
- Prefer small verifiable changes over broad combat or spell rewrites.

## High-priority candidates

### Inventory full treasure handling

Reference item: `WC033.Inventory-Full-Fix`.

Problem described by the reference: treasure could be assigned to a random
character whose inventory was full, causing the item to be lost even if another
party member had room.

Sorcery status:

- `Inventory::get_empty_slots()` and `Inventory::add_type()` already exist.
- Party membership is available through `State::get_party_characters()`.
- `Game::give_item_to_party(const Item &, bool log_inventory_full)` is
  implemented in `src/types/game.cpp` and returns `GiveItemToPartyResult`
  (`GIVEN` / `NO_PARTY_MEMBERS` / `PARTY_INVENTORY_FULL`). It scans party
  members via `State::get_party_characters()` and inserts into the first
  member with a free slot.
- `dat/strings.json` and `dat/strings.ja.json` already define
  `GAME_MESSAGE_PARTY_INVENTORY_FULL`, emitted via `state->add_log_message(...)`
  when no member has room.
- Current callers: `Game::_debug_give_party_random_items` only. Future
  treasure/chest/combat reward flows should route through this helper.

Remaining work:

1. Wire treasure/chest/combat reward paths through `give_item_to_party` as
   those flows are implemented.
2. Decide whether to surface `NO_PARTY_MEMBERS` to the player or treat it as a
   programming error (currently silent in either log path).
3. Consider an overload returning the recipient `char_id` if a future caller
   needs to attribute the find to a specific character for messaging.

Acceptance:

- A treasure item is not lost while any party member has an empty slot.
- If all party inventories are full, no item is added and a message is emitted.

### Chest traps and disarm handling

Reference item: `WC037.Disarm-Trap-Bug` and related chest-trap notes.

Sorcery status:

- Trap and thief capability text already exists in string data.
- Chest and treasure implementation appears incomplete or still emerging.

Plan:

1. Locate the eventual chest-open/disarm flow before implementation.
2. Define explicit outcomes: identified, disarmed, triggered, treasure awarded,
   and no-space-for-treasure.
3. Keep Calfo's 95 percent trap identification behaviour in the spell/task
   specification.
4. Add tests or debug probes around thief agility, trap identification, and
   failed disarm paths once the flow exists.

Acceptance:

- Trap identification and disarm outcomes are deterministic from logged rolls.
- Failed disarm does not bypass trap effects accidentally.
- Treasure award uses the inventory-full-safe helper.

### Spell behaviour audit

Reference items include `WC003.Latumapic-Does-Not-Work`,
`WC006.Haman-Mahaman`, `WC028.Loktofeit`, `WC014.Manifo`,
`WC009.Montino`, `WC012.Mamorlis`, and `WC013.Mabadi`.

Sorcery status:

- Spell metadata is present in `SpellStore` and `dat/strings.json`.
- Full combat spell behaviour is not yet a good target for direct patching.

Plan:

1. Create a spell behaviour checklist before implementing combat spell effects.
2. For each referenced spell, record expected scope, success chance, and side
   effects.
3. Implement field spells separately from combat spells where possible.
4. Add strict/recommended behaviour flags for effects that intentionally differ
   from the original Apple II behaviour.

Initial checklist:

- Latumapic: should have a visible, persistent enemy-identification effect.
- Manifo: should have a success profile comparable to Katino.
- Loktofeit: should use the intended success chance and consequences.
- Haman/Mahaman: should expose the intended effect list.
- Montino/Mamorlis/Mabadi: verify target scope, resistance, and status changes.

Acceptance:

- Each implemented spell has a short behaviour note and a repeatable debug or
  test path.
- Combat-only spells are not marked complete before combat flow exists.

## Medium-priority candidates

### Combat status and probability display

Reference items: `WC018.Display-Hit-Probability`,
`WC020.Show-Spells-During-Combat`, and
`WC044.Display-silenced-character-status`.

Plan:

1. Keep this as a UI requirement for the combat implementation phase.
2. Display hit chance only when the underlying combat roll formula is stable.
3. Show spell names for both player and monster casts.
4. Surface silence state in the combat status area.

Acceptance:

- Combat logs or UI expose enough information to debug hit and spell outcomes.
- Silence is visible before it changes player choice or monster behaviour.

### Drain and surprise-round rules

Reference items: `WC027.Drain-Only-Once-Per-Combat-Enhancement` and
`WC023.Surprise-Round-Enhancement`.

Sorcery status:

- `curable_draining` and `surprise_spellcasting` settings already exist.
- Monster level-drain data is loaded.

Plan:

1. During combat implementation, track whether each monster/group has already
   applied level drain in the current combat.
2. Respect `surprise_spellcasting` when presenting or resolving spell actions in
   surprise rounds.
3. Keep original-compatible behaviour available when strict mode requires it.

Acceptance:

- A monster cannot drain the same character repeatedly in one combat unless a
  mode setting explicitly permits legacy behaviour.
- Surprise-round spellcasting follows the configured rule.

### Ninja balance and class-change rules

Reference item: `WC030.Ninja-Enhancements`.

Plan:

1. Treat Ninja changes as balance changes, not bug fixes.
2. Compare current Sorcery Ninja requirements, unarmed damage, armor class, and
   initiative assumptions before changing values.
3. Put any non-original enhancement behind recommended mode or an explicit
   balance option.

Acceptance:

- Strict mode can preserve original-like requirements.
- Recommended mode may adopt quality-of-life or balance improvements with
  documented values.

## Low-priority or blocked candidates

- Victory handling and Werdna reward flow: useful later, but blocked until
  combat rewards and final encounter handling exist.
- Friendly/no-friends monster behaviour: useful once encounter negotiation or
  friendly monsters are implemented.
- Optional rendering or display improvements: defer until gameplay systems are
  stable.

## Suggested next implementation slice

The inventory-safe item award helper (`Game::give_item_to_party`) is already
implemented and used by the random-item debug path. The next small, local slice
that builds on it is to define the chest open/disarm flow skeleton, since the
"Inventory and Item Manipulation" work is in progress and chest rewards will
naturally route through the existing helper.

Proposed next task — chest flow skeleton:

1. Locate or introduce the chest interaction entry point in the dungeon event
   path.
2. Define an outcome enum covering identified, disarmed, triggered, treasure
   awarded, and party-inventory-full (the last mapping straight to
   `GiveItemToPartyResult::PARTY_INVENTORY_FULL`).
3. Stub the trap identification path so Calfo's 95 percent figure and a thief's
   agility check both feed into the same outcome enum, even if the dice rolls
   are placeholders initially.
4. Route any awarded treasure through `Game::give_item_to_party` so the
   inventory-full case is handled consistently from the start.
