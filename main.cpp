#include "controller/GameManager.hpp"
#include <ncurses.h>

int main(int argc, char **argv) {
  initscr();
  refresh();

  noecho();
  start_color();

  curs_set(0);

  GameManager game(0);

  while (!game.isOver()) {
    // 1 : User input
    game.processInput();

    // 2 : update game state
    game.updateState();

    // 3 : redraw display
    game.redraw();

    // 4 : go to 1, unless game over
  }

  getch();
  endwin();

  return 0;
}