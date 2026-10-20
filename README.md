# Stoney's World

An original Unreal Engine 5 open-world action prototype.

## Project status

This is a source-code prototype, not a packaged game. It does not yet include a finished world/map, art assets, vehicles, traffic, NPCs, missions, or a wanted system.

## Target

- Unreal Engine 5.8
- Windows 10/11 x64
- Visual Studio 2022 with the **Game development with C++** workload and a Windows SDK

## Current prototype systems

- Third-person character and camera
- Fast sprinting, jumping, and a basic slide
- Grapple trace with a basic pull/reel approximation (not a physical rope constraint or complete swing system)
- Basic glide while falling

## Controls

- **WASD** — move
- **Mouse** — look
- **Shift** — sprint
- **Space** — jump
- **Left Ctrl** — slide
- **Left Mouse Button** — grapple / release
- **G** — glide while falling
- **Mouse wheel** — grapple reel axis (if configured by the input settings)

## Build in Unreal Engine 5.8

1. Install Unreal Engine 5.8 through Epic Games Launcher.
2. Install Visual Studio 2022 and the **Game development with C++** workload.
3. Open `StoneyWorld.uproject` in UE 5.8.
4. If prompted, generate project files and build the **Development Editor | Win64** target in Visual Studio.
5. Open the project and create or assign a playable level before pressing Play. This repository currently has no authored map/assets, so the editor may open to an empty/default level.

If compilation fails, copy the first compiler error from Visual Studio's Output window. This source has not been compiled or tested in the current environment.

## Packaging

A Windows executable must be packaged on a machine with Unreal Engine 5.8 installed. No prebuilt `.exe` is included in this repository.
