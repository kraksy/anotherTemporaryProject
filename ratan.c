///////////////////////////////////////////////////////////////////
///
/// RATAN 
//
/// date year : 2024
/// author : krek 
/// 
///////////////////////////////////////////////////////////////////

#include <ncurses.h>
#include <cjson/cJSON.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define mapMaxSizeY 30
#define mapMaxSizeX 30

#define MAX_ENEMIES 100
#define MAX_MAPS 10

#define maxButtonSizeX 20
#define maxButtonSizeY 20

// enums
typedef enum State { MENU , GAME , SETT, CLASSPICK }state;
typedef enum Class { ROGUE , KNIGHT, MAGE }class;

typedef struct Vector2 {
    int y;
    int x;
} Vec2;

typedef struct {
  int *array;
  size_t used;
  size_t size;
}Array;

typedef struct 
{
  int **array;
  size_t used;
  size_t size;
}d_array;

typedef struct {
  char *array;
  size_t used;
  size_t size;
}CharArray;

typedef struct Tile {
    Vec2 pos;
    char vis;
} tile;

typedef struct Player {
    Vec2 pos;
    char vis;
    class cls; //  player picks before the game starts
} player;

typedef struct Enemy {
    Vec2 pos;
    char vis;
}enemy;

typedef struct Map {
    int id;
    d_array map;
} map;

typedef struct Button {
  char text;
  Vec2 pos;
  CharArray button;
  bool selected;
}button;

// struct definition here
static enum State game_state = MENU;

static struct Player pl = { { 1, 1 } };

static struct Map mp = { 0 };
static struct Map lvlone = { 0 };
static struct Map lvltwo = { 0 };

static struct Enemy en = { 0 };

static struct Tile air = {
  (Vec2) { 0 },
  '.'
};

static struct Tile wall = {
  (Vec2) { 0 },
  '#'
};

static struct Tile door = {
  (Vec2) { 0 },
  '+'
};

// values
static bool running;

// menu logic 

static void DrawMenuBorder();

static void MenuInputs(int input, int*selected);
static void DrawMenu(int selected);

static int Menu_Selected;

// class pick

static void DrawClassPickBorder();
static void ClassPickInputs(int input, int*selected);
static int ClassPick_Selected();

// Settings logic 

static void DrawSettingsBorder();

static int Settings_Selected;

static void SettingsInputs(int input, int*selected);
static void DrawSettings(int selected);

//ascii sht

static void DrawAsciiRat(int startY, int startX);
static void DrawLogo(int startY, int startX);

// enemy logic 
static enemy enemies[MAX_ENEMIES];
static int enemy_count = 0;

static void SpawnEnemy(enemy e);
static void UpdateEnemies();

// map 

static map maps[MAX_MAPS];
static int map_count;

static int GetTile(map mp, Vec2 pos);

static void DrawMap(map *map);

// inits
static bool CoreInit();

static bool MapInit();

// gui

static void CreateButton(button *btn, Vec2 pos, const char *text);
static void DrawButton(button btn, Vec2 pos);

// tools
static void check(bool is);
static void switchState(state st);

// loops
static void GameLoop();
static void GameInputs(int input);

// ascii

const char *logo = "               __                  \n"
                   "____________ _/  |______    ____   \n"
                   "\\_  __ \\__  \\\\   __\\__  \\  /    \\  \n"
                   " |  | \\\\/ __ \\|  |  / __ \\|   |  \\ \n"
                   " |__|  (____  /__| (____  /___|  / \n"
                   "            \\/          \\/     \\/  \n";

const char *ascii_art =
    "         __             _,-\"~^\"-.\n"
    "       _// )      _,-\"~`         `.\n"
    "     .\" ( /`\"-,-\"`                 ;\n"
    "   / 6                             ;\n"
    "  /           ,             ,-\"     ;\n"
    " (,__.--.      \\           /        ;\n"
    "  //'   /`-.\\   |          |        `._________\n"
    "    _.-'_/`  )  )--...,,,___\\     \\-----------,)\n"
    "  (((\"~` _.-'.-'           __`-.   )         //\n"
    "    jgs (((\"`             (((---~\"`         //\n"
    "                                           ((________________\n"
    "                                           `----\"\"\"\"~~~~^^^```\n";

// for the dynamic GUI
void initArray(Array *a, size_t Size) {
  a->array = malloc(Size * sizeof(int));
  a->used = 0;
  a->size = Size;
}

void insertArray(Array *a, int element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = realloc(a->array, a->size * sizeof(int));
  }
  a->array[a->used++] = element;
}

void initCharArray(CharArray *a, size_t size)
{
  a->array = (char *)malloc(size * sizeof(char));
  a->used = 0;
  a->size = size;
}

void appendToCharArray(CharArray *array, char c)
{
  if (array->used == array->size) {
    array->size *= 2;
    array->array = realloc(array->array, array->size * sizeof(char));
  }
  array->array[array->used++] = c;
}

void freeCharArray(CharArray *array)
{
  free(array->array);
}

