#ifndef MENU_H
#define MENU_H

#include <raylib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define CHIP8_EXTENTION ".ch8"
#define NUM_ROMS 75
#define MAX_VISIBLE_ITEMS 21
#define PAGE_INDEX_POS_X 580
#define PAGE_INDEX_POS_Y 300

enum {
    MENU = 1,
    RUNNING = 2
};

int menu_get_count();
int menu_create(char **rom_list, int rom_count);
bool menu_display(char **rom_list, char *rom_path, int menu_mode);

#endif