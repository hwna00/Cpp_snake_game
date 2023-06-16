#pragma once
#include "../view/Drawable.hpp"
#include <ncurses.h>

class Gate : public Drawable {
public:
  Gate() {}
  Gate(int row, int col) {
    this->row = row;
    this->col = col;
    this->icon = 'G';
  }
};