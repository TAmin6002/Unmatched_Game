# Unmatched Game (C++)

https://github.com/TAmin6002/Unmatched_Game.git

A modern C++ implementation of the **Unmatched** board game featuring both a **terminal interface** built with **FTXUI** and a graphical interface powered by **raylib**.

The project focuses on clean architecture, object-oriented design, and maintainable game logic while recreating the mechanics of the original board game.

> **Status:** 🚧 In Development

---

## Preview

### Terminal Interface
- Interactive menus using FTXUI
- Turn-based gameplay
- Keyboard navigation
- Rich terminal rendering

### Graphical Interface (raylib)
- Visual game board
- Hero sprites
- Card rendering
- Modern game UI

---

# Features

- Object-Oriented Architecture
- Modular Game Engine
- Multiple Heroes
- Card-Based Combat System
- Turn Management
- Action Point System
- Hero Abilities
- Board Movement
- Zone-based Map
- Terminal UI (FTXUI)
- Graphical UI (raylib)
- Cross-platform build using CMake

---

# Technologies

- C++17
- CMake
- FTXUI
- raylib

---

# Project Structure

```
.
├── assets/             # Fonts, textures, sprites, sounds
├── include/            # Header files
├── src/                # Source files
├── build/              # Build directory
├── CMakeLists.txt
└── README.md
```

---

# Architecture

The project follows a modular architecture where every major gameplay system is isolated into its own component.

Main modules include:

- Board
- Space
- Player
- Heroes
- Card
- Card Resolver
- Controller
- Terminal Frontend (FTXUI)
- Graphical Frontend (raylib)

This separation makes it easy to add new heroes, cards, maps, and gameplay mechanics without modifying the core engine.

---

# Building

## Requirements

- CMake 3.20+
- C++17 Compiler
- Git

Dependencies are downloaded automatically using **FetchContent**.

Clone the repository:

```bash
git clone https://github.com/TAmin6002/Unmatched_Game.git
cd Unmatched_Game
```

Configure:

```bash
cmake -B build
```

Build:

```bash
cmake --build build
```

Run:

```bash
./build/app
```

---

# Current Heroes

- Sherlock Holmes
- Dracula

More heroes will be added in future updates.

---

# Gameplay

Players take turns performing actions such as:

- Maneuver
- Attack
- Scheme
- Draw Cards
- Resolve Card Effects
- Move Fighters
- Use Hero Abilities

Combat is resolved using attack and defense cards inspired by the original board game.

---

# Roadmap

- More heroes
- More maps
- Complete raylib graphical interface
- Animations
- AI opponent
- Online Multiplayer
- Save / Load
- Sound effects
- Particle effects
- Improved card animations

---

# Design Goals

This project is built with the following principles:

- Clean C++ code
- Object-Oriented Design
- Modular architecture
- Easy extensibility
- Maintainability
- Separation of game logic from UI

---

# Disclaimer

This project is a fan-made implementation created for educational and programming purposes.

All rights related to the original **Unmatched** board game belong to their respective owners.