/*
void CreateButton(button *btn, Vec2 pos, const char *text)
{
  btn->pos.y = pos.y;
  btn->pos.x = pos.x;
  btn->selected = false;
  initCharArray(&btn->button, strlen(text));

  for (size_t i = 0; text[i] != "\0"; i++){
    appendToCharArray(&btn->button, text[i]);
  }
}
*/

void freeButton(button *btn)
{
  freeCharArray(&btn->button);
}

/*
button CreateButton(char txt)
{
  Array a;
  int sizeY;
  int sizeX;

  button btn = { 0 };
  btn.text = txt;

  int Barlenght = strlen(&btn.text);
  

  // entire bar size
  size = Barlenght+2;

  if(size == 0|| Barlenght == maxButtonSizeX)
  {
    return (button){0};
  }

  initArray(&a, Barlenght);


  for (int i = 0; i < size; i++)
  {
    for (int x = ;x < size)
    if (i == 0 || i == size-1) a.array[0] = '#';
    
    
    
    a.array[Barlenght] = btn.text;
  }

  return btn;
}
*/


void DrawClassPickBorder()
{
  clear();
  char HorizontalLine = 'x';
  char VerticalLine = 'x';
  char empty = ' ';
  char edge = '#';
  int terminal_width = COLS;
  int terminal_height = LINES;

  char border[terminal_height][terminal_width];

  for (int y = 0; y < terminal_height; y++) {
    for (int x = 0; x < terminal_width; x++) {
      // fill full
      border[y][x] = empty;

      // left and right
      border[y][0] = VerticalLine;
      border[y][terminal_width - 1] = VerticalLine;

      // bottom and up
      border[0][x] = HorizontalLine;
      border[terminal_height - 1][x] = HorizontalLine;

      // edges
      border[0][0] = edge;
      border[terminal_height - 1][0] = edge;
      border[0][terminal_width - 1] = edge;
      border[terminal_height - 1][terminal_width - 1] = edge;
      mvaddch(y, x, border[y][x]);
    }
  }
  refresh();
}

void ClassPickInputs(int input, int *selected)
{
  switch (input) {
  case KEY_UP:
    if (*selected > 0) {
      (*selected)--;
    }
    break;

  case KEY_DOWN:
    if (*selected < 2) {
      (*selected)++;
    }
    break;
  case '\n':
    switch (*selected) {
    case 0:
      pl.cls = ROGUE;
      game_state = GAME;
      break;
    case 1:
      pl.cls = KNIGHT;
      game_state = GAME;
      break;
    case 2:
      pl.cls = MAGE;
      game_state = GAME;
      break;
    }
    break;
  }

}

int GetTile(map mp ,Vec2 pos)
{
  return mp.map.array[pos.y][pos.x];
}

void DrawMenuBorder() {
  clear();
  char HorizontalLine = '-';
  char VerticalLine = '|';
  char empty = ' ';
  char edge = '#';
  int terminal_width = COLS;
  int terminal_height = LINES;

  char border[terminal_height][terminal_width];

  for (int y = 0; y < terminal_height; y++) {
    for (int x = 0; x < terminal_width; x++) {
      // fill full
      border[y][x] = empty;

      // left and right
      border[y][0] = VerticalLine;
      border[y][terminal_width - 1] = VerticalLine;

      // bottom and up
      border[0][x] = HorizontalLine;
      border[terminal_height - 1][x] = HorizontalLine;

      // edges
      border[0][0] = edge;
      border[terminal_height - 1][0] = edge;
      border[0][terminal_width - 1] = edge;
      border[terminal_height - 1][terminal_width - 1] = edge;
      mvaddch(y, x, border[y][x]);
    }
  }
  refresh();
}

void DrawAsciiRat(int startY, int startX)
{
  int y = startY;
  int x = startX;
  int i = 0;

  while (ascii_art[i] != '\0') {
    if (ascii_art[i] == '\n') { // Move to the next line
      y++;
      x = startX; // Reset x to the starting column
    } else {
      mvaddch(y, x, ascii_art[i]); // Print character at the specified location
      x++;
    }
    i++;
  }
}

void DrawLogo(int startY, int startX) {
  int y = startY;
  int x = startX;
  int i = 0;

  while (logo[i] != '\0') {
    if (logo[i] == '\n') {
      y++;
      x = startX;
    } else {
      mvaddch(y, x, logo[i]);
      x++;
    }
    i++;
  }
}

// buttons 


//todo redo this with the gui functions 
char StartButton[3][6] = {{'-', '-', '-', '-', '-', '-'},
                          {'|', 'p', 'l', 'a', 'y', '|'},
                          {'-', '-', '-', '-', '-', '-'}};

char SettingsButton[3][6] = {{'-', '-', '-', '-', '-', '-'},
                             {'|', 's', 'e', 't', 't', '|'},
                             {'-', '-', '-', '-', '-', '-'}};

char ExitButton[3][6] = {{'-', '-', '-', '-', '-', '-'},
                         {'|', ' ', 'e', 'n', 'd', '|'},
                         {'-', '-', '-', '-', '-', '-'}};


