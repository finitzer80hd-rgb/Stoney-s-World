# Stoney's World

An original Unreal Engine 5.8 C++ open-world action prototype for Windows 10/11 x64.

## Prototype systems in source

- Third-person character and camera
- Sprint, jump, slide, grapple pull/reel, and glide
- Hitscan combat (F) and basic hostile NPCs that detect, approach, and damage the player
- Mission marker actors that can be completed by looking at them and pressing E
- Drivable prototype vehicle pawn with enter/exit (E); the vehicle uses a placeholder engine cube mesh until you assign a real vehicle mesh
- Player health, wanted level (0-5), mission completion counter, and local save/load
- Save to slot `StoneyWorld_Save01` with F5; load with F9

This is a **playable systems prototype**, not a finished open-world game. It does not ship with an authored world, final character/vehicle art, UI/HUD, audio, animations, navmesh, mission chain, police response, or a packaged executable. NPCs and mission/vehicle actors must be placed in a level (or spawned by your own Blueprint). A usable floor and collision geometry are required. NPC navigation is rudimentary and works best with a Nav Mesh Bounds Volume covering the area.

## Controls

- **WASD** — move
- **Mouse** — look
- **Left Shift** — sprint
- **Space** — jump
- **Left Ctrl** — slide
- **Hold Left Mouse Button** — grapple; release to detach
- **Mouse wheel** — reel grapple
- **Hold G** — glide while falling
- **F** — fire hitscan weapon
- **E** — interact with a mission marker or vehicle in view; E exits a vehicle while driving
- **F5** — save player location, rotation, health, wanted level, and mission count
- **F9** — load saved player state

## Windows build (Unreal Engine 5.8)

1. Install Unreal Engine 5.8 through Epic Games Launcher.
2. Install Visual Studio 2022 with **Game development with C++** and a Windows 10/11 SDK.
3. Clone or pull this repository:
   ```powershell
   git clone https://github.com/finitzer80hd-rgb/Stoney-s-World.git
   cd Stoney-s-World
   ```
   If already cloned, run `git pull`.
4. Right-click `StoneyWorld.uproject` and choose **Generate Visual Studio project files** (or right-click and select the Unreal Engine 5.8 association if that menu is available).
5. Open `StoneyWorld.sln` and build **Development Editor | Win64**. Alternatively, use an Unreal Engine 5.8 command prompt:
   ```powershell
   & "$env:ProgramFiles\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" StoneyWorldEditor Win64 Development -Project="$PWD\StoneyWorld.uproject" -WaitMutex
   ```
   If UE is installed to a different directory, change the path accordingly.
6. Open `StoneyWorld.uproject`. Create/open a level with a floor and lighting. Place an `AStoneyWorldMission`, `AStoneyWorldNPC`, and `AStoneyWorldVehicle` actor in the map. For NPC movement, add a **Nav Mesh Bounds Volume** and press **P** to verify the green navigation area.
7. Press Play and test the controls above.

## Package a Windows build

In Unreal Editor: **Platforms > Windows > Package Project** (menu wording may vary by UE version). Select a Windows 64-bit Development build for initial testing. Packaging requires a local UE 5.8 installation and a valid playable map. No prebuilt .exe is included because this environment cannot compile or package Unreal Engine projects.

## Known prototype limitations

- Gameplay systems are source-level implementations and have **not been compiled or play-tested here**; resolve any compiler errors reported by UE 5.8 before adding assets.
- Combat is a simple line trace, not a full weapon inventory/reload system.
- Wanted level is a saved 0–5 value raised when the player hits a hostile NPC; police/chase/spawn response is not implemented.
- Missions are individual interactable markers, not a quest graph or objective chain.
- NPCs use simple local pursuit and attack logic; add navigation mesh, animation, perception, and better AI for production.
- The vehicle uses a floating pawn movement model and a placeholder cube mesh, not Chaos Vehicles physics.
- Save/load restores player state only; mission actor completion states and world actors are not persisted.
- The uploaded `Untitled.umap` and `Untitled_HLOD0_Instancing.uasset` have not been integrated because their referenced assets/dependencies cannot be validated without opening the full project in UE 5.8.


## First playable district blockout (new)

A C++ runtime builder now creates a **graybox test district** from Unreal's built-in cube mesh. It is intended to make a first playable test space without requiring external art assets; it is not a final city map.

### Set up the district in UE 5.8

1. Pull the latest `main` branch and build `StoneyWorldEditor` for `Development Editor | Win64`.
2. Open the project. Create a new level named `L_District_Blockout` (or duplicate a level you can safely edit).
3. From the C++ Classes browser, drag **StoneyWorldDistrictBuilder** into the level at Location `(0, 0, 0)`, Rotation `(0, 0, 0)`, Scale `(1, 1, 1)`. If the class is not listed, compile successfully and refresh/reopen the editor.
4. Add a **Player Start** from Place Actors. Put it at approximately `(-350, -350, 130)`, rotation `(0, 0, 0)`. Keep it clear of the buildings and road edges. The builder intentionally does not rely on spawning a PlayerStart during BeginPlay because GameMode chooses the initial pawn spawn earlier.
5. Add a **Directional Light** and **Sky Light** from Place Actors. For a quick preview, enable atmosphere/sun settings if available in the light details, and add a Sky Atmosphere and Sky Dome/sky setup if the level looks black. Add a **Nav Mesh Bounds Volume** covering the central streets and buildings; press **P** to see the green navigation area.
6. In the builder Details panel, leave **Build At Runtime** and **Spawn Gameplay Actors** enabled. `Building Rows Per Side` defaults to 3; reduce it to 2 if editor/game performance is poor.
7. Open **World Settings** and verify GameMode Override is empty or set to `StoneyWorldGameMode`. The project default GameMode already uses `AStoneyWorldCharacter`.
8. Save the level, press **Play**, and verify that the player spawns on the PlayerStart. The builder creates a broad floor, crossing roads, sidewalks, lane marks, placeholder buildings, one mission marker, two basic hostile NPCs, and one placeholder vehicle when the level begins.
9. Aim at the floating **MISSION** label and press **E** to complete it. Press **F** to test hitscan combat. Approach the placeholder vehicle and press **E** to possess it; press **E** again to exit. Existing movement controls remain unchanged.
10. Save the level as an asset and set it as the Editor Startup Map / Game Default Map in **Project Settings > Maps & Modes** when you want it to open by default. For packaging, make sure this map is included in the package list.

### Important blockout notes

- The district uses engine basic cubes and collision as temporary geometry. It has no final materials, authored interiors, detailed roads, traffic, polished lighting, or final skyline.
- The district builder creates geometry at runtime. This keeps the implementation self-contained, but the generated block actors are not a hand-authored, baked/static city and will add runtime actor overhead. Replace this blockout with instanced meshes and authored assets as the world grows.
- The current NPC class has rudimentary chase/attack logic and no production animation/visual character setup. Add Blueprint subclasses with skeletal meshes, Animation Blueprints, AI Controllers, and Nav Mesh for a proper presentation.
- The placeholder vehicle uses simplified floating-pawn movement, not Chaos Vehicle physics.
- The runtime builder and all new code still require compilation and in-editor validation in Unreal Engine 5.8. If compilation fails, fix the first compiler/UHT error before proceeding.
