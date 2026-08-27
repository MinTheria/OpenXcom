# OpenXcom fork changes

This document records the engine behavior added in this OpenXcom fork. It is
the index for changes that XPiratez submods or this workspace's runtime may
depend on.

## Scope and branch layout

The inventory below was audited on 2026-08-03 against upstream
`origin/oxce-plus` at `911ca487f` (`Meh`, following OXCE 8.6.1).

- `armor-loadouts` is the primary integration branch. Its documented tip is
  `d3e3edc9e`; it contains 23 fork commits from `ecbbfab1f` through
  `d3e3edc9e`.
- `animations` is a separate experimental line. Its documented tip is
  `f272cdf5a`; it contains three fork commits from `27e214134` through
  `f272cdf5a`.
- The two maintained lines have not been merged. A build of `armor-loadouts`
  does not contain melee swing animation, and a build of `animations` does not
  contain the features on `armor-loadouts`.
- Uncommitted working-tree edits are not part of the commit inventory. Audit
  them separately before building or updating this document.

### Pending working-tree change: quickdraw shortcut

The battlescape controls include a configurable **Draw Quickdraw Item (hold
Shift)** key, defaulting to Q and therefore used as Shift+Q. It draws the
topmost movable item from `STR_QD_SLOT`, preferring an empty right hand and
falling back to an empty left hand. The move uses the normal ruleset inventory
cost, including item-specific cost modifiers, and normal inventory behavior for
TU reserves.

The shortcut does not swap or stow occupied hand items. It rejects hands that
the item's `supportedInventorySections` disallow and never moves fixed items,
including XPiratez's fixed quickdraw-slot blockers and built-in weapons. Mods
without `STR_QD_SLOT` are unaffected.

### Pending working-tree change: extended Graphs history

The Geoscape Graphs screen now preserves every monthly graph sample instead of
discarding all but the latest twelve. This applies to funds, income,
expenditure, maintenance, research score, country funding, and country/region
XCom and alien activity. Existing saves remain compatible, but begin collecting
additional history only after being loaded by this build; data previously
discarded by an older build cannot be restored.

The newest twelve months remain the default window. Press **Page Up** for an
older 12-month window and **Page Down** to return toward the present. Holding
Shift while using the mouse wheel provides the same navigation; an unmodified
wheel continues to scroll country/region selector lists. Month and year labels
and every graph type follow the selected window.

### Pending working-tree change: off-chart unit stat maxima

The battlescape Unit Info screen extends the existing numeric off-chart maximum
from health to every stat row with a meaningful maximum. When a maximum is at
least 147 and the displayed current value differs, the right end of the bar
shows `/maximum`; this covers depleted time units, energy and mana,
health-reduced firing/throwing/melee accuracy, and damaged armor facings. The
original current number remains unchanged.

All maximum labels reuse the `stats.numMaxHealth` interface element for color,
position, and compatibility. Its `custom` bit 1 still forces maxima to display
when current equals maximum, and bit 2 controls the slash prefix.

### Pending working-tree change: soldier bonus recovery summary

The Soldier Bonuses summary now displays the combined per-turn recovery bonus
for time units, energy, morale, health, stun, and mana when every contributing
bonus defines that recovery as a direct integer `flatOne` value. Recovery rows
whose value depends on stats, current unit state, or ruleset scripts remain
unvalued because no single number can accurately summarize them outside battle.

### Pending working-tree change: craft-system Ufopaedia lookup

Middle-clicking an installed craft system now opens the Ufopaedia article of
its shared launcher item by default. This makes craft-specific system variants
such as the several Dragonscales slot rules resolve to their common discovered
article, both in the craft information screen and the armament selection list.
An explicit craft-weapon `ufopediaType` remains the highest-priority override;
ordinary craft weapons continue to use their craft-weapon type. The normal
Ufopaedia availability filtering still prevents undiscovered articles from
opening.

### Pending working-tree change: craft maintenance priority

The Craft Info screen has a per-craft selector for the first dockside
maintenance task: repair, fuel, or ammunition. The selection is saved as
`maintenancePriority` on the craft; absent values retain the original repair,
rearm, refuel order. Once the selected task is complete, remaining work follows
the original relative order.

