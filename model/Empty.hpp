#pragma once
#include <ncurses.h>
#include "../view/Drawable.hpp"

class Empty : public Drawable {
public:
    Empty(int row, int col) {
        this->row = row;
        this->col = col;
        this->icon = '0';
    }
};