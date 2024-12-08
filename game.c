
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

enum State { MENU, GAME, SETT };
enum logStatus { MESSAGE, ERROR };

struct Player {
  float health;
  int posX;
  int posY;
};

typedef struct Vector2 {
  int x;
  int y;
} Vec2;

static struct Player pl = {10.0f, 10, 10};
static enum State st = MENU;

static bool running;

// pre decleration
void init();

////////////////////////////////////////////////////////////
// recreate Logging so it makes bordered text on the screen

/*
void logging(char *text, enum logStatus status) {
  printw(stringFromLog(status), "::", text);
}
*/
////////////////////////////////////////////////////////////

/*
void waitForDuration(int milliseconds) {
  int elapsed = 0;
  while (elapsed < milliseconds) {
    napms(100); // Sleep for 100 milliseconds
    elapsed += 100;
    int ch = getch(); // Check for user input
    if (ch != ERR) {
      break; // Exit early if user input is detected
    }
  }
}
*/

int arr_length(int arr[]) {
  int i;
  int count = 0;
  for (i = 0; arr[i] != '\0'; i++) {
    count++;
  }
  return count;
}

void init() {
  initscr();
  cbreak();
  start_color();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);
  timeout(100);

  init_pair(1, COLOR_RED, COLOR_BLACK);  // red text , black bg
  init_pair(2, COLOR_BLUE, COLOR_BLACK); // if I want to add another one

  if (!has_colors()) {
    endwin();
    printf("Your terminal does not support colors.\n");
  }
}

void drawMenuBorder() {
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
}

// give cords and set border
void setBorder(Vec2 pos, Vec2 size) {
  char HorizontalLine = '-';
  char VerticalLine = '|';
  char empty = ' ';
  char edge = '#';

  int x1 = pos.x;
  int y1 = pos.y;
  int width = size.x;
  int height = size.y;

  char border[height][width];

  for (int y2 = 0; y2 < height; y2++) {
    for (int x2 = 0; x2 < width; x2++) {
      // fill full
      border[y2][x2] = empty;

      // left and right
      border[y2][0] = VerticalLine;
      border[y2][width - 1] = VerticalLine;

      // bottom and up
      border[0][x2] = HorizontalLine;
      border[height - 1][x2] = HorizontalLine;

      // edges
      border[0][0] = edge;
      border[height - 1][0] = edge;
      border[0][width - 1] = edge;
      border[height - 1][width - 1] = edge;
      mvaddch(y1, x1, border[y2][x2]);
    }
  }
}

void waitForDuration(double time) {
  if (time < 0) {
    return;
  }
  sleep(time * 1000000);
}

void fakeLoad() {
  char side1 = '[';
  char side2 = ']';
  char empty = '.';
  char full = '#';

  int barY = LINES / 2;
  int barLenght = 20;
  int barX = 20;

  char bar[barLenght];

  int progress = 0;

  for (int x = 0; x < barLenght; x++) {
    progress++;
    bar[progress] = full;
    bar[progress - barLenght] = empty;
    bar[0] = side1;
    bar[barLenght - 1] = side2;

    waitForDuration(1);
    mvaddch(barY, x - barX, bar[x]);
  }
}

char StartButton[3][6] = {{'-', '-', '-', '-', '-', '-'},
                          {'|', 'p', 'l', 'a', 'y', '|'},
                          {'-', '-', '-', '-', '-', '-'}};

char SettingsButton[3][6] = {{'-', '-', '-', '-', '-', '-'},
                             {'|', 's', 'e', 't', 't', '|'},
                             {'-', '-', '-', '-', '-', '-'}};

char ExitButton[3][6] = {{'-', '-', '-', '-', '-', '-'},
                         {'|', ' ', 'e', 'n', 'd', '|'},
                         {'-', '-', '-', '-', '-', '-'}};

// void wait(int sec) { sleep(sec * 1000000); }

void menuInputs(int input, int *selected) {
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
      st = GAME;
      break;
    case 1:
      printw("sett");
      st = SETT;
      break;
    case 2:
      printw("exit");
      running = false;
      break;
    }
    break;
  }
}

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

void drawAsciiRAT(int start_y, int start_x) {
  int y = start_y;
  int x = start_x;
  int i = 0;

  // Iterate through the ASCII art and display it with ncurses
  while (ascii_art[i] != '\0') {
    if (ascii_art[i] == '\n') { // Move to the next line
      y++;
      x = start_x; // Reset x to the starting column
    } else {
      mvaddch(y, x, ascii_art[i]); // Print character at the specified location
      x++;
    }
    i++;
  }
}

