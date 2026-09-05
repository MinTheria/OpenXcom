# OpenXcom fork changes

This document records the engine behavior added in this OpenXcom fork. It is
the index for changes that XPiratez submods or this workspace's runtime may
depend on.

## Scope and branch layout

The primary branch inventory was refreshed on 2026-09-05 for integration of
`origin/oxce-plus` at `931537818` (OXCE 8.6.6). The pre-merge fork tip is
`35d814a05`, containing 44 fork commits since `911ca487f`. The integration
commit containing this document is the new primary tip after fast-forwarding
`armor-loadouts`; its upstream merge-base is `931537818`.

- `armor-loadouts` is the primary integration branch. All features below are
  committed behavior, including the former “pending working-tree” sections.
- `animations` remains a separate experimental line at `f272cdf5a`, containing
  three fork commits since its upstream merge-base `911ca487f`. It has not been
  merged into the primary branch; primary builds do not contain melee animation.
- The 38 incoming commits include a merge and release/maintenance commits;
  their cumulative change affects 79 files. No primary fork feature is dropped.

### OXCE 8.6.6 integration behavior

The upstream voice-set rules, loader, UI, build entries, and optional
`voiceSetID` save fields coexist with `armorLoadouts`. Existing response sound
arrays remain supported. Voice selection and avatar-reset behavior now follow
upstream; armor changes retain the fork's equipment-layout generation.

Both soldier-list screens honor a nonzero `oxceBaseSoldierInfoColumnDefault`.
With default zero (including an invalid value reset to zero), missing mana is
shown when mana is enabled and not replenished after missions. Initialization
only selects the displayed column: it does not sort or modify manual roster
order. Upstream's Alt-selection stores the preferred column; explicitly sorting
continues to behave as before.

Upstream `extendedIgnoreOverweightRule` defaults to false and only bypasses its
normal auto-equipping weight check. Armor-loadout generation retains its own
strength, `capacityOffset`, and `overCapacityAllowance` limits.

Debrief scoring now uses upstream's separate kill/capture/civilian/VIP values,
while retaining fork losses, statistics, and wounds pages. The default penalty
for player-killed civilians uses `(value * 5) / 3`, whose integer rounding can
differ from the previous `value + 2 * (value / 3)`. Mind-control kills increment
the controller's kill counter. Friendly melee ignores dodge in the default
script, with `is_same_faction` available to custom melee scripts.

Upstream's blocked-AI movement counter coexists with the fork's door, flying,
vertical-turn, and strafe path-cost fixes. Manufacture touch controls and
transfer-cost fixes coexist with automatic orders and typed hangars.

Validation passed on 2026-09-05: source/shell checks, a Release build through
`tools/build_openxcom.sh --source OpenXcom-integration --cmake`, and XPiratez
startup/log checks. The deployed binary matched the integration build by SHA-256.
`user/openxcom.log` confirmed 8.6.6, the enabled Piratez submods, successful data
and language loading, and startup, with no ERROR or WARN entries. Existing
suppressed ruleset diagnostics remain; the compiler reports one unused YAML
helper warning in unchanged code.
Interactive gameplay acceptance remains a manual follow-up; this inventory does
not claim those scenarios have been playtested.

### Committed feature: quickdraw shortcut

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

### Committed feature: extended Graphs history

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

### Committed feature: off-chart unit stat maxima

The battlescape Unit Info screen extends the existing numeric off-chart maximum
from health to every stat row with a meaningful maximum. When a maximum is at
least 147 and the displayed current value differs, the right end of the bar
shows `/maximum`; this covers depleted time units, energy and mana,
health-reduced firing/throwing/melee accuracy, and damaged armor facings. The
original current number remains unchanged.

All maximum labels reuse the `stats.numMaxHealth` interface element for color,
position, and compatibility. Its `custom` bit 1 still forces maxima to display
when current equals maximum, and bit 2 controls the slash prefix.

### Committed feature: soldier bonus recovery summary

The Soldier Bonuses summary now displays the combined per-turn recovery bonus
for time units, energy, morale, health, stun, and mana when every contributing
bonus defines that recovery as a direct integer `flatOne` value. Recovery rows
whose value depends on stats, current unit state, or ruleset scripts remain
unvalued because no single number can accurately summarize them outside battle.

