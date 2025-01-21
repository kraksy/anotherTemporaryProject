#ifndef CORE_H
#define CORE_H

#include "ascii_art.h"
#include "classpick.h"
#include "enemy.h"
#include "map.h"
#include "menu.h"
#include "settings.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <cjson/cJSON.h>

#define uint8 uint8_t
#define uint16 uint16_t
#define uint32 uint32_t
#define uint64 uint64_t

#define int8 int8_t
#define int16 int16_t
#define int32 int32_t
#define int64 int64_t

#define MAP_MAX_SIZEY 50
#define MAP_MAX_SIZEX 50

#define MAX_ENEMIES 100
#define MAX_MAPS 10 

#define MAX_BUTTONS_SIZEY 5
#define MAX_BUTTONS_siZEX 20

typedef enum State { MENU, GAME, SETT, CLASSPICK } state;
typedef enum Class { ROGUE, KNIGHT, MAGE } class;

typedef struct Vector2 {
    int8 y;
    int8 x;
} Vec2;

typedef struct {
    int *array;
    size_t used;
    size_t size;
} Array;

typedef struct {
    char *array;
    size_t used;
    size_t size;
} CharArray;

typedef struct Tile {
    Vec2 pos;
    char vis;
} tile;

typedef struct Player {
    Vec2 pos;
    char vis;
    class cls; // player picks before the game starts
} player;

typedef struct Enemy {
    Vec2 pos;
    char vis;
} enemy;

typedef struct Map {
    int id;
    Array *map;
} map;

typedef struct Button {
    char text;
    Vec2 pos;
    CharArray button;
    bool selected;
} button;

extern state game_state;
extern player pl;
extern map mp, lvlone, lvltwo;
extern enemy enemies[MAX_ENEMIES];
extern int enemy_count;

extern bool running;

bool CursesInit();

#endif
