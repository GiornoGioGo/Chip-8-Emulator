#ifndef MENU_H
#define MENU_H

#include <raylib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

#define NUM_ROMS 75
#define MAX_VISIBLE_ITEMS 21
#define PAGE_INDEX_POS_X 580
#define PAGE_INDEX_POS_Y 300
#define PAUSE_OPTION_COUNT 3

#define CHIP8_EXTENTION ".ch8"
#define MENU_ROOT_DIR "roms/"
#define PAUSE_MENU_TITLE "Paused"
#define PAUSE_MENU_RESUME "Resume"
#define PAUSE_MENU_RETURN "Rom Menu"
#define PAUSE_MENU_EXIT "EXIT"

enum 
{
    MENU = 1,
    RUNNING = 2,
    PAUSED = 3
};

enum 
{
    FOLDER = 1,
    ROM = 2,
    BACK = 3
};

struct MenuItem 
{
    char item_display_name[256];
    char item_path[512];
    int item_type;
};

struct Menu 
{
    char current_directory[256];
    char current_directory_name[64];
    int item_count;
    int selected_index;
    int menu_mode;
    struct MenuItem *items;
    bool is_paused;
};

struct PauseMenu
{
    int selected_index;
    int option_count;
    const char *title;
    const char *options[PAUSE_OPTION_COUNT];
};

int menu_get_count(const char *dir_path);
int menu_create(struct MenuItem *items, int rom_count);
int menu_load_directory(struct Menu *menu, const char *dir_path);
bool menu_display(struct Menu *menu, char *rom_path, int menu_mode);
void menu_pause_init(struct PauseMenu *pause);
void menu_pause_display(struct PauseMenu *pause);
void menu_get_parent_directory(char *dest, const char *dir_path);
void menu_set_parent_directory(struct Menu *menu, const char *dir_path);
void menu_get_menu_mode(struct Menu *menu, struct PauseMenu *pause, int menu_mode, bool *should_quit);
bool contains_rom(const char *dir_path, int size);
void menu_eject_rom(char *dir_path);
  

#endif