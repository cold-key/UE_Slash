# Slash

An Unreal Engine C++ project with character movement, items and weapons, and Control Rig foot IK.

## Requirements

- A compatible Unreal Engine installation. This project currently uses a source build reporting version 5.8.1.
- Visual Studio with the C++ game development tools; see `.vsconfig`.
- Git and Git LFS.

## Clone and open

```powershell
git lfs install
git clone https://github.com/cold-key/UE_Slash.git
cd UE_Slash
git lfs pull
```

Associate `Slash.uproject` with your local engine installation using **Switch Unreal Engine version**, then generate Visual Studio project files. Build `SlashEditor` using **Development Editor / Win64** and open the project. The startup map is `Content/Maps/SlashOpenWorld.umap`.

## Version control

Commit `Source/`, `Config/`, `Content/`, `SourceArt/`, plugin sources, and the project descriptor. Commit World Partition external actor and object files together with map edits. Unreal assets and source art use Git LFS.

Build output, editor caches, local IDE state, and `Saved/` are ignored. Keep machine-specific credentials out of tracked configuration. If Android File Server authentication is needed locally, configure its token in `Saved/Config/WindowsEditor/Engine.ini`; that file is not versioned.

See [AGENTS.md](AGENTS.md) for build commands and contributor guidelines.
