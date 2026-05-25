#include "menu.h"
#include "chip8.h"

#define _DEFAULT_SOURC

int menu_get_count(const char *dir_path)
{
    DIR *dir_ptr;
    struct dirent *entry;
    int item_count = 0;
    //const char *chip8_extention = ".ch8";

    //const char *dir_path = "roms/games/";

    dir_ptr = opendir(dir_path);
    if (dir_ptr == NULL) {
        perror("Error opening directory");
        return -1;
    }

    while ((entry = readdir(dir_ptr)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (strstr(entry->d_name, CHIP8_EXTENTION) != NULL) {
            item_count++;
        }
            
        else {
            char full_path[1024];
            struct stat path_stat;

            snprintf(full_path, sizeof(full_path), "%s%s", dir_path, entry->d_name);

            if (stat(full_path, &path_stat) == 0) {
                if (S_ISDIR(path_stat.st_mode)) {
                    item_count++;
                }
            }
        }
    }

    closedir(dir_ptr);

    return item_count;
}

// int menu_create(struct MenuItem *items, int rom_count)
// {
//     DIR *dir_ptr;
//     struct dirent *entry;
//     int counter = 0;

//     const char *dir_path = "roms/games/";
    
//     dir_ptr = opendir(dir_path);
//     if (dir_ptr == NULL) {
//         perror("Error opening directory");
//         return EXIT_FAILURE;
//     }

//      while ((entry = readdir(dir_ptr)) != NULL && counter < rom_count) {
//         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
//             continue;
//         }
//         if (strstr(entry->d_name, CHIP8_EXTENTION) != NULL) {
//             strcpy(items[counter].item_display_name, entry->d_name);
//             counter++;
//         }
//     }
//     closedir(dir_ptr);
//     return EXIT_SUCCESS;
// }

int menu_load_directory(struct Menu *menu, const char *dir_path)
{
    printf("Current dir: %s\n", dir_path);
    DIR *dir_ptr = opendir(dir_path);
    if (dir_ptr == NULL) {
        perror("Error opening directory");
        return EXIT_FAILURE;
    }

    int item_count = menu_get_count(dir_path);
    if (item_count <= 0) {
        closedir(dir_ptr);
        return EXIT_FAILURE;
    }
    if (strcmp(dir_path, MENU_ROOT_DIR) != 0) item_count++;

    struct MenuItem *new_items = malloc(sizeof(struct MenuItem) * item_count);
    if (new_items == NULL) {
        perror("Error allocating menu items");
        closedir(dir_ptr);
        return EXIT_FAILURE;
    }

    struct dirent *entry;
    int counter = 0;

    if (strcmp(dir_path, MENU_ROOT_DIR) != 0) {
        strcpy(new_items[0].item_display_name, "..");

        snprintf(
                new_items[0].item_path,
                sizeof(new_items[0].item_path),
                "%s%s",
                dir_path,
                ".."
            );
            new_items[0].item_type = BACK;
            counter++;
    }

    while ((entry = readdir(dir_ptr)) != NULL && counter < item_count) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        if (strstr(entry->d_name, CHIP8_EXTENTION) != NULL) {
            strcpy(new_items[counter].item_display_name, entry->d_name);

            snprintf(
                new_items[counter].item_path,
                sizeof(new_items[counter].item_path),
                "%s%s",
                dir_path,
                entry->d_name
            );

            new_items[counter].item_type = ROM;
            counter++;
        }
        else {
            strcpy(new_items[counter].item_display_name, entry->d_name);

            struct stat path_stat;
            snprintf(
                new_items[counter].item_path,
                sizeof(new_items[counter].item_path),
                "%s%s",
                dir_path,
                entry->d_name
            );

            if (stat(new_items[counter].item_path, &path_stat) == 0) {
                if (S_ISDIR(path_stat.st_mode)) {
                    new_items[counter].item_type = FOLDER;
                    counter++;
                }
            }
        }
    }

    closedir(dir_ptr);

    if (menu->items != NULL) {
        free(menu->items);
    }

    menu->items = new_items;
    menu->item_count = counter;
    menu->selected_index = 0;

    strcpy(menu->current_directory, dir_path);

    //printf("Item count: %d\n", menu->item_count);
    return EXIT_SUCCESS;
}

bool menu_display(struct Menu *menu, char *dir_path, int menu_mode)
{
    //printf("Current directory: %s", menu->items->item_display_name);
    uint8_t num_pages = (menu->item_count + MAX_VISIBLE_ITEMS - 1) / MAX_VISIBLE_ITEMS;
    uint8_t current_page = (menu->selected_index / MAX_VISIBLE_ITEMS) + 1;

    uint8_t scroll_offset = (menu->selected_index / MAX_VISIBLE_ITEMS) * MAX_VISIBLE_ITEMS;
    uint16_t start_x = 10;
    uint16_t start_y = 10;
    
    int fontSize = 10;
    int paddingX = 4;
    int paddingY = 2;

    DrawText(TextFormat("Page %d/%d", current_page, num_pages), PAGE_INDEX_POS_X, PAGE_INDEX_POS_Y, fontSize, WHITE);

    for (size_t i = scroll_offset; (i < scroll_offset + MAX_VISIBLE_ITEMS) && (i < menu->item_count); i++) {
        bool is_selected = (i == menu->selected_index);

        if (is_selected) {
            int text_width = MeasureText(menu->items[i].item_display_name, fontSize);

            DrawRectangle(
                start_x - paddingX,
                start_y - paddingY,
                text_width + (paddingX * 2),
                fontSize + (paddingY * 2),
                WHITE
            );
            DrawText(menu->items[i].item_display_name, start_x, start_y, fontSize, BLACK);
        } else {
            DrawText(menu->items[i].item_display_name, start_x, start_y, fontSize, WHITE);
        }
        
        start_y += 15;
    }

    if (IsKeyPressed(KEY_W)) {
        menu->selected_index = (menu->selected_index - 1 + menu->item_count) % menu->item_count;
        printf("%d\n", menu->selected_index);
    }

    if (IsKeyPressed(KEY_S)) {
            menu->selected_index = (menu->selected_index + 1) % menu->item_count;
            printf("%d\n", menu->selected_index);
    }
           
    if (IsKeyPressed(KEY_ENTER)) {
            if (menu->items[menu->selected_index].item_type == ROM) {
                //rom_path = menu->items[menu->selected_index].item_display_name;
                strcat(dir_path, menu->items[menu->selected_index].item_display_name);
                // strcpy(menu->current_directory, rom_path);
                printf("Selected ROM path: %s\n", dir_path);
                return true;
            }
            else if (menu->items[menu->selected_index].item_type == FOLDER) {
                printf("%s\n", menu->items[menu->selected_index].item_display_name);
                strcpy(menu->current_directory_name, menu->items[menu->selected_index].item_display_name);
                strcat(dir_path, menu->items[menu->selected_index].item_display_name);
                strcat(dir_path, "/");
                strcpy(menu->current_directory, dir_path);
                menu_load_directory(menu, dir_path);

                
                return false;
            }
            else if (menu->items[menu->selected_index].item_type == BACK) {
                char *test = strstr(dir_path, menu->current_directory_name);

                if (test != NULL) {
                    *test = '\0';
                }
                printf("%s\n", dir_path);
                menu_load_directory(menu, dir_path);
            }       
    }
    
    return false;
}

