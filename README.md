# INDA-Projektarbete

A pseudo-3D raycasting FPS written in C, rendered with OpenGL/GLUT. Inspired by early-90s engines (Wolfenstein 3D-style), the world is a 2D grid that is "lifted" into a 3D-looking scene one vertical column at a time using DDA raycasting.

Built as a project for the *INDA* course at KTH.

## Features

- Pseudo-3D rendering via per-column DDA raycasting
- First-person movement and turning (WASD)
- Shooting mechanic with a basic enemy
- Tile-based maps loaded from plain text files
- Multiple hand-authored levels
- AI-generated maps via a local LLM (Ollama / Mistral)
- Player–wall collision handling
- Cross-platform build (macOS and Linux)

## Tech stack

- **Language:** C (C99)
- **Graphics:** OpenGL + GLUT (`-framework OpenGL -framework GLUT` on macOS, `-lGL -lGLU -lglut` on Linux)
- **Build:** GNU Make / `gcc`
- **Tooling:** Python 3 for the map generator (`requests` + a local Ollama server)

## Demo

*TBA*

## Project structure

```
.
├── Makefile
├── src/                # Game source (.c)
├── include/            # Headers (.h)
├── assets/maps/        # Level files (level1.txt, level2.txt, ...)
├── tools/              # Python map generator
└── docs/
```

## Installation

### Prerequisites

- `gcc` (or any C compiler that accepts the `Makefile` flags)
- `make`
- OpenGL + GLUT development libraries
  - **macOS:** ships with the system (uses the `OpenGL` and `GLUT` frameworks)
  - **Linux (Debian/Ubuntu):** `sudo apt install build-essential freeglut3-dev`
- *(Optional, for AI map generation)* Python 3.10+ and a running [Ollama](https://ollama.com) instance with the `mistral` model pulled

### Clone

```sh
git clone https://github.com/your-username/INDA-Projektarbete.git
cd INDA-Projektarbete
```

### Build

```sh
make
```

To rebuild from scratch:

```sh
make re
```

To remove the built binary:

```sh
make clean
```

## How to run

After building, run the game from the project root so the asset paths resolve correctly:

```sh
./game
```

### Controls

| Key     | Action          |
| ------- | --------------- |
| `W`     | Move forward    |
| `S`     | Move backward   |
| `A`     | Turn left       |
| `D`     | Turn right      |
| `Space` | Shoot           |

### Generating a map with AI *(optional)*

The game itself invokes the map generator at runtime via `.venv/bin/python tools/generate_map.py`, so you only need to set up the virtual environment once. With Ollama running locally and the `mistral` model available:

```sh
python3 -m venv .venv
source .venv/bin/activate
pip install -r tools/requirements.txt
```

After that, just run `./game` as usual — the generated map is written to `assets/maps/generated/generated_map.txt`.
