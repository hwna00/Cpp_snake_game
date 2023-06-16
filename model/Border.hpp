#pragma once
#include <ncurses.h>
#include "../view/Drawable.hpp"

class Border : public Drawable {
public:
    Border(int row, int col) {
        this->row = row;
        this->col = col;
        this->icon = '1';
    }
};