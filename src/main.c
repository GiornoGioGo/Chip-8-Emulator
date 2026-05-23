#include "chip8.h"
#include "platform_raylib.h"
#include "menu.h"

int menu_mode;

int main(int argc, char *argv[])
{
    struct chip8 cpu;

    char rom_path[512] = "roms/games/";

    printf("Loaded ROM successfully\n");

    srand(time(NULL));

    int count = menu_get_count();
    char **rom_list = malloc(count * sizeof(char *));

    menu_create(rom_list, count);

    InitWindow(640, 320, "Chip-8");
    SetTargetFPS(60);

    int menu_count = menu_get_count();
    if (menu_count < 0) return 1;
    char menu_count_text[32];

    menu_mode = MENU;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        if (menu_mode == MENU) {
            bool seleted_rom = menu_display(rom_list, rom_path, menu_mode);

            if (seleted_rom) {
                chip8_init(&cpu);

                if (chip8_load_rom(&cpu, rom_path) != EXIT_SUCCESS) {
                    printf("Failed to load ROM. Exiting program.\n");
                    return 1;
                }
                menu_mode = RUNNING;
            }
        }
        else if (menu_mode == RUNNING) {
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
            chip8_render(&cpu);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}