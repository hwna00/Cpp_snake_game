#pragma once
#include <ncurses.h>
#include "../view/Drawable.hpp"

class Gate : public Drawable {
public:
    Gate(int row, int col) {
        this->row = row;
        this->col = col;
        this->icon = 'G';
    }
};