# Repository Guidelines

## Project Structure & Module Organization

`Slash.uproject` defines the Unreal Engine project and its `Slash` runtime module.

- `Source/Slash/Public/` contains headers; `Private/` contains implementations, organized into `Characters`, `Pawns`, and `Items`.
- `Source/Slash.Build.cs` declares module dependencies; `Source/*.Target.cs` defines game and editor targets.
- `Content/` holds maps, Blueprints, textures, effects, and imported asset packs. The startup map is `Content/Maps/SlashOpenWorld.umap`.
- `SourceArt/` stores source textures; `Config/` contains project defaults and input mappings.
- `Plugins/VisualStudioTools/` contains Visual Studio integration code.
- `Binaries/`, `Intermediate/`, `DerivedDataCache/`, and `Saved/` are generated and ignored.

## Build, Test, and Development Commands

Use the engine installation associated with `Slash.uproject`; its association is a local GUID. Install Visual Studio with Unreal C++ tooling. From the repository root, set `$env:UE_ROOT` to the matching engine directory, then run:

```powershell
# Generate Visual Studio project files.
& "$env:UE_ROOT/Engine/Build/BatchFiles/GenerateProjectFiles.bat" "-project=$PWD/Slash.uproject" -game -engine
# Build the Windows editor target.
& "$env:UE_ROOT/Engine/Build/BatchFiles/Build.bat" SlashEditor Win64 Development "-Project=$PWD/Slash.uproject" -WaitMutex
# Launch the editor.
& "$env:UE_ROOT/Engine/Binaries/Win64/UnrealEditor.exe" "$PWD/Slash.uproject"
```

Use Play in Editor to exercise gameplay changes.

## Coding Style & Naming Conventions

Follow surrounding C++ style: tabs for indentation, matching header/source names, and Unreal type prefixes (`ASlashCharacter`, `UCameraComponent`, `FVector`). Keep generated headers last in header include lists. Preserve reflected property names unless Blueprint references are migrated. Add dependencies in `Slash.Build.cs`. No project-wide formatter or linter is configured.

## Testing Guidelines

No gameplay automation suite or coverage threshold currently exists. Build `SlashEditor`, open `SlashOpenWorld`, and verify affected movement, camera, jump, pawn, or item behavior. Check the Output Log for errors. For new automated tests, use Unreal Automation tests under `Source/Slash/Private/Tests/`, name them `Slash.<Feature>.<Behavior>`, and run them through Session Frontend's Automation tab.

## Commit & Pull Request Guidelines

History currently contains only `init`, so no established message convention exists. Use concise imperative subjects, such as `Fix bird camera rotation`. Keep changes focused. PRs should describe behavior, affected maps/assets, and validation performed; link issues when applicable and include screenshots or video for visual changes.

## Asset & Configuration Hygiene

Edit `.uasset` and `.umap` files through Unreal Editor. Include related World Partition external actor/object changes when saving maps. Review `Config/` changes deliberately and exclude generated build output from commits.
