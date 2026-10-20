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
