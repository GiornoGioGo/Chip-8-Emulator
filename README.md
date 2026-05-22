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