#pragma once

#include "Drawable.hpp"
#include <ncurses.h>
#include <stdlib.h>

class Board {
public:
  Board() { construct(0, 0); }

  Board(int height, int width) { construct(height, width); }

  void initalize() {
    clear();
    refresh();
  }

  void addBoarder() { box(board_win, 0, 0); }

  void add(Drawable drawable) {
    addAt(drawable.getY(), drawable.getX(), drawable.getIcon());
  }

  void addAt(int y, int x, chtype ch) { mvwaddch(board_win, y, x, ch); }

  chtype getInput() { return wgetch(board_win); }

  void getEmptyCoordinates(int &y, int &x) {
    while ((mvwinch(board_win, y = rand() % height, x = rand() % width)) != ' ')
      ;
  }

  void clear() {
    wclear(board_win);
    addBoarder();
  }

  void refresh() { wrefresh(board_win); }

  void setTimeout(int timeout) { wtimeout(board_win, timeout); }

private:
  WINDOW *board_win;
  int height, width;

  void construct(int height, int width) {
    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax);
    this->height = height;
    this->width = width;

    board_win = newwin(height, width, (yMax / 2) - (height / 2),
                       (xMax / 2) - (width / 2));
    setTimeout(500); // 프레임 설정
    keypad(board_win, true);
  }
};