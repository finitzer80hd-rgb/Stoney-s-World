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
- Sprinting, jumping, crouched slide with friction restoration
- Grapple pull with rope-length correction and retained tangential momentum (prototype, not a production rope solver)
- Glide fall-speed limiting with restored air-control settings

## Controls

- **WASD** — move
- **Mouse** — look
- **Shift** — sprint
- **Space** — jump
- **Left Ctrl** — slide
- **Hold Left Mouse Button** — grapple; release to detach
- **Hold G** — glide while falling; release to stop gliding
- **Mouse wheel** — reel the grapple in/out

## Build in Unreal Engine 5.8

1. Install Unreal Engine 5.8 through Epic Games Launcher.
2. Install Visual Studio 2022 and the **Game development with C++** workload.
3. Open `StoneyWorld.uproject` in UE 5.8.
4. If prompted, generate project files and build the **Development Editor | Win64** target in Visual Studio.
5. Open the project and create or assign a playable level before pressing Play. This repository currently has no authored map/assets, so the editor may open to an empty/default level.

If compilation fails, copy the first compiler error from Visual Studio's Output window. The updated movement code is a source change and has not been compiled or tested in this environment. After rebuilding, test on a floor and a tall wall before tuning the editable movement values.

## Packaging

A Windows executable must be packaged on a machine with Unreal Engine 5.8 installed. No prebuilt `.exe` is included in this repository.
