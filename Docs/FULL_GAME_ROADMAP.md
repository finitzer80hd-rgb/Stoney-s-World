# Stoney's World — Full Game Production Plan

## Creative direction

Build an original, modern open-world crime-and-adventure game inspired by the broad genre ambitions of contemporary open-world games: a dense coastal region, systemic NPCs, drivable vehicles, missions, law enforcement, combat, exploration, and a reactive world. Stoney's World must use its own characters, story, locations, dialogue, visual identity, and mission designs; it is not a recreation of GTA VI or any proprietary assets or story.

## Target playable vertical slice

The first complete, finishable Windows prototype should deliver one polished district, one short story arc, repeatable activities, and a saveable player state. A small coherent world is the goal before expanding map size.

### Player loop
1. Explore on foot using the existing sprint, jump, slide, grapple, and glide mechanics.
2. Meet a contact and accept a mission.
3. Drive to a location, investigate or fight, and complete an objective.
4. Earn cash/reputation, manage health, and respond to a wanted level.
5. Escape or evade law enforcement, return to a safehouse, and save progress.

## Production phases

### Phase 0 — Stabilize the foundation
- Compile the existing UE 5.8 C++ project in Development Editor Win64.
- Fix compiler/UHT errors before layering on more systems.
- Confirm default game mode, input bindings, player possession, collision, and the uploaded map's asset dependencies.
- Add a reproducible test map and smoke-test checklist.
**Exit criteria:** clean editor build and reliable Play-in-Editor start.

### Phase 1 — Playable core
- Create a small authored test district with a road loop, sidewalks, safehouse, mission start, and combat encounter.
- Add a HUD for health, cash, objective, ammo, and wanted level.
- Add interaction prompts and a pause/settings menu.
- Ensure death/restart and save/load work safely.
**Exit criteria:** a new player can start, understand the objective, finish a mission, save, and resume.

### Phase 2 — Mission framework
- Replace single-purpose markers with a data-driven mission objective framework.
- Support reach-location, interact, eliminate-target, collect-item, escape-area, and timed objectives.
- Add prerequisites, mission states, rewards, failure/retry, and mission persistence.
- Implement a short original three-mission arc with checkpoints.
**Exit criteria:** mission flow is repeatable and state survives a save/reload.

### Phase 3 — Vehicles and traffic
- Replace the placeholder cube vehicle with Chaos Vehicles-based car classes and a documented setup path.
- Add enter/exit placement, camera handoff, vehicle health, damage, and recovery.
- Add a limited traffic spawner with pooled vehicles, simple lane/path following, and despawn rules.
**Exit criteria:** player can drive a real vehicle around the test district without getting stuck or losing possession.

### Phase 4 — NPCs and world activity
- Separate civilian, hostile, contact, and law-enforcement AI roles.
- Use AIController, navigation, perception, behavior trees or state trees, and animation-ready Blueprint subclasses.
- Add simple civilian flee/observe reactions and interaction dialogue.
- Add spawn budgets and distance-based activation for performance.
**Exit criteria:** a populated test district with predictable AI behavior and no runaway spawning.

### Phase 5 — Combat and wanted response
- Add weapon data, ammo, reload, fire modes, damage feedback, and pickup/inventory foundations.
- Add a wanted-level escalation/decay model based on witnessed crimes and line-of-sight.
- Spawn and dispatch law-enforcement AI, search last-known locations, pursue, and disengage when the player escapes.
- Add a safehouse/garage service that can clear wanted status through a deliberate gameplay rule.
**Exit criteria:** player actions create a readable, fair chase that can be escaped.

### Phase 6 — World, story, and progression
- Expand from the test district to a few handcrafted neighborhoods connected by roads and traversal routes.
- Add original characters, dialogue, mission cutscenes, side activities, collectibles, and safehouses.
- Add money, unlocks, vehicle storage, and persistent world-state flags.
- Keep map and content streaming budgeted; do not attempt a huge map before performance tests.
**Exit criteria:** a short but complete campaign with meaningful side activities.

### Phase 7 — Polish and release candidate
- Audio, animation, VFX, UI/UX, accessibility, input remapping, settings, and controller support.
- Automated smoke tests where practical; profile CPU/GPU/memory and test long sessions.
- Validate save compatibility, packaging, clean-machine startup, and crash logs.
- Package Development first, then Shipping only after stable testing.
**Exit criteria:** repeatable Windows package and a documented known-issues list.

## Technical rules

- Keep gameplay rules in C++ and expose tuning/content to Blueprint or data assets.
- Avoid hardcoding level-specific actor references; use interfaces, tags, soft references, and subsystem-owned state where appropriate.
- Prefer Enhanced Input for the long-term input setup; migrate only after the current controls have regression tests.
- Persist stable IDs and mission state, not raw pointers or transient actor references.
- Do not claim a feature is complete until it compiles in UE 5.8 and passes an in-editor test.
- Use original assets and content. Placeholder meshes are acceptable only for early system tests.

## Current known gaps

The current repository is a source prototype. It has movement, basic hitscan damage, rudimentary NPC pursuit, simple mission markers, placeholder vehicle movement, and player-state save/load. It does not yet provide a finished map, HUD, connected mission campaign, police response, production vehicle physics, complete actor/world persistence, final art/audio/animation, or a tested packaged executable.

## Build and validation checklist

1. Install Unreal Engine 5.8 and Visual Studio 2022 with Game development with C++.
2. Pull the repository and generate Visual Studio project files from `StoneyWorld.uproject`.
3. Build `StoneyWorldEditor` for `Development Editor | Win64`.
4. Open the project, create/open a test map, verify the GameMode and input bindings, and press Play.
5. Fix the first compiler/runtime error before continuing to the next phase.
6. Package for Windows only after the test map and core loop pass.
