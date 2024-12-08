#include <ncurses.h>

typedef struct Player {
  int posX;
  int posY;
} player;

// Function to initialize the game screen
void init_screen() {
  initscr();            // Start curses mode
  cbreak();             // Disable line buffering
  keypad(stdscr, TRUE); // Enable special keys like arrows
  noecho();             // Don't echo keypresses to the screen
  curs_set(0);          // Hide the cursor
  timeout(100);
}

// Function to handle the player's movement
void move_character(int input) {
  switch (input) {
  case KEY_UP:
    posY--; // Move up
    break;
  case KEY_DOWN:
    posY++; // Move down
    break;
  case KEY_LEFT:
    posX--; // Move left
    break;
  case KEY_RIGHT:
    posX++; // Move right
    break;
  default:
    break;
  }
}

// Function to draw the game screen
void draw_screen() {
  clear();                   // Clear the screen
  mvprintw(posY, posX, "@"); // Draw the character at the new position
  mvprintw(0, 0, "Use arrow keys to move '@'. Press 'q' to quit.");
  refresh(); // Refresh the screen to show changes
}

// Main function to run the game loop
int main() {
  int ch;
  init_screen();

  while ((ch = getch()) != 'q') { // Press 'q' to quit the game
    move_character(ch);           // Handle input and move character
    draw_screen();                // Redraw the screen
  }

  endwin(); // End curses mode
  return 0;
}
