#include "chip8.h"

void chip8_render(struct chip8 *cpu) 
{
    for (size_t y = 0; y <= 31; y++) {
            for (size_t x = 0; x <= 63; x++) {
                size_t index = y * 64 + x;

                int screen_x = x * 10;
                int screen_y = y * 10;
                if (cpu->display[index] == 1)
                    DrawRectangle(screen_x, screen_y, 10, 10, WHITE);
            }
        }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Error: Please provide a file path.\n");
        printf("Usage: %s <file_path>\n", argv[0]);
        return 1;
    }
    struct chip8 cpu;
    chip8_init(&cpu);
    char *romPath = argv[1];
    if (chip8_load_rom(&cpu, romPath) != EXIT_SUCCESS) {
        printf("Failed to load ROM. Exiting program.\n");
        return -1;
    } else {
        printf("Loaded ROM successfully\n");
    }

    InitWindow(640, 320, "Chip-8");
    SetTargetFPS(60);

    srand(time(NULL));

    while (!WindowShouldClose()) {
        cpu.keypad[0x1] = IsKeyDown(KEY_ONE);
        cpu.keypad[0x2] = IsKeyDown(KEY_TWO);
        cpu.keypad[0x3] = IsKeyDown(KEY_THREE);
        cpu.keypad[0xC] = IsKeyDown(KEY_FOUR);

        cpu.keypad[0x4] = IsKeyDown(KEY_Q);
        cpu.keypad[0x5] = IsKeyDown(KEY_W);
        cpu.keypad[0x6] = IsKeyDown(KEY_E);
        cpu.keypad[0xD] = IsKeyDown(KEY_R);

        cpu.keypad[0x7] = IsKeyDown(KEY_A);
        cpu.keypad[0x8] = IsKeyDown(KEY_S);
        cpu.keypad[0x9] = IsKeyDown(KEY_D);
        cpu.keypad[0xE] = IsKeyDown(KEY_F);

        cpu.keypad[0xA] = IsKeyDown(KEY_Z);
        cpu.keypad[0x0] = IsKeyDown(KEY_X);
        cpu.keypad[0xB] = IsKeyDown(KEY_C);
        cpu.keypad[0xF] = IsKeyDown(KEY_V);

        for (int i = 0; i < 10; i++) {
            chip8_cycle(&cpu);
        }

        if (cpu.delay_timer > 0)
            cpu.delay_timer--;
        
        if (cpu.sound_timer > 0)
            cpu.sound_timer--;
        
        BeginDrawing();
        ClearBackground(BLACK);

        chip8_render(&cpu);
        //DrawText("Hello Chip-8", 20, 20, 20, WHITE);
     

        EndDrawing();
    }

    CloseWindow();
    return 0;
}