### Committed feature: craft-system Ufopaedia lookup

Middle-clicking an installed craft system now opens the Ufopaedia article of
its shared launcher item by default. This makes craft-specific system variants
such as the several Dragonscales slot rules resolve to their common discovered
article, both in the craft information screen and the armament selection list.
An explicit craft-weapon `ufopediaType` remains the highest-priority override;
ordinary craft weapons continue to use their craft-weapon type. The normal
Ufopaedia availability filtering still prevents undiscovered articles from
opening.

### Committed feature: craft maintenance priority

The Craft Info screen has a per-craft selector for the first dockside
maintenance task: repair, fuel, or ammunition. The selection is saved as
`maintenancePriority` on the craft; absent values retain the original repair,
rearm, refuel order. Once the selected task is complete, remaining work follows
the original relative order.

Maintenance tasks that lack fuel or ammunition are skipped so other work can
continue. The task becomes eligible again when matching supplies arrive. A
craft with zero fuel remains unavailable while waiting for fuel, matching the
original launch-safety behavior.

### Committed feature: post-mission item losses

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

### Committed feature: dogfight mode mouse handling

Dogfight stance buttons now reserve their normal action for the left mouse
button. Right-click continues to apply a stance to every active interception;
middle-click and other mouse buttons no longer change a craft's stance without
updating the grouped-button selection indicator.

### Committed feature: destination route preview

While selecting a Geoscape destination, moving the cursor over the globe now
draws the selected craft's prospective great-circle route. For a craft already
in flight, the preview replaces its old route until destination selection ends.
The preview uses the existing flight-path rendering and follows the
`globeFlightPaths` display option.

### Committed feature: animated-door path costs

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

An animated door becomes movement-cost-free as soon as its opening animation
starts. The unit still waits for the animation to finish, but an intervening
movement-state update no longer charges the already-paid door cost again and
incorrectly aborts a low-TU route. This also removes the result's dependency on
whether the door animation timer advances before the movement timer.

### Committed feature: vertical path-preview turn costs

The battlescape path preview no longer treats an initial vertical move as a
horizontal turn for armor with `turnBeforeFirstStep` enabled. Moving up or down
first therefore displays the same TU cost regardless of the unit's facing and
matches movement execution, which does not turn the unit before vertical
movement. Initial horizontal turns retain their armor-defined turn cost.

### Committed feature: cheaper airborne route selection

For flying units, an unobstructed straight-line path is now treated as a fast
candidate rather than the final result. A* is allowed to replace it only with a
strictly cheaper route, permitting armor whose airborne movement costs less
than grounded movement to take off, cross above the ground, and land even when
the direct ground line is unobstructed. If no cheaper route exists, the original
straight path and its cost are retained.

Non-flying units and guided missiles retain the original straight-line fast
path. The A* result now also preserves the total cost of the selected goal node
for callers that query it after path calculation.

### Committed feature: one-tile strafe cost consistency

One-tile strafing previews now omit initial facing-turn costs, matching
execution: a valid strafe preserves facing without performing chargeable turns
and pays only its armor-defined strafe movement cost. This is particularly
visible on armor with `turnBeforeFirstStep` and a non-default `turnCost`.

If the requested direction cannot be animated as a strafe, such as a backward
move outside the supported facing arc, the action is downgraded and recalculated
as normal movement before preview and execution. The fallback therefore uses
normal movement modifiers and applies the armor's initial-turn policy instead
of retaining a stale strafe flag that made the physical turn free.

### Committed feature: quieter grouped UFO detections

UFOs assigned as escorts on supply missions are detected, tracked, displayed,
and interceptable normally, but no longer open a UFO-detected alert. Other uses
of the same UFO type continue to alert normally.

Existing-base hunt missions group newly detected UFO alerts by mission during
each half-hour detection pass. The first newly detected UFO opens the normal
alert; additional UFOs from that mission detected in the same pass are tracked
silently. UFOs first detected in a later pass can still alert. In the current
XPiratez rules this covers the grouped Ninja and T'Leth hideout-defense launches;
the single-UFO secret-base hunt is unchanged in practice.

