# UE Project Context

*Last updated: 2026-07-07*

This document is the shared Unreal Engine project context for Codex, Claude, and UE-related skills. It is a living V1 auto-drafted from the current repository. Unknown or unsettled items are marked as `TBD` instead of guessed.

## Engine & Project Overview

**Engine version:** UE 5.8, launcher/source status TBD  
**Project name:** Aura  
**Description:** A learning Unreal C++/GAS project that is evolving into a general-purpose skill system and skill editor prototype.  
**Project type:** Game / gameplay systems prototype  
**Genre / domain:** Action RPG / ability system research  
**Target platforms:**
- Windows desktop is the current assumed development target.
- Other target platforms are TBD.
**Skill system multiplayer direction:** First skill prototypes are ARPG-oriented and should be designed for multiplayer correctness. The initial validation target is local playable behavior first, then Listen Server / two-client checks for visible presentation, replicated state, and authoritative effects on other actors. Dedicated server support remains TBD.
**Confirmed V1 skill runtime direction:** Use a restrained hybrid model: a small number of generic GameplayAbility classes, `UAuraSkillDefinition` data assets, and native C++ mechanism fragments. The primary V1 path is `UAuraSkillDefinition + UAuraConfiguredActiveAbility + AbilitySpec.SourceObject`. Keep the tutorial-era ability path during V1-A and add the configured-skill path in parallel.
**Initial V1-A implementation scope:** The first implementation prompt should only build the minimal Fireball loop, not the full six-skill V1-A feature set.
**Asset preference:** For each concrete skill, inspect `Content/Assets` first for reusable tutorial animation/VFX/audio/material assets. Prefer soft references in skill definitions where practical; document any temporary hard-reference or synchronous-load compromise.

## Module Structure

**Primary game module:** `Aura`

| Module | Type | Notes |
| --- | --- | --- |
| `Aura` | Runtime | Main gameplay module. Contains GAS, character, UI, input, actors, and data assets. |
| `McpAutomationBridge` | Editor plugin module | Editor automation bridge under `Plugins/McpAutomationBridge`. Used by MCP tooling. |

**Build targets:**
- `AuraTarget`: Game target, `BuildSettingsVersion.V7`, `EngineIncludeOrderVersion.Unreal5_8`, `ExtraModuleNames.Add("Aura")`.
- `AuraEditorTarget`: Editor target, same build/include settings, `ExtraModuleNames.Add("Aura")`.

**Key dependencies per module:**
- **Aura PublicDeps:** `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`, `GameplayAbilities`.
- **Aura PrivateDeps:** `GameplayTags`, `GameplayTasks`, `NavigationSystem`, `Niagara`.
- **Note:** `EnhancedInput` appears twice in `Aura.Build.cs`; this should be cleaned later during code hygiene.

## Plugin Dependencies

**Enabled project plugins from `Aura.uproject`:**
- `GameplayAbilities`: Core GAS support.
- `MotionWarping`: Animation/movement alignment support.
- `PCG`: Procedural generation support.
- `ModelingToolsEditorMode`: Editor modeling tools.

**Custom / local plugins:**
- `McpAutomationBridge`: Editor automation bridge with MCP handlers for assets, actors, levels, animation, GAS, AI, Niagara, materials, PCG, widgets, and more. Native MCP is enabled in `Config/DefaultGame.ini`.

**Plugin notes:**
- `McpAutomationBridge` is beta/experimental according to its descriptor.
- Marketplace/Fab asset/plugin licensing is TBD.

## Coding Conventions

**Naming prefixes:** Standard UE prefixes are used (`F`, `U`, `A`, `E`, `I`).  
**Header style:** `#pragma once`.  
**Header organization:** Public/Private folders per module.  
**UObject pointer style:** Existing code often uses `TObjectPtr` for reflected UObject members. Continue this direction.  
**Assertion style:** Existing code uses `check()` in several GAS setup paths. Broader assertion policy is TBD.  
**Comments and encoding:** Some Chinese comments currently display as mojibake in shell output. Before broad edits, verify file encoding and avoid accidental re-encoding churn.
**Learning-oriented comments:** Formal code changes should explain non-obvious GAS lifecycle, authority/replication, configuration, and design tradeoffs. If the explanation would make code noisy, put it in a Chinese Markdown document and reference it from the implementation report.

**Additional rules for agents:**
- Do not move editor-only dependencies into the Runtime module.
- Put future custom editor tooling in an Editor module or editor plugin.
- Keep Runtime gameplay code independent from editor-only UI/tooling.
- Do not rewrite tutorial-era code broadly without an approved migration plan.
- Directly adding `.h` / `.cpp` files is allowed. Ensure correct `Public`/`Private` placement, `.generated.h` ordering, reflected UE macros, and `Aura.Build.cs` dependency updates.

