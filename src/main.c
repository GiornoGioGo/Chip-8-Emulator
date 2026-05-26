#include "chip8.h"
#include "platform_raylib.h"
#include "menu.h"

int menu_mode;

int main(int argc, char *argv[])
{
    struct chip8 cpu;

    srand(time(NULL));
    
    struct Menu menu = {0};
    menu.items = NULL;

    struct PauseMenu pause = {0};
    menu_pause_init(&pause);

    InitWindow(640, 320, "Chip-8");
    SetTargetFPS(60);

    menu_load_directory(&menu, "roms/");

    char menu_count_text[32];

    char dir_path[512] = "roms/";

    menu_mode = MENU;

    menu.is_paused = false;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        if (menu_mode == MENU) {
            bool seleted_rom = menu_display(&menu, dir_path, menu_mode);

            if (seleted_rom) {
                chip8_init(&cpu);

                if (chip8_load_rom(&cpu, dir_path) != EXIT_SUCCESS) {
                    printf("Failed to load ROM. Exiting program.\n");
                    return 1;
                }
                menu_mode = RUNNING;
            }
        }
        else if (menu_mode == RUNNING) {
            chip8_set_keypad(&cpu);

            for (int i = 0; i < 12; i++) {
                chip8_cycle(&cpu);
            }

            if (cpu.delay_timer > 0) {
                cpu.delay_timer--;
            }

            if (cpu.sound_timer > 0) {
                cpu.sound_timer--;
            }
            chip8_render(&cpu);

            if (IsKeyPressed(KEY_P)) {
                menu_mode = PAUSED;
            }
        }
        else if (menu_mode == PAUSED) {

            menu_pause_display(&pause);

            if (IsKeyPressed(KEY_P)) {
                menu_mode = RUNNING;
            }

            if (IsKeyPressed(KEY_ENTER)) {
                switch (pause.selected_index) {
                    case 0:
                        menu_mode = RUNNING;
                        break;
                    case 1:
                        menu_mode = MENU;
                        break;
                    case 2:
                        CloseWindow();
                        break;
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}