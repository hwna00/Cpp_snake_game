#pragma once
#include <ncurses.h>
#include "../view/Drawable.hpp"

class PoisonItem : public Drawable {
public:
    PoisonItem(int row, int col) {
        this->row = row;
        this->col = col;
        this->icon = 'P';
    }
};