
#include <ncurses.h>

#define mapPosY 30
#define mapPosX 30

#define mapSizeY 30
#define mapSizeX 30

enum State { GAME, MENU, SETT };

typedef struct Vector2 {
  int x, y;
} Vec2;

struct Player {
  int posX;
  int posY;
};

typedef struct Tile {
  Vec2 pos;
  Vec2 size;
} tile;

static int map[mapSizeY][mapSizeX];
static bool running;

// predecleration

static struct Player player = {10, 10};

// tools
static int getSizefromPoints(int start, int end);
static void check(bool is);

// init
static void Coreinit();
static bool Mapinit();

// drawing
static void DrawBorder(Vec2 pos, Vec2 size);
static void DrawGame();
static void InputsGame(int input);

void check(bool is) {
  if (!is) {
    endwin();
  }
}

bool Mapinit() {
  for (int y = 0; y < mapSizeY; y++) {
    for (int x = 0; x < mapSizeX; x++) {
      map[y][x] = 0;
      map[y][1] = 1;
    }
  }
  return true;
}

void Coreinit() {
  initscr();
  cbreak();
  start_color();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);
  timeout(100);

  init_pair(1, COLOR_RED,
            COLOR_BLACK); // red text , black bg for selected stuff
  init_pair(2, COLOR_BLUE, COLOR_BLACK); // if I want to add another one

  check(has_colors());
  check(Mapinit());
}

int getSizefromPoints(int start, int end) { return start - end; }

void DrawBorder(Vec2 pos, Vec2 size) {
  char HorizontalLine = '-';
  char VerticalLine = '|';
  char empty = ' ';
  char edge = '#';

  int width, height;
  width = getSizefromPoints(pos.x, size.x);
  height = getSizefromPoints(pos.y, size.y);

  char box[height][width];

  for (int y = pos.y; y < size.y; y++) {
    for (int x = pos.x; x < size.x; x++) {
      // full fill
      box[y][x] = empty;

      // vertical
      box[y][0] = VerticalLine;
      box[y][width] = VerticalLine;

      // horizontal
      box[0][x] = HorizontalLine;
      box[height][x] = HorizontalLine;

      // edges
      box[0][0] = edge;
      box[height - 1][0] = edge;
      box[0][width - 1] = edge;
      box[height - 1][width - 1] = edge;

      mvaddch(y, x, box[y][x]);
    }
  }
}

void DrawGame() {
  clear();
  DrawBorder((Vec2){0, 0}, (Vec2){30, 30});
  mvprintw(player.posY, player.posX, "@");

  for (int y = 0; y < mapSizeY; y++) {
    for (int x = 0; x < mapSizeX; x++) {
      map[]
    }
  }
  refresh();
}

void InputsGame(int input) {
  switch (input) {
  case KEY_UP:
    if (player.posY > 0)
      player.posY--;
    break;
  case KEY_DOWN:
    if (player.posY < mapSizeY - 1)
      player.posY++;
    break;
  case KEY_LEFT:
    if (player.posX > 0)
      player.posX--;
    break;
  case KEY_RIGHT:
    if (player.posX < mapSizeX - 1)
      player.posX++;
    break;
  default:
    break;
  }
}

int main() {
  running = true;
  Coreinit();
  while (running) {
    int ch = getch();
    if (ch == 'q') {
      running = false;
    }
    InputsGame(ch);
    DrawGame();
  }
  endwin();
  return 0;
}