void DrawMenu(int selected)
{
  int startRow = 10;
  int startCol = 20;
  int settRow = 15;
  int settCol = 25;
  int endRow = 20;
  int endCol = 30;
  DrawMenuBorder(); 
  DrawLogo(25, 55);
  DrawAsciiRat(14, 50);
  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 6; x++) {
      if (selected == 0) {
        attron(COLOR_PAIR(1));
      }
      mvaddch(startRow + y, startCol + x, StartButton[y][x]);
      attroff(COLOR_PAIR(1));
    if (selected == 1) {
      attron(COLOR_PAIR(1));
    }
      mvaddch(settRow + y, settCol + x, SettingsButton[y][x]);
      attroff(COLOR_PAIR(1));
    if (selected == 2) {
      attron(COLOR_PAIR(1));
    }
    mvaddch(endRow + y, endCol + x, ExitButton[y][x]);
    attroff(COLOR_PAIR(1));
    }
  }
}

void MenuInputs(int input, int *selected) {
  switch (input) {
  case KEY_UP:
    if (*selected > 0) {
      (*selected)--;
    }
    break;

  case KEY_DOWN:
    if (*selected < 2) {
      (*selected)++;
    }
    break;
  case '\n':
    switch (*selected) {
    case 0:
      printw("start");
      game_state = GAME;
      break;
    case 1:
      printw("sett");
      game_state = SETT;
      break;
    case 2:
      printw("exit");
      running = false;
      break;
    }
    break;
  }
}

void SpawnEnemy(enemy e)
{
  if (enemy_count >= MAX_ENEMIES) return;

  e.vis = '&';
  e.pos = (Vec2){2, 2};

  enemies[enemy_count++] = e;
}

void UpdateEnemies()
{
  for (int i = 0; i < enemy_count; i++)
  {
    // random movement 
    int direction = rand() % 4;
    switch (direction)
      {
      case 0: if (enemies[i].pos.y > 0) enemies[i].pos.y--; break; // Up
      case 1: if (enemies[i].pos.y < mapMaxSizeY - 1) enemies[i].pos.y++; break; // Down
      case 2: if (enemies[i].pos.x > 0) enemies[i].pos.x--; break; // Left
      case 3: if (enemies[i].pos.x < mapMaxSizeX - 1) enemies[i].pos.x++; break; // Right
      default: break;
    } 
  }
}

// checking func for inits
void check(bool is) {
    if (!is) {
        endwin();
    }
}

bool CoreInit()
{
    initscr();
    cbreak();
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    init_pair(1, COLOR_RED, COLOR_BLACK); // red text , black bg for selected stuff
    init_pair(2, COLOR_BLUE, COLOR_BLACK); // if I want to add another one
    
    check(has_colors());
    return true;
}

bool MapInit()
{
    for (int y = 0; y < mapMaxSizeY; y++)
    {
        for (int x = 0; x < mapMaxSizeX; x++)
        {
            if (y == 0 || y == mapMaxSizeY - 1 || x == 0 || x == mapMaxSizeX - 1)
                mp.map.array[y][x] = '#'; // Boundary wall
            else
                mp.map.array[y][x] = 0; // Empty space
        }
    }
    return true;
}

void GameLoop()
{
    clear();

    // draw map
    for (int y = 0; y < mapMaxSizeY; y++)
    {
        for (int x = 0; x < mapMaxSizeX; x++)
        {
            if (mp.map.array[y][x] == 1)
                mvprintw(y, x, "#");
        }
    }
    
    // enemy rendering
    for (int i = 0; i < enemy_count; i++)
    {
      mvprintw(enemies[i].pos.y, enemies[i].pos.x, "%c", enemies[i].vis);
    }

    // draw player
    mvprintw(pl.pos.y, pl.pos.x, "@");
    refresh();
}

void GameInputs(int input)
{
    switch (input)
    {
    case KEY_UP:
        if (pl.pos.y > 0)
            pl.pos.y--;
        break;
    case KEY_DOWN:
        if (pl.pos.y < mapMaxSizeY - 1)
            pl.pos.y++;
        break;
    case KEY_LEFT:
        if (pl.pos.x > 0)
            pl.pos.x--;
        break;
    case KEY_RIGHT:
        if (pl.pos.x < mapMaxSizeX - 1)
            pl.pos.x++;
        break;
    case 'e':
      SpawnEnemy(en);
      break;
    default:
        break;
    }
}

int main()
{
    running = true;
    CoreInit();
    MapInit();

    while (running)
    {
        int ch = getch();
        if (game_state == GAME)
        {
            GameInputs(ch);
            UpdateEnemies();
            GameLoop();
        }
        else if (game_state == MENU)
        {
            DrawMenu(Menu_Selected);
            MenuInputs(ch, &Menu_Selected);
        }
        else if (game_state == SETT)
        {
            // Settings logic
        }
        else if (game_state == CLASSPICK)
        {
            // Load logic
        }
        if (ch == 'q')
        {
            running = false;
        }
    }

    endwin();
    return 0;
}

