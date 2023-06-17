#pragma once

#include "Drawable.hpp"
#include "Map.hpp"
#include <iostream>
#include <ncurses.h>
class Board {
  Map m;
  int level, col, row;
  WINDOW *board_win;

public:
  Board(int level = 0) : level(level) {
    m = Map(level);
    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax);
    col = m.getCol(), row = m.getRow();
    board_win = newwin(row, col, 1, 3);
    setTimeout(500);
    keypad(board_win, true);
  }

  void initialize() {
    initColor();
    initBoarder();
    refresh();
  }

  chtype getInput() { return wgetch(board_win); }

  void refresh() { wrefresh(board_win); }

  void initColor() {
    init_pair(0, 1, 0); // Deafult (블랙, 블랙)
    init_pair(1, 1, 7); // 배경색 (블랙, 화이트)
    init_pair(2, 1, 2); // 사과 (그린, 그린)
    init_pair(3, 1, 4); // 독사과 (레드, 레드)
    init_pair(4, 1, 8); // 벽 (그레이, 그레이)
    init_pair(5, 1, 5); // 지렁이 (브라운, 브라운)
    init_pair(6, 1, 3); // 게이트 (시안, 시안)
  }

  void addAt(int row, int col, chtype icon, int n) {
    wattron(board_win, COLOR_PAIR(n));
    mvwaddch(board_win, row, col, icon);
    wattroff(board_win, COLOR_PAIR(n));
  }

  void add(Drawable drawable) {
    int n;
    switch (drawable.getIcon()) {
    // 벽
    case 'X':
    case '1':
      n = 4;
      break;
    // 공백
    case '0':
      n = 1;
      break;
    // 사과
    case 'A':
      n = 2;
      break;
    // 독사과
    case 'P':
      n = 3;
      break;
    // 뱀
    case '#':
      n = 5;
      break;
    // 게이트
    case 'G':
      n = 6;
      break;
    default:
      n = 0;
    }
    addAt(drawable.getRow(), drawable.getCol(), drawable.getIcon(), n);
    m.setData(drawable.getRow(), drawable.getCol(),
              drawable.getIcon()); // map Data 업데이트
    wrefresh(board_win);
  }

  void initBoarder() {
    wclear(board_win);

    for (int i = 0; i < row; i++)
      for (int j = 0; j < col; j++)
        add(Drawable(i, j, m.getData(i, j)));
  }

  void getEmptyCoordinates(int &r, int &c) {
    do {
      r = rand() % row;
      c = rand() % col;
    } while (m.getData(r, c) != '0');
  }

  void getNormalWallCoordinates(int &r, int &c) {
    do {
      r = rand() % row;
      c = rand() % col;
    } while (m.getData(r, c) != '1');
  }

  char getChatAt(int row, int col) { return m.getData(row, col); }

  void setTimeout(int timeout) { wtimeout(board_win, timeout); }

  int getBCnt() { return m.getBCnt(); }
  int getGrowthCnt() { return m.getGrowthCnt(); }
  int getPoisonCnt() { return m.getPoisonCnt(); }
  int getGateCnt() { return m.getGateCnt(); }

  int getCol() { return col; }
  int getRow() { return row; }
};