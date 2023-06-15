#pragma once

#include "Drawable.hpp"
#include <ncurses.h>
#include <stdlib.h>

class Board {
public:
  Board() { 
    construct(0, 0); 
    // init pallet
    init_pair(1, 0, 7); // 배경색 (블랙, 화이트)
    init_pair(2, 2, 2); // 사과 (그린, 그린)
    init_pair(3, 4, 4); // 독사과 (레드, 레드)
    init_pair(4, 8, 8); // 벽 (그레이, 그레이)
  }

  Board(int height, int width) { construct(height, width); }

  void initalize() {
    clear();
    refresh();
  }

  void addBoarder() { 
    box(board_win, 0, 0);
    wbkgd(board_win, COLOR_PAIR(1));
    wattron(board_win, COLOR_PAIR(4));
    for (int i = 0; i < height; i++) {
      addAt(i, 0, 'h');
      addAt(i, width-2, 'h');
    }
    for (int i = 0; i < width * 2.5; i++) {
      addAt(0, i, 'r');
      addAt(height-1, i, 'r');
    }
    wattroff(board_win, COLOR_PAIR(4));
  }

  void add(Drawable drawable) {
    addAt(drawable.getY(), drawable.getX(), drawable.getIcon());
  }

  void addAt(int y, int x, chtype ch) {
    switch (ch) {
      case 'X':
        wattron(board_win, COLOR_PAIR(4));
        mvwprintw(board_win, y, x, "X");
        wattroff(board_win, COLOR_PAIR(4));
        break;
      default:
        mvwaddch(board_win, y, x, ch);
    }
  }

  chtype getInput() { return wgetch(board_win); }

  void getEmptyCoordinates(int &y, int &x) {
    while ((mvwinch(board_win, y = rand() % height, x = rand() % width)) != ' ');
  }

  void clear() {
    wclear(board_win);
    addBoarder();
  }

  void refresh() { wrefresh(board_win); }

private:
  WINDOW *board_win;
  int height, width;

  void construct(int height, int width) {
    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax);
    this->height = height;
    this->width = width;

    board_win = newwin(height, width, (yMax / 2) - (height / 2), (xMax / 2) - (width / 2));
    wtimeout(board_win, 500); // 프레임 설정
  }
};