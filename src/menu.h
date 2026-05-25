#ifndef MENU_H
#define MENU_H

#include <raylib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

#define CHIP8_EXTENTION ".ch8"
#define NUM_ROMS 75
#define MAX_VISIBLE_ITEMS 21
#define PAGE_INDEX_POS_X 580
#define PAGE_INDEX_POS_Y 300
#define MENU_ROOT_DIR "roms/"

enum {
    MENU = 1,
    RUNNING = 2
};

enum {
    FOLDER = 1,
    ROM = 2,
    BACK = 3
};

struct MenuItem {
    char item_display_name[256];
    char item_path[512];
    int item_type;
};

struct Menu {
    char current_directory[256];
    char current_directory_name[64];
    int item_count;
    int selected_index;
    struct MenuItem *items;
};

int menu_get_count(const char *dir_path);
int menu_create(struct MenuItem *items, int rom_count);
int menu_load_directory(struct Menu *menu, const char *dir_path);
bool menu_display(struct Menu *menu, char *rom_path, int menu_mode);

#endif