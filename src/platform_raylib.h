#ifndef PLATFORM_RAYLIB_H
#define PLATFORM_RAYLIB_H

#include "chip8.h"

void chip8_render(struct chip8 *cpu);
void chip8_set_keypad(struct chip8 *cpu);

#endif 