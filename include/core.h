#ifndef CORE_H
#define CORE_H

#include <ncurses.h>
#include <cjson/cJSON.h>

typedef enum State { MENU, GAME, SETT, CLASSPICK } state;
typedef enum Class { ROGUE, KNIGHT, MAGE } class;

typedef struct Vector2 {
    int y;
    int x;
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

void check(bool is);
bool CoreInit();
bool MapInit();
void GameLoop();
void GameInputs(int input);

#endif
