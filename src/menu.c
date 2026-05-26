#include "menu.h"
#include "chip8.h"

#define _DEFAULT_SOURC

int menu_get_count(const char *dir_path)
{
    DIR *dir_ptr;
    struct dirent *entry;
    int item_count = 0;

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

void menu_get_parent_directory(char *dest, const char *dir_path)
{
    strcpy(dest, dir_path);

    char *last_slash = strrchr(dest, '/');

    if (last_slash != NULL) {
        *last_slash = '\0';

        char *second_last_slash = strrchr(dest, '/');

        if (second_last_slash != NULL) {
            *(second_last_slash + 1) = '\0';
        } else {
            *last_slash = '/';
        }
    }
}

bool contains_rom(const char *dir_path, int size)
{
    if (dir_path == NULL || size <= 0) {
        return false; 
    }

    int last_index = size - 1;

    if (dir_path[last_index] == '8') {
        return true;
    }
    
    return false;
}

void menu_eject_rom(char *dir_path)
{
    char *last_slash = strrchr(dir_path, '/');

    if (last_slash != NULL) {
        *(last_slash + 1) = '\0'; 
    }
}

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

        char parent_dir[64];
        menu_get_parent_directory(parent_dir, dir_path);
        snprintf(
                new_items[0].item_path,
                sizeof(new_items[0].item_path),
                "%s",
                parent_dir
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
    return EXIT_SUCCESS;
}

bool menu_display(struct Menu *menu, char *dir_path, int menu_mode)
{
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
                strcat(dir_path, menu->items[menu->selected_index].item_display_name);
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
                strcpy(dir_path, menu->items[menu->selected_index].item_path);
                strcpy(menu->current_directory, dir_path); 
                menu_load_directory(menu, menu->items[menu->selected_index].item_path);

                return false;
            }       
    }
    
    return false;
}

void menu_pause_init(struct PauseMenu *pause)
{
    pause->selected_index = 0;
    pause->option_count = 3;
    pause->title = "Paused";
    pause->options[0] = "Resume";
    pause->options[1] = "Rom Menu";
    pause->options[2] = "Exit";
}

void menu_pause_display(struct PauseMenu *pause)
{

    if (IsKeyPressed(KEY_S))
        pause->selected_index++;
    if (IsKeyPressed(KEY_W))
        pause->selected_index--;

    if (pause->selected_index >= pause->option_count)
        pause->selected_index = 0;
    if (pause->selected_index < 0)
        pause->selected_index = 2;

    int titleSize = 50;
    int titleX = (GetScreenWidth() / 2) - (MeasureText(pause->title, titleSize) / 2);
    int titleY = (GetScreenHeight() * 0.2); // 30% down
    DrawText(pause->title, titleX, titleY, titleSize, WHITE);

    int menuFontSize = 30;
    int startY = titleY + 80; 
    int lineSpacing = 50;     

    int x1 = (GetScreenWidth() / 2) - (MeasureText(pause->options[0], menuFontSize) / 2);
    int y1 = startY + (0 * lineSpacing);
    if (pause->selected_index == 0) {
        DrawText(">", x1 - 20, y1, menuFontSize, WHITE);
        DrawText(pause->options[0], x1, y1, menuFontSize, GRAY);
    }
    else {
        DrawText(pause->options[0], x1, y1, menuFontSize, GRAY);
    }

    int x2 = (GetScreenWidth() / 2) - (MeasureText(pause->options[1], menuFontSize) / 2);
    int y2 = startY + (1 * lineSpacing);
    if (pause->selected_index == 1) {
        DrawText(">", x2 - 20, y2, menuFontSize, WHITE);
        DrawText(pause->options[1], x2, y2, menuFontSize, GRAY);
    }
    else {
        DrawText(pause->options[1], x2, y2, menuFontSize, GRAY);
    }

    int x3 = (GetScreenWidth() / 2) - (MeasureText(pause->options[2], menuFontSize) / 2);
    int y3 = startY + (2 * lineSpacing);
    if (pause->selected_index == 2) {
        DrawText(">", x3 - 20, y3, menuFontSize, WHITE);
        DrawText(pause->options[2], x3, y3, menuFontSize, GRAY);
    }
    else {
        DrawText(pause->options[2], x3, y3, menuFontSize, GRAY);
    }
}

void menu_get_menu_mode(struct Menu *menu, struct PauseMenu *pause, int menu_mode, bool *should_quit)
{

    if (IsKeyPressed(KEY_P)) {
                menu->menu_mode = RUNNING;
            }

    if (IsKeyPressed(KEY_ENTER)) {
                switch (pause->selected_index) {
                    case 0:
                        menu->menu_mode = RUNNING;
                        break;
                    case 1:
                        menu->menu_mode = MENU;
                        break;
                    case 2:
                        *should_quit = true;
                        break;
                }
        }
}