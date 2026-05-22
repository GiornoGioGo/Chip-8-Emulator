#include "platform_raylib.h"

static const int CHIP8_SCALE = 10;

void chip8_render(struct chip8 *cpu)
{
    for (size_t y = 0; y <= 31; y++) {
            for (size_t x = 0; x <= 63; x++) {
                size_t index = y * 64 + x;

                int screen_x = x * CHIP8_SCALE;
                int screen_y = y * CHIP8_SCALE;
                if (cpu->display[index] == 1)
                    DrawRectangle(screen_x, screen_y, CHIP8_SCALE, CHIP8_SCALE, WHITE);
            }
        }
}

void chip8_set_keypad(struct chip8 *cpu)
{
        cpu->keypad[0x1] = IsKeyDown(KEY_ONE);
        cpu->keypad[0x2] = IsKeyDown(KEY_TWO);
        cpu->keypad[0x3] = IsKeyDown(KEY_THREE);
        cpu->keypad[0xC] = IsKeyDown(KEY_FOUR);

        cpu->keypad[0x4] = IsKeyDown(KEY_Q);
        cpu->keypad[0x5] = IsKeyDown(KEY_W);
        cpu->keypad[0x6] = IsKeyDown(KEY_E);
        cpu->keypad[0xD] = IsKeyDown(KEY_R);

        cpu->keypad[0x7] = IsKeyDown(KEY_A);
        cpu->keypad[0x8] = IsKeyDown(KEY_S);
        cpu->keypad[0x9] = IsKeyDown(KEY_D);
        cpu->keypad[0xE] = IsKeyDown(KEY_F);

        cpu->keypad[0xA] = IsKeyDown(KEY_Z);
        cpu->keypad[0x0] = IsKeyDown(KEY_X);
        cpu->keypad[0xB] = IsKeyDown(KEY_C);
        cpu->keypad[0xF] = IsKeyDown(KEY_V);
}