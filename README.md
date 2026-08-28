<div align="center">

# 🃏 Unmatched — C++ Board Game Engine

### A modern C++ recreation of the *Unmatched* board game — from a terminal prototype to a full graphical experience.

[![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://en.cppreference.com/)
[![CMake](https://img.shields.io/badge/CMake-3.11%2B-064F8C?style=for-the-badge&logo=cmake&logoColor=white)](https://cmake.org/)
[![raylib](https://img.shields.io/badge/raylib-graphics-000000?style=for-the-badge&logo=c&logoColor=white)](https://www.raylib.com/)
[![FTXUI](https://img.shields.io/badge/FTXUI-legacy%20frontend-6c757d?style=for-the-badge)](https://github.com/ArthurSonzogni/FTXUI)
[![Phase](https://img.shields.io/badge/Phase-2%20Released-2fa84f?style=for-the-badge)](#-development-phases)

<p>
  <a href="#-preview">Preview</a> •
  <a href="#-features">Features</a> •
  <a href="#-building">Building</a> •
  <a href="#-architecture">Architecture</a> •
  <a href="#-current-heroes">Heroes</a> •
  <a href="#-roadmap">Roadmap</a>
</p>

</div>

---

## 📖 Overview

A modern C++ implementation of the **Unmatched** board game — originally built with a terminal interface (**FTXUI**) in **Phase 1**, and now fully driven by a graphical interface (**raylib**) in **Phase 2**.

The project focuses on **clean architecture**, **object-oriented design**, and **maintainable game logic**, while faithfully recreating the mechanics of the original board game: combat resolution, hero decks, board movement, and hero-specific abilities.

> 💡 **Get the latest release — Phase 2 (`v2.0.0`):**
> ```bash
> git clone https://github.com/TAmin6002/Unmatched_Game.git
> cd Unmatched_Game
> git checkout v2.0.0
> ```
> Browse all releases and tags → [github.com/TAmin6002/Unmatched_Game/tags](https://github.com/TAmin6002/Unmatched_Game/tags)

---

## 🖼️ Preview

<div align="center">

<!--
  📸 Add your Phase 2 screenshots / GIFs here, e.g.:
  <img src="assets/screenshots/main-menu.png" width="45%" />
  <img src="assets/screenshots/board.png" width="45%" />
-->

| Main Menu | Board & Combat |
|:---:|:---:|
| *screenshot placeholder* | *screenshot placeholder* |

</div>

### 🎨 Graphical Interface (raylib) — Active Frontend
- Fullscreen game window with a custom event loop (`InitWindow`, `SetTargetFPS(60)`)
- Custom-loaded font (**Cinzel**) for a themed board-game look
- Board rendering with hero and fog-token markers
- Card art rendering for every hero's deck
- Background music / audio device support
- Menus for: **Main Menu · Play · Continue (Load) · Help · Exit**
- Full in-game UI: hero selection, action menu, attacker/defender selection, card reveal, hand viewing, discard flow, slot-based save/load menu

### 🖥️ Terminal Interface (FTXUI) — Phase 1 Legacy
- The original Phase 1 interface was built with FTXUI (interactive menus, keyboard navigation, turn-based text rendering).
- As of Phase 2, the entire game loop runs through the raylib frontend; the FTXUI frontend source is kept in the repository for reference but is no longer wired into the active build.

---

## ✨ Features

| | |
|---|---|
| 🧩 | Object-Oriented architecture with a polymorphic `Heroes` base class |
| ⚙️ | Modular game engine: `Board`, `Space`, `Player`, `Heroes`, `Card`, `CardResolver`, `Controller` |
| 🎯 | Turn-based gameplay with an Action Point system (2 actions/turn) |
| 🗺️ | Zone-based board: 32 connected spaces, including a 4-house **portal network** for instant travel |
| 🃏 | Card-based combat system (Attack / Defense / Event cards, each with timing rules: *Before / During / After / Immediate*) |
| 🦹 | Hero-specific decks and abilities |
| 🤝 | Sidekick / comrade system (Dracula's Vampire Sisters, Sherlock's Dr. Watson, Invisible Man's Fog tokens) |
| 🌫️ | **Fog / Mist token system** tied to Invisible Man *(Phase 2)* |
| 💾 | **JSON-based Save / Load system** with 3 save slots *(Phase 2)* |
| 🖥️➡️🎨 | **Full raylib graphical frontend** replacing the terminal UI as the primary interface *(Phase 2)* |
| 🛠️ | Cross-platform build using CMake, with raylib vendored directly in the repository |

---

## 🛠️ Technologies

| Technology | Role |
|---|---|
| **C++17** | Core language |
| **CMake 3.11+** | Build system |
| **raylib** *(vendored under `raylib-master/`)* | Active graphical frontend |
| **FTXUI** | Phase 1 terminal frontend *(retained, currently inactive)* |
| **Custom JSON library** (`Json.h` / `Json.cpp`) | Hand-written save/load serialization |

---

## 📂 Project Structure

```text
.
├── assets/             # Fonts (Cinzel), music, board/hero/card textures, backgrounds
├── includes/           # Header files (Board, Space, Player, Heroes, Card, Controller, Raylib, Json, ...)
├── src/                # Source files (.cpp) for every module above
├── raylib-master/      # Vendored raylib source tree (built via add_subdirectory)
├── saves/              # Created at runtime; holds slot_1.json, slot_2.json, slot_3.json
├── CMakeLists.txt
└── README.md
```

---

## 🏗️ Architecture

The project follows a modular architecture where every major gameplay system is isolated into its own component.

<details open>
<summary><b>⚙️ Core Engine</b></summary>
<br>

| Class | Responsibility |
|---|---|
| **`Space`** | A single board tile. Holds pointers to its `zone` and `neighbor` tiles, an optional occupying `Heroes*`, an optional `Fog` token (`Heroes*`), a tile `number`, and optional portal connections for instant travel between linked spaces. |
| **`Board`** | Owns all 32 `Space` objects and wires up their adjacency/zone relationships and portal links (currently connecting 4 "houses" into a mutual portal network). Provides adjacency checks (`is_Adjacent`) and hero-swapping (`SwapHeroes`). |
| **`Player`** | Represents one of the two human players: name, age (used to determine turn order), selected `Heroes* character`, list of `comrade` sidekicks, current action `count`, and the currently `selected_card`. |
| **`Heroes`** *(abstract base class)* | Shared state for every fighter: name, attack type, movement, action points, health, board `place`, live/dead status, and per-hero `deck` / `hand` / `discard` piles. Declares a pure-virtual `abiliti(Board*)` for hero-specific special abilities, and exposes `toJson()` / `loadFromJson()` for full state serialization. |
| **`Card`** | A single game card: `CardType`, `CardTiming` (Before/During/After/Immediate), attack/defense type, numeric `amount` and `Boost`, owning hero, and an `ApplyEffects` flag used by the resolver. |
| **`CardResolver`** | The rules engine. `execute()` switches over every `CardType` and applies that card's specific gameplay effect (damage, movement, token manipulation, hand effects, etc.) against the live `Board`/`Heroes`/`Player` state. |
| **`Controller`** | The central game loop owner. Holds the `Board`, both `Player`s, every hero instance (Dracula + 3 Sisters, Sherlock Holmes + Dr. Watson, Invisible Man + 3 Fog tokens), the active `Attacker`/`Defender` and their selected cards, turn/round tracking, and the `CardResolver`. Drives combat resolution, turn switching, win-condition checks, and save/load. |

</details>

<details>
<summary><b>🦸 Hero Hierarchy</b> (<code>Heroes</code> subclasses)</summary>
<br>

- `Dracula`, `Sisters` (×3, his comrades)
- `SherlockHolmes`, `Dr_Watson` (his comrade)
- `InvisibleMan`, `Fog` (×3 mist tokens, his comrades)

</details>

<details>
<summary><b>🖥️ Frontend</b></summary>
<br>

| Class | Role |
|---|---|
| **`Raylib`** | The **active** graphical frontend. Owns all screen/menu rendering: main menu, board rendering (`DrawBoardMap`, `DrawHeroBox`), hero and card selection menus, combat reveal, hand viewing, fog-token movement prompts, and the save/load slot menu. |
| **`Ftxui_Front`** | Phase 1's terminal frontend. Present in the repository for historical reference; its implementation is currently commented out and not linked into the build. |

</details>

<details>
<summary><b>💾 Persistence</b></summary>
<br>

| Class | Role |
|---|---|
| **`Json`** | A small hand-rolled JSON value type (`Null`, `Boolean`, `Number`, `String`, `Array`, `Object`) supporting `parse()`, `dump()`, and typed accessors (`asInt`, `asBool`, `asString`), used exclusively for save/load serialization. |

</details>

> This separation makes it easy to add new heroes, cards, maps, and gameplay mechanics without modifying the core engine.

---

## ⚡ Building

### Requirements
- CMake 3.11+
- C++17 compiler
- Git
- A display/OpenGL-capable environment (raylib opens a real window) and an audio device

> raylib is vendored directly in the repository (`raylib-master/`) and built via `add_subdirectory`; no external package manager step is required for it.

```bash
# 1. Clone the repository
git clone https://github.com/TAmin6002/Unmatched_Game.git
cd Unmatched_Game
git checkout v2.0.0

# 2. Configure
cmake -B build

# 3. Build
cmake --build build

# 4. Run
./build/Unmatched
```

> The build step also copies the `assets/` folder next to the produced executable so fonts, textures, and music are found at runtime.

---

## 🦸 Current Heroes

| Hero | Comrades / Tokens | Notes |
|---|---|---|
| 🕵️ **Sherlock Holmes** | Dr. Watson | Deduction / investigation-themed card set |
| 🧛 **Dracula** | 3 Vampire Sisters | Blood-magic / seduction-themed card set |
| 👤 **Invisible Man** *(Phase 2)* | 3 Fog tokens | Stealth hero built around Mist/Fog board tokens |

> More heroes will be added in future updates.

---

## 🎮 Gameplay

Players take turns performing actions such as:

- 🚶 **Maneuver** — move a fighter
- ⚔️ **Attack** — select attacker → defender → reveal combat cards → resolve damage
- 🃏 **Scheme / Event cards**
- 📥 **Draw cards**
- ✨ **Resolve card effects** (via `CardResolver`)
- 🏃 **Move fighters**
- 🌟 **Use hero abilities**
- 🌫️ **Place, move, or interact with Fog tokens** *(Invisible Man only)*

Combat is resolved by revealing an attack card and a defense card and comparing their `amount` values, with each `CardType` able to trigger additional effects through `CardResolver`.

### 🌫️ Invisible Man & Fog Tokens *(Phase 2)*

Invisible Man plays around three Fog tokens scattered on the board:

- Standing on a space with a Fog token grants **+1 Defense** when defending, and lets his cards deal bonus damage (e.g. *Step Lightly* deals 3 damage instead of 1 while on Fog).
- His deck includes fog-manipulation cards such as **Rolling Fog** (teleport a Fog token anywhere), **Slip Away** (teleport the hero to an empty space near a Fog token), **Step Lightly** (attack an adjacent hero, then relocate a Fog token), and **Vanish** (remove the hero from the board for later re-placement).
- **Reign of Terror** deals area damage to the opponent's whole team, but only while Invisible Man himself is standing on a Fog token.

### 💾 Save / Load System *(Phase 2)*

- The game serializes its entire state to JSON — round number, whose turn it is, both players (name, age, action count, character, comrades, selected card), every hero's full state (health, movement, action points, deck/hand/discard contents, board position), and board occupancy (heroes and Fog tokens per space).
- Games are written to `saves/slot_<n>.json` for **3 independent save slots**.
- The main menu's **Continue** option reads all 3 slots and shows a live summary (`"<Player 1> vs <Player 2> - Round <n>"`) so you can pick which one to resume.

---

## 🚀 Development Phases

<table>
<tr>
<td valign="top" width="50%">

### 🥇 Phase 1

- Core game engine: `Board`, `Space`, `Player`, `Heroes`, `Card`, `CardResolver`, `Controller`
- Terminal interface built with FTXUI (menus, keyboard navigation, text rendering)
- Initial heroes: Sherlock Holmes (+ Dr. Watson) and Dracula (+ 3 Vampire Sisters)
- Turn/action system and card-based combat resolution

</td>
<td valign="top" width="50%">

### 🥈 Phase 2

- **Invisible Man** added as a fully playable hero, including his complete card set and the Fog/Mist token mechanics (placement, movement, defense bonus, and fog-dependent card effects)
- **Save / Load system**: full JSON serialization of game state across 3 save slots, with slot summaries in the menu
- **Graphical interface implemented with raylib**, replacing the terminal frontend as the game's primary UI — fullscreen rendering, board and card art, audio, and all interactive menus (hero selection, combat, hand management, fog-token prompts, save/load)

</td>
</tr>
</table>

---

## 🗺️ Roadmap

- [ ] More heroes
- [ ] More maps
- [ ] Further raylib UI polish and animations
- [ ] AI opponent
- [ ] Online multiplayer
- [ ] Sound effects / particle effects
- [ ] Improved card animations

---

## 🎯 Design Goals

This project is built with the following principles:

- ✅ Clean C++ code
- ✅ Object-Oriented Design
- ✅ Modular architecture
- ✅ Easy extensibility
- ✅ Maintainability
- ✅ Separation of game logic from UI

---

<div align="center">

Made with ❤️ and a lot of `switch(CardType)` statements.

**⭐ If you like this project, consider giving it a star!**

</div>
