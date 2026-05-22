# CHIP-8 Emulator

A CHIP-8 emulator written in C using [raylib](https://github.com/raysan5/raylib) for graphics and keyboard input.

This project was built as a beginner-friendly emulator project to learn CPU fetch/decode/execute cycles, memory, registers, stack behavior, timers, input handling, and simple graphics rendering.

## Features

- CHIP-8 CPU fetch/decode/execute loop
- 4KB memory
- 16 general-purpose registers
- Index register
- Program counter
- Stack and subroutine calls
- Delay timer and sound timer
- 64x32 monochrome display
- Keyboard input mapping
- ROM loading
- raylib-based rendering
- Static raylib build through CMake submodule

## Project Structure

```text
chip-8/
├── src/
│   ├── main.c
│   ├── chip8.c
│   └── chip8.h
├── thirdParty/
│   └── raylib/
├── roms/
├── CMakeLists.txt
├── README.md
└── .gitignore
```

## Dependencies
This project uses the Raylib graphics API as a Git Submodule

You will need:
- C compiler
- CMake
- Git
- Linux graphics development libraries, if building on Linux/WSL

On Ubuntu:
```text
sudo apt update
sudo apt install build-essential cmake git libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev
```

## Cloning
Cloning with submodules:

To clone this project, please follow the following steps:
```text
git clone --recurse-submodules https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git
cd YOUR_REPO_NAME
```
If you have already cloned without submodules:
```text
git submodules update --init --recursive
```

## Building
```text
cmake -S . -B build
cmake --build build
```

## Running
The program expects two command line arguments. One for the binary and another for the ROM you wish to run
```text
.build/chip8 path/to/rom.ch8
```
Example:
```text
.build/chip8 roms/games/Pong (1 player).ch8
```
I recommend cloning the following repo into a folder named "roms"
```text
https://github.com/kripod/chip8-roms
```

## Controls
CHIP-8 uses a 16-key hexadecimal keypad. The emulator maps it to the keyboard like this:
```text
CHIP-8 Keypad        Keyboard
1 2 3 C              1 2 3 4
4 5 6 D              Q W E R
7 8 9 E              A S D F
A 0 B F              Z X C V
```
Note: Controls may vary depending on the ROM.

## Using my Repo
This project is a learning experience and was built for fun. Feel free to use it however you wish :D