const char *logo = "               __                  \n"
                   "____________ _/  |______    ____   \n"
                   "\\_  __ \\__  \\\\   __\\__  \\  /    \\  \n"
                   " |  | \\\\/ __ \\|  |  / __ \\|   |  \\ \n"
                   " |__|  (____  /__| (____  /___|  / \n"
                   "            \\/          \\/     \\/  \n";

void drawLogo(int start_y, int start_x) {
  int y = start_y;
  int x = start_x;
  int i = 0;

  while (logo[i] != '\0') {
    if (logo[i] == '\n') {
      y++;
      x = start_x;
    } else {
      mvaddch(y, x, logo[i]);
      x++;
    }
    i++;
  }
}

void drawMenu(int selected) {
  refresh();
  int startRow = 10;
  int startCol = 20;
  int settRow = 15;
  int settCol = 25;
  int endRow = 20;
  int endCol = 30;

  // box(stdscr, 0, 0);
  drawMenuBorder();
  drawAsciiRAT(15, 50);
  drawLogo(25, 55);
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

char SettExitButton[3][6] = {{'-', '-', '-', '-', '-', '-'},
                             {'|', ' ', 'e', 'n', 'd', '|'},
                             {'-', '-', '-', '-', '-', '-'}};

char SaveButton[3][6] = {{'-', '-', '-', '-', '-', '-'},
                         {'|', 's', 'a', 'v', 'e', '|'},
                         {'-', '-', '-', '-', '-', '-'}};

char ToggleFullScreenButton[3][6] = {{'-', '-', '-', '-', '-', '-'},
                                     {'|', 'f', 'l', 'e', 'x', '|'},
                                     {'-', '-', '-', '-', '-', '-'}};

void drawSett(int selected) {
  refresh();
  int exitRow = 10;
  int exitCol = 10;
  int saveRow = 15;
  int saveCol = 15;
  int tfsRow = 20;
  int tfsCol = 20;

  drawMenuBorder();
  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 6; x++) {
      if (selected == 0) {
        attron(COLOR_PAIR(1));
      }
      mvaddch(exitRow + y, exitCol + x, SettExitButton[y][x]);
      attroff(COLOR_PAIR(1));
      if (selected == 1) {
        attron(COLOR_PAIR(1));
      }
      mvaddch(saveRow + y, saveCol + x, SaveButton[y][x]);
      attroff(COLOR_PAIR(1));
      if (selected == 2) {
        attron(COLOR_PAIR(1));
      }
      mvaddch(tfsRow + y, tfsCol + x, ToggleFullScreenButton[y][x]);
      attroff(COLOR_PAIR(1));
    }
  }
}

void SettInputs(int input, int *selected) {
  switch (input) {
  case KEY_UP:
    if (*selected < 0) {
      (*selected)--;
    }
    break;
  case KEY_DOWN:
    if (*selected < 2) {
      (*selected)++;
    }
    break;
  case '\n':
    switch (*selected)
    {
      case 0:
        printw("exit");
        break;
      case 1:
        printw("save");
        break;
      case 2:
        printw("ToggleFullScreenButton");
        break;
    }
    break;
  }
}

void move_character(int input) {
  switch (input) {
  case KEY_UP:
    pl.posY--;
    break;
  case KEY_DOWN:
    pl.posY++;
    break;
  case KEY_LEFT:
    pl.posX--;
    break;
  case KEY_RIGHT:
    pl.posX++;
    break;
  default:
    break;
  }
}

/*
void detectMouseInput(int ch)
{
  MEVENT event;

  ch = getch();
  if(ch == KEY_MOUSE)
      if(getmouse(&event) == OK)
      {

      }
}
*/

char health[3][6] = {{'-', '-', '-', '-', '-', '-'},
                     {'|', 'h', 'p', ':', ' ', '|'}, // [2][4]for hp value
                     {'-', '-', '-', '-', '-', '-'}};

int getSizefromPoints(int start, int end) { return start - end; }