The baseline and pre-integration tips are recorded explicitly because branch
names and remote tracking refs can move. To reproduce the feature inventories:

```bash
git log --reverse --oneline 911ca487f..35d814a05
git diff --stat 911ca487f..35d814a05
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
- Soldier and craft-assignment lists show missing mana as the default fallback,
  without changing manual roster order, when mana is enabled and not replenished
  after missions. An explicit upstream default column takes precedence.
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

This table covers all 44 primary fork commits before integration. The merge
containing this inventory additionally incorporates upstream through `931537818`
and resolves soldier-column defaults as documented above.

| Commit | Subject / documented area |
| --- | --- |
| `ecbbfab1f` | armor loadouts v1 |
| `b6ba2bf59` | 1x1 hangar |
| `d49a51c85` | Destination Store/Prison capacity on shipping screen, automatic manufacturing, automatic training capacity fill, shield color marker on inspect, reaction fire off by default support, some of these require submods to function |
| `1f723ff00` | Reaction fire default fix, automatic manufacturing update |
| `88b56c1d1` | Streamlined pilot assignment |
| `4209d3495` | Fixed streamlined pilot assignment |
| `198301b31` | Bypassed palette flash by skipping the animation |
| `f2edcd528` | Automated runt allocation fix |
| `e34db425c` | Re enabled popups for consumeAll and infiniteAutoSell orders |
| `562667fd0` | Inventory defaults support |
| `2f422f842` | Changed stat gain to recalculate during commendation check, making Super Size awardable at monthly report without mission participation |
| `fd2b00950` | Voodoo training autofill |
| `26730099f` | Shield color tag fix |
| `93dc4f8be` | Added melee dodge to inventory screen for player units |
| `f02276820` | Variable shields & Pain Response TUs mission type exclusion support |
| `108930b1c` | Automated manufacture expansion, yield-aware stocks, tiered stock maintainenance, research-aware captive enslavement, finish current project before switching to a higher tier |
| `6d7059ba2` | Treat automated runts as free |
| `1b4a10b7a` | Massive performance improvements for the Automated Manufactury calcs |
| `5f94d9e1c` | Alt-click & middle-click support for 'empty' hands |
| `43fe87d04` | Inventory defaults translation key and hidden: true |
| `75c775a11` | Post mission kill counter |
| `d8f941751` | Show unfreshness by default |
| `d3e3edc9e` | Mission statistics window UI changes |
| `9dc1427d5` | Maintain stock per-base toggle button |
| `3599fcb95` | Added translation key |
| `3c2a4bfb2` | Charts historical support added |
| `34668a172` | Two minor fixes for historical charts |
| `4c39ac6d6` | Show /max stats for other than HP as well |
| `35c29f828` | Middle clicking craft/ship system brings up the ufopaedia entry |
| `43e2840a1` | Net losses debriefing screen, might've run into a transfer state bug after this, not sure though |
| `5afd910c7` | Moved auto-fill buttons, added flat recovery bonuses to bonuses summary |
| `3bd3358a0` | Ship rearm/refuel/repair priority button |
| `1a3591e02` | Middle mouse no longer works during Dogfight (used to work without UI responding) |
| `0d77706c7` | Automatic Voodoo training uses intrinsic voodoo skill > 0 before voodoo power ranking in unit selection |
| `86a1ee82d` | Ship path preview |
| `3e0ac6a94` | Wound and Freshness damage post mission debrief window |
| `872707443` | Supply ship and base defense waves only give one alert on Geoscape |
| `0f470eb37` | Quickdraw keybind |
| `35c249ce0` | docs: add fork changes inventory |
| `b1549ab27` | Path preview updated to match pathfinding, fixes running through doors preview showing incorrect values |
| `a415b78a3` | Door opening caused an extra TU deduction (which was refunded), resulting in incorrect 'no tus' warning when low on tus |
| `2ba9eb8ab` | Vertical path-preview bug fix for armors with turnBeforeFirstStep enabled |
| `82e548f99` | Pathfinding sometimes failed to find the fastest route for flying units. |
| `35d814a05` | Strafing preview and execution TU cost fixes for turnBeforeFirstStep enabled armors |

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