Maintenance tasks that lack fuel or ammunition are skipped so other work can
continue. The task becomes eligible again when matching supplies arrive. A
craft with zero fuel remains unavailable while waiting for fuel, matching the
original launch-safety behavior.

### Pending working-tree change: post-mission item losses

The battlescape debriefing adds a fourth **Losses** page after recovered loot.
It reports the net reduction in each strategic item type between mission start
and completed recovery/reequipping, so it includes destroyed or abandoned base
defense equipment as well as expended ammunition, grenades, and other
consumables. If stores cannot restore a craft's configured loadout, the amount
removed from that craft's manifest is included too.

As with the existing recovered-loot comparison, vehicles and their ammunition
are excluded because base-defense setup converts them before the saved store
snapshot is taken. The report is a net stock change: recovered items of the
same type offset losses, and it does not distinguish consumption from battlefield
destruction. The result is shown only in the immediate debriefing and is not
added to persistent mission history.

### Pending working-tree change: dogfight mode mouse handling

Dogfight stance buttons now reserve their normal action for the left mouse
button. Right-click continues to apply a stance to every active interception;
middle-click and other mouse buttons no longer change a craft's stance without
updating the grouped-button selection indicator.

### Pending working-tree change: destination route preview

While selecting a Geoscape destination, moving the cursor over the globe now
draws the selected craft's prospective great-circle route. For a craft already
in flight, the preview replaces its old route until destination selection ends.
The preview uses the existing flight-path rendering and follows the
`globeFlightPaths` display option.

### Pending working-tree change: animated-door path costs

Battlescape pathfinding treats opening an animated UFO door as a stop before
movement resumes. The door's full TU activation cost is no longer modified by
walking, running, flying, or other movement-cost multipliers, while the movement
after it retains the selected movement mode's TU and energy costs. Opening the
door spends no energy, matching battlescape execution, but path availability
and preview coloring still account for the existing requirement to have energy
equal to half the door's TU cost at the moment it is opened.

This makes path selection, remaining-TU and remaining-energy markers, reserve
coloring, and the execution-time affordability check agree for routes through
closed animated doors. Ordinary swinging doors, which open without stopping
automatic movement, retain their existing combined movement cost.

### Pending working-tree change: quieter grouped UFO detections

UFOs assigned as escorts on supply missions are detected, tracked, displayed,
and interceptable normally, but no longer open a UFO-detected alert. Other uses
of the same UFO type continue to alert normally.

Existing-base hunt missions group newly detected UFO alerts by mission during
each half-hour detection pass. The first newly detected UFO opens the normal
alert; additional UFOs from that mission detected in the same pass are tracked
silently. UFOs first detected in a later pass can still alert. In the current
XPiratez rules this covers the grouped Ninja and T'Leth hideout-defense launches;
the single-UFO secret-base hunt is unchanged in practice.

The baseline and tips are recorded explicitly because branch names and remote
tracking refs can move. To reproduce the committed inventories:

```bash
git log --reverse --oneline 911ca487f..d3e3edc9e
git diff --stat 911ca487f..d3e3edc9e
git log --reverse --oneline 911ca487f..f272cdf5a
git diff --stat 911ca487f..f272cdf5a
```

## Primary `armor-loadouts` branch

### Armor-driven equipment layouts

The engine loads a new top-level `armorLoadouts` ruleset collection keyed by
armor `type`. Changing to a configured armor replaces the soldier's saved
equipment layout, and the layout is regenerated against the actual available
inventory before deployment. Geoscape armor changes also move required items
between base and craft storage where possible.

Generation is ordered and availability-aware. It accounts for strength after
bonuses, armor and item weight, fixed/occupied slots, item dimensions, loaded
ammo, spare ammo, and the inventory sections supported by each item. An entry's
choices are tried in order, allowing preferred equipment with fallbacks.

Ruleset surface:

```yaml
armorLoadouts:
  - type: STR_ARMOR_ID
    capacityOffset: 0
    entries:
      - slot: STR_RIGHT_HAND       # optional
        slotX: 0                   # optional; use with slotY
        slotY: 0
        count: 1                   # max is an alias; min also marks it required
        required: false
        overCapacityAllowance: 0
        choices:
          - STR_SIMPLE_ITEM
          - item: STR_WEAPON
            ammo:
              - [STR_FIRST_CLIP, STR_FALLBACK_CLIP]
            spareAmmo: 1
            allowUnloaded: false
```

