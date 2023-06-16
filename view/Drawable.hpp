#pragma once
#include <ncurses.h>

class Drawable {
protected:
  int row, col;
  chtype icon;
public:
  Drawable(int row = 0, int col = 0, chtype icon = 'X') : row(row), col(col), icon(icon) {}

  int getRow() {return row;}

  int getCol() {return col;}

  chtype getIcon() {return icon;}
};