## Subsystems in Use

**Gameplay framework:**
- GameMode: `AAuraGameModeBase`
- PlayerController: `AAuraPlayerController`
- PlayerState: `AAuraPlayerState`
- Player Character: `AAuraCharacter`
- Enemy Character: `AAuraEnemy`
- Base Character: `AAuraCharacterBase`

**Core custom systems:**
- GAS bootstrap and helpers: `UAuraAbilitySystemComponent`, `UAuraAbilitySystemLibrary`, `UAuraAbilitySystemGlobals`
- Attribute set: `UAuraAttributeSet`
- Ability base classes: `UAuraGameplayAbility`, `UAuraDamageGameplayAbility`, `UAuraProjectileSpell`
- Ability task: `UTargetDataUnderMouse`
- Damage execution: `UExecCalc_Damage`
- Magnitude calculations: `UMMC_MaxHealth`, `UMMC_MaxMana`
- Data assets: `UCharacterClassInfo`, `UAttributeInfo`, `UAuraInputConfig`
- UI layer: `AAuraHUD`, `UAuraWidgetController`, `UOverlayWidgetController`, `UAttributeMenuWidgetController`, `UAuraUserWidget`, `UDamageTextComponent`

**GAS ownership and replication:**
- Player ASC owner: `AAuraPlayerState`.
- Player avatar: `AAuraCharacter`.
- Player ASC replication mode: `Mixed`.
- Enemy ASC owner/avatar: `AAuraEnemy`.
- Enemy ASC replication mode: `Minimal`.
- AttributeSet for player is created on `AAuraPlayerState`.
- AttributeSet for enemy is created on `AAuraEnemy`.

**GAS initialization paths:**
- Player server init: `AAuraCharacter::PossessedBy`.
- Player client init: `AAuraCharacter::OnRep_PlayerState`.
- Enemy init: `AAuraEnemy::BeginPlay` through `InitAbilityActorInfo`.

**Current attributes:**
- Primary: Strength, Intelligence, Resilience, Vigor.
- Secondary: Armor, ArmorPenetration, BlockChance, CriticalHitChance, CriticalHitDamage, CriticalHitResistance, HealthRegeneration, ManaRegeneration, MaxHealth, MaxMana.
- Resistances: Fire, Lightning, Arcane, Physical.
- Vital: Health, Mana.
- Meta: IncomingDamage.

**Gameplay tags:**
- Config tags live in `Config/DefaultGameplayTags.ini`.
- Native tags are initialized in `FAuraGameplayTags`.
- Current notable domains: `Attributes.*`, `InputTag.*`, `Damage.*`, `Effects.HitReact`, `Event.Montage.FireBolt`, `Message.*`.
- Note: `Attributes.Resistance.lightning` uses lowercase `lightning` in native tag creation; later tag normalization should review this.

## Build Configuration

**Build targets:** Game and Editor. Dedicated Server/Client targets are not currently present.  
**Custom macros / build flags:** None identified yet beyond standard UE target settings.  
**Third-party C++ libraries:** None identified in the game module.  
**Engine modifications:** TBD.  
**Default map:** `/Game/Maps/StartupMap.StartupMap`.  
**Asset manager:** `AssetManagerClassName=/Script/Aura.AuraAssetManager`.  
**Ability system globals:** `AbilitySystemGlobalsClassName=/Script/Aura.AuraAbilitySystemGlobals`.  
**Custom collision channel:** `Projectile` on `ECC_GameTraceChannel1`.

## Team Context

**Human project owner:** User.  
**Agent roles:** Codex plans/reviews; Claude implements specific tasks and writes reports.  
**Source control:** Git. Current branch observed as `main` tracking `origin/main`.  
**Baseline tag:** `tutorial-baseline-before-v1-skill-runtime` marks the tutorial baseline before V1 skill runtime implementation.
**Branching strategy:** Proposed in `Agent/00_Coordination/Git_Workflow.md`; not yet finalized.  
**Code review:** Planned: Claude implementation -> report -> Codex review -> user approval.  
**Documentation standards:** Planning, decisions, prompts, and reports live under `Agent/`.

## Open Questions

- Confirm whether UE 5.8 is launcher build or source build.
- Confirm target platforms beyond Windows.
- Confirm whether future skill editor should be a custom asset editor, Editor Utility Widget, graph editor, or staged combination.
- Confirm whether existing tutorial assets can be freely reused and redistributed in this project.