See the [Armor Loadouts README](https://github.com/MinTheria/XPiratez_mods/blob/mods/Piratez%20Armor%20Loadouts/README.md)
and [ruleset](https://github.com/MinTheria/XPiratez_mods/blob/mods/Piratez%20Armor%20Loadouts/Ruleset/armor_loadouts.rul)
for the maintained XPiratez policy and inventory slot IDs. The implementation is centered in `RuleArmorLoadout`,
`Soldier::applyArmorLoadout`, `InventoryState`, and `BattlescapeGenerator`.

Commits: `ecbbfab1f`.

### Typed hangars and compact facilities

Craft rules accept `hangarType`; base facilities accept
`allowedCraftHangarTypes`. An empty facility allow-list keeps upstream's
unrestricted behavior. A restricted facility rejects untagged craft and craft
whose tag is not listed.

Hangar capacity is resolved as a compatibility matching problem, not just a
slot count. Existing craft, craft transfers, queued craft production, purchases,
facility replacement, and new manufacture orders are checked together.
Restricted berths are considered first so compatible compact craft prefer them
while unrestricted hangars remain available to larger craft. The same matching
drives physical craft-to-facility placement in the base view.

The companion `Piratez Small Hangar` submod defines a restricted 1x1 berth and
tags supported light craft. See the
[Small Hangar README](https://github.com/MinTheria/XPiratez_mods/blob/mods/Piratez%20Small%20Hangar/README.md).

Commit: `b6ba2bf59`.

### Automatic manufacturing

Manufacture rules accept an `automaticOrder` map. The engine reconciles these
orders once per game hour, after arrivals and before the normal production step,
and marks them with an `[A]` prefix in the manufacture screen.

| Mode | Required fields | Engine behavior |
| --- | --- | --- |
| `maintainStock` | `item`, positive `target` | Schedules enough recipe runs to reach projected stock, including transfers and recipe yield. Optional `tierGroup` plus positive `tier` forms an upgrade ladder. |
| `consumeAll` | none | Schedules every complete run supported by currently stored inputs. |
| `consumeExcessResearch` | one recipe input consumed by finite research | Keeps the globally required interrogation reserve and consumes only the captive surplus allocated to the base. Repeatable research prevents consumption. |
| `infiniteAutoSell` | `tierGroup`, positive `tier` | Runs the highest available tier indefinitely with autosell initially enabled. The player's sell/keep choice follows the tier group. |

The reconciler respects research, base services, money, materials, workshop
space, and manual projects. It adopts only compatible unstaffed finite manual
orders, uses only free or automation-owned runts, prioritizes finite work, and
reclaims automatic sales runts when finite work appears. Tier replacements
finish the already-paid current unit before switching. Stopping an automatic
order suppresses it at that base until its trigger clears. Suppression and
tier-group sell preferences are persisted in the base save data; productions
also persist whether they are automatic.

Each base also has a persistent **Auto-stock** toggle on Current Production.
It defaults to enabled and controls only `maintainStock` orders. Disabling it
immediately releases their runts and pauses reconciliation without cancelling a
paid-for current unit; re-enabling resumes normal reconciliation. The save field
is `autoMaintainStock` (written only when disabled).

The later revisions add yield-aware stock targets, research-aware captive
reserves, tiered stock and sales selection, same-hour runt redistribution, and
cached global research calculations. Completion/failure popups are suppressed
only for the quiet automatic cases; `consumeAll` summaries and relevant
`infiniteAutoSell` failures remain visible.

The detailed scheduling contract and examples live in the
[Automatic Manufacturing README](https://github.com/MinTheria/XPiratez_mods/blob/mods/Piratez%20Automatic%20Manufacturing/README.md).

Commits: `d49a51c85`, `1f723ff00`, `f2edcd528`, `e34db425c`,
`108930b1c`, `6d7059ba2`, `1b4a10b7a`.

### Automatic training allocation

The martial- and psi-training screens have a persistent per-base **Auto Fill**
toggle. Manually removing a soldier while auto-fill is active records a
persistent per-soldier exclusion; manually adding them clears it.
The toggle is positioned in the top header row immediately left of the
bonus-stat **+** control so it does not overlap the training column header.

- Martial training fills vacancies in base roster order, skipping wounded,
  fully trained, already assigned, and excluded soldiers. It is refreshed daily.
- Psi training removes ineligible/fully trained assignments, then prioritizes
  soldiers whose intrinsic psi skill is already positive before sorting each
  group in descending psi-strength order. This continues candidates who are
  already showing results without comparing the deliberately hidden negative
  initial-training countdown; commendation and transformation bonuses are not
  considered. Wounds do not prevent psi training. It is refreshed daily, at
  the monthly psi-training boundary, and when enabled.

The save fields are `autoFillTraining` and `autoFillPsiTraining` on a base, and
`autoTrainingExcluded` and `autoPsiTrainingExcluded` on a soldier.

Commits: `d49a51c85`, `fd2b00950`.

### Ruleset and script extensions

| Interface | Meaning |
| --- | --- |
| Root `finisherExperienceChance` | Integer clamped to 0-100. If a direct player attack changes an active target to dead or unconscious and its scripted XP multiplier is positive, this is the minimum XP roll chance. Delayed damage is unaffected. |
| Root `battleTypeInventorySlotOrder` | Map from numeric battle type to ordered inventory-section IDs. It is tried by Ctrl-click placement and automatic unit equipment after per-item defaults and before generic fallback. Ground entries are ignored. |
| Item `defaultReactionFireDisabled` | Initializes that hand's reaction-fire-disabled flag for player units once, after fresh pre-battle equipment is complete. Tactical saves and later player choices are preserved. |
| Armor script `alienInventoryIndicator` | Inspection hook with output variables `visible` and `color` and input `unit`. When visible, alien inspection draws a bordered palette swatch. For variable shields, the engine also supports the compatibility tags `VAR_SHIELD_APPLIED` and `VAR_SHIELD_FLASH_COLOR`. |
| `SavedBattleGame.getMissionType` | Returns the current battlescape mission type to ruleset scripts, or empty text without a battle. This lets effects exclude specific mission types. |

Companion documentation and examples:

- [Finisher XP](https://github.com/MinTheria/XPiratez_mods/blob/mods/Piratez%20Finisher%20XP/README.md)
- [Inventory Defaults](https://github.com/MinTheria/XPiratez_mods/blob/mods/Piratez%20Inventory%20Defaults/README.md)
- [Reaction Defaults](https://github.com/MinTheria/XPiratez_mods/blob/mods/Piratez%20Reaction%20Defaults/README.md)
- [Variable Enemy Shields](https://github.com/MinTheria/XPiratez_mods/blob/mods/Piratez%20Variable%20Enemy%20Shields/README.md)
- [Pain Response TUs](https://github.com/MinTheria/XPiratez_mods/blob/mods/Piratez%20Pain%20Response%20TUs/README.md)

Commits: `d49a51c85`, `1f723ff00`, `26730099f`, `562667fd0`,
`f02276820`.

### Interface and quality-of-life changes

- Transfer destination selection shows remaining stores, or remaining capacity
  for the relevant prison type when moving captives.
- Selecting a ready craft without enough pilots from the intercept screen opens
  that craft's information screen for assignment. Returning refreshes its pilot
  status. This transition skips the mismatched-palette popup animation.
- Player inventory displays armor-derived melee dodge when the extended stats
  option is enabled.
- Alt-hover damage details and middle-click Ufopaedia lookup work over an empty
  hand when the unit has an innate/special weapon configured for empty-hand use.
- After a mission, the medals screen also provides mission statistics grouped by
  participant and weapon, with hostile kills and stuns. It opens directly when
  there are statistics but no new medals and provides a page toggle when both
  exist. The statistics page uses compact plural KILLS/STUNS headers and a
  selectable list sized for the narrower count columns.
- The same post-mission screen rotates onward to a Wounds page that lists final
  missing health and mana for every participating soldier-backed unit. The
  values are captured before the tactical battle data is discarded, including
  when health or mana is configured to replenish immediately after missions.
- Commendation checks recalculate total stat gain from current minus initial
  stats, allowing awards such as Super Size after non-mission/monthly gains.
- Soldier and craft-assignment lists show missing mana by default, without
  changing the manually maintained original roster order, when mana is enabled
  and not replenished after missions.
- Hidden item categories remain hidden in purchase, sell, and transfer category
  filters.

Commits: `d49a51c85`, `88b56c1d1`, `4209d3495`, `198301b31`,
`2f422f842`, `93dc4f8be`, `5f94d9e1c`, `43fe87d04`, `75c775a11`,
`d8f941751`, `d3e3edc9e`.

## Experimental `animations` branch

This branch adds a configurable held-weapon wind-up to melee attacks. Damage is
delayed until the final animation phase. Only the item that initiated the attack
is animated, avoiding the prior dual-wield behavior where both held sprites
moved.

Items can use the legacy scalar form:

```yaml
items:
  - type: STR_MELEE_WEAPON
    meleeSwingFrames: 3
    meleeSwingReach: 6
```

or a baked keyframe arc produced by the animation studio:

```yaml
items:
  - type: STR_MELEE_WEAPON
    meleeSwing:
      reach: 6
      frames:
        - { forward: -1, lateral: 0, rot: 2, lift: 0 }
        - { forward:  0, lateral: 0, rot: 1, lift: -1 }
        - { forward:  1, lateral: 0, rot: -1, lift: 0 }
```

The phase advances every four game ticks and impact lands when the final phase
is entered. `forward` and `lateral` are projected relative to the unit's facing
and scaled by `reach`; `lift` is an absolute vertical pixel offset; `rot`
selects an adjacent HANDOB facing frame as a rotation effect.
Scalar rules synthesize the original procedural arc. With no swing fields, melee
keeps the upstream instant behavior. `BattleUnit.getMeleeAnimPhase` exposes the
current phase to scripts.

Known limitations:

- The swing applies only to attacks using `MeleeAttackBState`; melee-like actions
  routed through projectile/ranged code do not use it.
- Swing phase and initiating-item state are not serialized. Saving or otherwise
  interrupting the battle mid-swing is not supported.
- This feature must be merged or rebased with `armor-loadouts` before both sets
  can be present in one executable.

The test rules, tuning notes, and edge-case checklist are in the
[Melee Swing Test README](https://github.com/MinTheria/XPiratez_mods/blob/mods/Piratez%20Melee%20Swing%20Test/README.md).

Commits: `27e214134`, `3c21f6541`, `f272cdf5a`.

## Commit coverage for `armor-loadouts`

This table is a completeness check for the primary branch. Every fork commit in
the audited range is represented above.

| Commit | Subject / documented area |
| --- | --- |
| `ecbbfab1f` | Armor loadouts v1 |
| `b6ba2bf59` | Typed 1x1 hangar support |
| `d49a51c85` | Initial automation, training, transfer capacity, shield indicator, reaction defaults, finisher XP |
| `1f723ff00` | Reaction-default timing and automatic manufacturing update |
| `88b56c1d1`, `4209d3495`, `198301b31` | Pilot assignment workflow and palette-transition fix |
| `f2edcd528`, `e34db425c` | Automatic runt allocation and popup behavior fixes |
| `562667fd0` | Battle-type inventory slot defaults |
| `2f422f842` | Commendation stat-gain recalculation |
| `fd2b00950` | Psi-training auto-fill |
| `26730099f` | Variable-shield swatch color fix |
| `93dc4f8be` | Melee dodge in inventory |
| `f02276820` | Mission type exposed to scripts |
| `108930b1c`, `6d7059ba2`, `1b4a10b7a` | Expanded and optimized automatic manufacturing |
| `5f94d9e1c` | Empty-hand hover and middle-click support |
| `43fe87d04` | Hidden inventory categories respected |
| `75c775a11` | Post-mission kill/stun statistics |
| `d8f941751` | Missing mana shown by default in soldier lists |
| `d3e3edc9e` | Mission-statistics window layout and header polish |

## Maintenance

Keep this document synchronized with engine work. When a fork commit is added,
changed, rebased, merged, or dropped:

1. Record each maintained branch's new upstream merge-base and tip.
2. Audit both the commit log and cumulative diff; do not rely on commit subjects
   alone.
3. Update behavior, ruleset keys, script hooks, save fields, compatibility notes,
   known limitations, and companion submod links.
4. Add every new fork commit to a feature section and the coverage table.
5. Check the working tree separately so unfinished changes are not silently
   described as committed behavior.
