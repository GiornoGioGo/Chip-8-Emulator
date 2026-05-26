#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define STACK_DEPTH 16

enum 
{
    CHIP8_MEMORY_SIZE = 4096,
    CHIP8_DISPLAY_WIDTH = 64,
    CHIP8_DISPLAY_HEIGHT = 32,
    CHIP8_DISPLAY_SIZE = CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_HEIGHT,
    CHIP8_V_REGISTER_SIZE = 16,
    CHIP8_STACK_SIZE = 16,
    CHIP8_KEY_COUNT = 16,
    CHIP8_PROGRAM_START = 0x200,
    CHIP8_FONT_START = 0x050,
    CHIP8_FONTSET_SIZE = 80
};

struct chip8
{
    uint8_t  memory[CHIP8_MEMORY_SIZE];
    uint8_t  display[CHIP8_DISPLAY_SIZE];
    uint8_t  V[CHIP8_V_REGISTER_SIZE];
    uint16_t stack[CHIP8_STACK_SIZE];
    uint16_t pc;
    uint16_t I;
    uint8_t  sp;
    uint8_t  delay_timer;
    uint8_t  sound_timer;
    uint8_t  keypad[CHIP8_KEY_COUNT];
};

static const uint8_t fontset[80] = {
    // 0
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    // 1
    0x20, 0x60, 0x20, 0x20, 0x70,
    // 2
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    // 3
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    // 4
    0x90, 0x90, 0xF0, 0x10, 0x10,
    // 5
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    // 6
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    // 7
    0xF0, 0x10, 0x20, 0x40, 0x40,
    // 8
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    // 9
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    // A
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    // B
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    // C
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    // D
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    // E
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    // F
    0xF0, 0x80, 0xF0, 0x80, 0x80
};

void chip8_init(struct chip8 *cpu);
int chip8_load_rom(struct chip8 *cpu, const char *rom_path);
void chip8_cycle(struct chip8 *cpu);

#endif 
