#include "menu.h"
#include "chip8.h"

int menu_get_count()
{
    DIR *dir_ptr;
    struct dirent *entry;
    int file_count = 0;
    //const char *chip8_extention = ".ch8";

    const char *dir_path = "roms/games/";

    dir_ptr = opendir(dir_path);
    if (dir_ptr == NULL) {
        perror("Error opening directory");
        return -1;
    }

    while ((entry = readdir(dir_ptr)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (strstr(entry->d_name, CHIP8_EXTENTION) != NULL)
            file_count++;
    }

    closedir(dir_ptr);

    return file_count;
}

int menu_create(char **rom_list, int rom_count)
{
    DIR *dir_ptr;
    struct dirent *entry;
    int counter = 0;

    const char *dir_path = "roms/games/";
    
    dir_ptr = opendir(dir_path);
    if (dir_ptr == NULL) {
        perror("Error opening directory");
        return EXIT_FAILURE;
    }

     while ((entry = readdir(dir_ptr)) != NULL && counter < rom_count) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (strstr(entry->d_name, CHIP8_EXTENTION) != NULL) {
            rom_list[counter] = malloc(strlen(entry->d_name) + 1);
            if (rom_list[counter] != NULL) {
                strcpy(rom_list[counter], entry->d_name);
                counter++;
            }
        }
    }
    closedir(dir_ptr);
    return EXIT_SUCCESS;
}

bool menu_display(char **rom_list, char *rom_path, int menu_mode)
{
    static uint8_t selected_index = 0;
    uint16_t start_x = 10;
    uint16_t start_y = 10;
    for (size_t i = 0; i < NUM_ROMS; i++) {
        DrawText(rom_list[i], start_x, start_y, 10, WHITE);
        start_y += 15;
    }

    if (IsKeyPressed(KEY_W)) {
        selected_index = (selected_index - 1 + NUM_ROMS) % NUM_ROMS;
        printf("%d\n", selected_index);
    }

    if (IsKeyPressed(KEY_S)) {
            selected_index = (selected_index + 1) % NUM_ROMS;
            printf("%d\n", selected_index);
        }
           
        if (IsKeyPressed(KEY_ENTER)) {
            strcat(rom_path, rom_list[selected_index]);
            printf("Selected ROM path: %s\n", rom_path);
            return true;
        }
    
    return false;
}