void drawInfoPanel(Vec2 pos, Vec2 size) {
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

#define mapSizeX 30
#define mapSizeY 30
#define maxRooms 10
#define minRoomSize 3
#define maxRoomSize 8

//  scalable room generation for custom rooms
/*char **generateRoomSizable(Vec2 size) {
  int x = size.x;
  int y = size.y;
  char room[y][x];

  char HorizontalLine = '-';
  char VerticalLine = '|';
  char empty = '.';

  for (int x2 = 0; x2 < x; x2++) {
    for (int y2 = 0; y2 < y; y2++) {
      // fill
      room[y2][x2] = empty;

      // vertical
      room[y2][0] = VerticalLine;
      room[y2][y - 1] = VerticalLine;

      // horizontal
      room[0][x2] = HorizontalLine;
      room[x - 1][x2] = HorizontalLine;
    }
  }
  return room;
}
*/

/*
char smallRoom[5][5] =
{
  {'-','-','-','-','-'},
  {'|',' ',' ',' ','|'},
  {'|',' ',' ',' ','|'},
  {'|',' ',' ',' ','|'},
  {'-','-','-','-','-'}
};
*/

// most important step
/*int **generateMap(int SizeX, int SizeY) {
  int x = SizeX;
  int y = SizeY;
  int **map[y][x] = (int **)malloc(SizeY * sizeof(int *));

  for (int i = 0; i < y; i++) {
    for (int j = 0; j < x; j++) {
      map[i][j] = 0;
    }
  }

  return map;
}
*/

static int map[20][20];

int getTile(int **map, Vec2 pos) { return map[pos.x][pos.y]; }

/*
bool calculateRoomSpace(Vec2 pos, Vec2 size) {
  if (pos.x + size.x >= mapSizeX || pos.y + size.y >= mapSizeY) {
    return false;
  }

  for (int y = pos.y; y < size.y + pos.y; y++) {
    for (int x = pos.x; x < size.x + pos.x; x++) {
      if (map[y][x] != ' ') {
        return false;
      }
    }
  }
  return false;
}

void placeRoom(Vec2 pos, Vec2 size) {
  for (int y = pos.y; y < pos.y + size.y; y++) {
    for (int x = pos.x; x < pos.x + size.x; x++) {
      if (y == pos.y || y == pos.y + size.y - 1 || x == pos.x ||
          x == pos.x + size.x - 1) {
        map[y][x] = '#'; // Walls of the room
      } else {
        map[y][x] = '.'; // Interior (empty space)
      }
    }
  }
}

void generateRandomRooms() {
  int roomCount = 0;

  while (roomCount < maxRooms) {
    Vec2 roomSize = {rand() % (maxRoomSize - minRoomSize + 1) + minRoomSize,
                     rand() % (maxRoomSize - minRoomSize + 1) + minRoomSize};

    Vec2 roomPos = {rand() % (mapSizeX - roomSize.x),
                    rand() % (mapSizeY - roomSize.y)};

    if (calculateRoomSpace(roomPos, roomSize)) {
      placeRoom(roomPos, roomSize);
      roomCount++;
    }
  }
}

void connectRooms(Vec2 room1, Vec2 room2) {
  Vec2 start = room1;
  Vec2 end = room2;

  while (start.x != end.x) {
    map[start.y][start.x] = 2; // Path
    start.x += (start.x < end.x) ? 1 : -1;
  }

  while (start.y != end.y) {
    map[start.y][start.x] = 2; // Path
    start.y += (start.y < end.y) ? 1 : -1;
  }
}

// Generate paths between the rooms.
void connectAllRooms(Vec2 *roomCenters, int roomCount) {
  for (int i = 0; i < roomCount - 1; i++) {
    connectRooms(roomCenters[i], roomCenters[i + 1]);
  }
}
*/

void generateMap() {
  for (int y = 0; y < mapSizeY; y++) {
    for (int x = 0; x < mapSizeX; x++) {
      map[y][x] = 0;
    }
    map[1][2] = 1;
  }
}

void drawMap() {
  for (int y = 0; y < mapSizeY; y++) {
    for (int x = 0; x < mapSizeX; x++) {
      if (map[y][x] == 1) {
        mvaddch(y, x, '#'); // Draw walls
      } else if (map[y][x] == 2) {
        mvaddch(y, x, '.'); // Draw empty room spaces
      } else {
        mvaddch(y, x, ' '); // Draw empty map spaces
      }
    }
  }
}

void GameLoop() {
  drawMenuBorder();
  drawInfoPanel((Vec2){30, 30}, (Vec2){30, 30});
  mvprintw(pl.posY, pl.posX, "@");
  refresh();
}

void SettingsLoop(int selected) {
  drawMenuBorder();
  drawSett(selected);
  refresh();
}

int main() {
  running = true;
  int ch;

  int selected = 0;

  init();

  st = GAME;

  while (running) {
    if (st == MENU) {
      menuInputs(ch, &selected);
      drawMenu(selected);
    }
    if (st == GAME) {
      move_character(ch);
      GameLoop();
    }
    if (st == SETT) {
      SettInputs(ch, &selected);
      SettingsLoop(selected);
    }
    if ((ch = getch()) == 'q')
      running = false;
  }
  endwin();
  return 0;
}
