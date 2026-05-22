#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "raylib.h"

#define STACK_DEPTH 16

struct chip8
{
    uint8_t  memory[4096];
    uint8_t  display[64 * 32];
    uint8_t  V[16];
    uint16_t stack[STACK_DEPTH];
    uint16_t pc;
    uint16_t I;
    uint8_t  sp;
    uint8_t  delay_timer;
    uint8_t  sound_timer;
    uint8_t  keypad[16];
};

void chip8_init(struct chip8 *cpu);
int chip8_load_rom(struct chip8 *cpu, const char *rom_path);
void chip8_cycle(struct chip8 *cpu);
