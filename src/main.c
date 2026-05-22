#include "chip8.h"
#include "platform_raylib.h"

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
        return 1;
    }

    printf("Loaded ROM successfully\n");

    srand(time(NULL));

    InitWindow(640, 320, "Chip-8");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        chip8_set_keypad(&cpu);

        for (int i = 0; i < 10; i++) {
            chip8_cycle(&cpu);
        }

        if (cpu.delay_timer > 0) {
            cpu.delay_timer--;
        }

        if (cpu.sound_timer > 0) {
            cpu.sound_timer--;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        chip8_render(&cpu);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}