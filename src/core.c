#include "core.h"
#include <stdlib.h>

state game_state = MENU;
player pl = { { 1, 1 }, '@', ROGUE };
map mp = { 0 };
map lvlone = { 0 };
map lvltwo = { 0 };
enemy enemies[MAX_ENEMIES];
int enemy_count = 0;
bool running = true;

bool CursesInit() {
    initscr();
    cbreak();
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    return true;
}
