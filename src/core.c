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

void check(bool is) {
    if (!is) {
        endwin();
    }
}

bool CoreInit() {
    initscr();
    cbreak();
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    check(has_colors());
    return true;
}

bool MapInit() {
    for (int y = 0; y < mapMaxSizeY; y++) {
        for (int x = 0; x < mapMaxSizeX; x++) {
            if (y == 0 || y == mapMaxSizeY - 1 || x == 0 || x == mapMaxSizeX - 1)
                mp.map[y][x] = '#';
            else
                mp.map[y][x] = 0;
        }
    }
    return true;
}

void GameLoop() {
    clear();
    for (int y = 0; y < mapMaxSizeY; y++) {
        for (int x = 0; x < mapMaxSizeX; x++) {
            if (mp.map[y][x] == 1)
                mvprintw(y, x, "#");
        }
    }
    for (int i = 0; i < enemy_count; i++) {
        mvprintw(enemies[i].pos.y, enemies[i].pos.x, "%c", enemies[i].vis);
    }
    mvprintw(pl.pos.y, pl.pos.x, "@");
    refresh();
}

void GameInputs(int input) {
    switch (input) {
        case KEY_UP: if (pl.pos.y > 0) pl.pos.y--; break;
        case KEY_DOWN: if (pl.pos.y < mapMaxSizeY - 1) pl.pos.y++; break;
        case KEY_LEFT: if (pl.pos.x > 0) pl.pos.x--; break;
        case KEY_RIGHT: if (pl.pos.x < mapMaxSizeX - 1) pl.pos.x++; break;
        case 'e': SpawnEnemy((enemy){.pos = {2, 2}, .vis = '&'}); break;
        default: break;
    }
}

void SpawnEnemy(enemy e) {
    if (enemy_count >= MAX_ENEMIES) return;
    enemies[enemy_count++] = e;
}
