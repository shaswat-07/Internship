# Snake Game

A modern, polished Snake game implementation in C++ using SFML with smooth gameplay, sound effects, and increasing difficulty.

## Features

- **Smooth 60 FPS gameplay** with delta time-based movement
- **Progressive difficulty** - speed increases as you eat more food
- **High-quality graphics** with custom textures and animations
- **Sound effects** - eating sounds, game over sounds, and background music
- **Game state management** - Menu, Playing, and Game Over states
- **Polished UI** with score display and smooth transitions
- **Cross-platform** support (Windows and Linux)

## Controls

- **Arrow Keys** or **WASD** - Move the snake
- **Space** - Start game from menu
- **R** - Restart after game over

## Building

### Prerequisites

- CMake 3.16 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)
- SFML 2.5 or higher

### Ubuntu/Debian
```bash
sudo apt update
sudo apt install cmake build-essential libsfml-dev
```

### Windows
Install SFML from https://www.sfml-dev.org/download.php and set up your environment.

### Build Instructions

#### Using CMake (Recommended)
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

#### Using the build script
```bash
chmod +x build.sh
./build.sh
```

#### Manual compilation (Linux)
```bash
g++ -std=c++17 -O2 *.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o SnakeGame
```

## Running

After building, run:
```bash
./bin/SnakeGame      # Linux
bin/SnakeGame.exe    # Windows
```

## Project Structure

```
SnakeGame/
├── main.cpp              # Entry point
├── Game.hpp/cpp          # Main game class
├── Snake.hpp/cpp         # Snake logic and rendering
├── Food.hpp/cpp          # Food spawning and animation
├── GameStateManager.hpp/cpp # Game state management
├── CMakeLists.txt        # CMake build configuration
├── build.sh              # Build script
├── README.md             # This file
└── assets/               # Game assets
    ├── fonts/
    │   └── arial.ttf
    ├── textures/
    │   ├── background.png
    │   ├── snake_head.png
    │   ├── snake_body.png
    │   └── apple.png
    └── sounds/
        ├── eat.wav
        ├── gameover.wav
        └── background.ogg
```

## Game Mechanics

- Snake starts with 3 segments and moves continuously
- Eating food increases score by 10 points and grows the snake
- Speed increases slightly with each food eaten
- Game ends when snake hits walls or itself
- Smooth restart functionality with 'R' key

## Technical Details

- **Engine**: SFML 2.5+
- **Language**: C++17
- **Architecture**: Object-oriented with clean separation of concerns
- **Memory Management**: Smart pointers to prevent leaks
- **Frame Rate**: Locked at 60 FPS with delta time for smooth movement
- **Audio**: Positional audio with volume control

## Asset Credits

The game includes procedurally generated textures if external assets are not found, ensuring it runs out of the box.