#include "chip8.h"
#include "platform_raylib.h"
#include "menu.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320
#define WINDOW_TITLE "chip-8"
#define WINDOW_FPS 60

int main(int argc, char *argv[])
{
    struct chip8 cpu;

    srand(time(NULL));
    
    struct Menu menu = {0};
    menu.items = NULL;

    struct PauseMenu pause = {0};
    menu_pause_init(&pause);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(WINDOW_FPS);

    menu_load_directory(&menu, "roms/");

    char menu_count_text[32];

    char dir_path[512] = "roms/";

    menu.menu_mode = MENU;

    menu.is_paused = false;

    bool should_quit = false;

    while (!WindowShouldClose() && !should_quit) {
        BeginDrawing();
        ClearBackground(BLACK);
        int dir_size = strlen(dir_path);

        if (menu.menu_mode == MENU) {
            if (contains_rom(dir_path, dir_size)) {
                menu_eject_rom(dir_path);
            }
            bool seleted_rom = menu_display(&menu, dir_path,  menu.menu_mode);

            if (seleted_rom) {
                chip8_init(&cpu);

                if (chip8_load_rom(&cpu, dir_path) != EXIT_SUCCESS) {
                    printf("Failed to load ROM. Exiting program.\n");
                    return 1;
                }
                menu.menu_mode = RUNNING;
            }
        }
        else if (menu.menu_mode == RUNNING) {
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
                menu.menu_mode = PAUSED;
            }
        }
        else if (menu.menu_mode == PAUSED) {
            menu_pause_display(&pause);
            menu_get_menu_mode(&menu, &pause, menu.menu_mode, &should_quit);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}