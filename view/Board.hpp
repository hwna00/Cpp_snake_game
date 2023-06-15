#pragma once

#include "Drawable.hpp"
#include "../model/Border.hpp"
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
    init_pair(5, 5, 5); // 지렁이 (브라운, 브라운)
  }

  Board(int height, int width) { construct(height, width); }

  void initalize() {
    clear();
    refresh();
  }

  void addBlank() {
    for (int i = 0; i < height; i++)
      for (int j = 0; j < width * 2.5; j++)
        add(Empty(i, j));
  }

  void addBoarder() { 
    box(board_win, 0, 0);
    wattron(board_win, COLOR_PAIR(4));
    for (int i = 0; i < height; i++) {
      add(Border(i, 0));
      add(Border(i, 1));
      add(Border(i, width-2));
      add(Border(i, width-1));
    }
    for (int i = 0; i < width * 2.5; i++) {
      add(Border(0, i));
      add(Border(height-1, i));
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
      case 'E':
        wattron(board_win, COLOR_PAIR(1));
        mvwprintw(board_win, y, x, " ");
        wattroff(board_win, COLOR_PAIR(1));
        break;
      case '#':
        wattron(board_win, COLOR_PAIR(5));
        mvwprintw(board_win, y, x, "#");
        wattroff(board_win, COLOR_PAIR(5));
        break;
      case 'A':
        wattron(board_win, COLOR_PAIR(2));
        mvwprintw(board_win, y, x, "A");
        wattroff(board_win, COLOR_PAIR(2));
        break;
      default:
        mvwaddch(board_win, y, x, ch);
    }
  }

  chtype getInput() { return wgetch(board_win); }

  void getEmptyCoordinates(int &y, int &x) {
    while ((mvwinch(board_win, y = rand() % height, x = rand() % width)) != ' ')
      ;
  }

  void clear() {
    wclear(board_win);
    addBlank();
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