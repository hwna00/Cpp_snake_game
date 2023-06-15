#pragma once
#include <ncurses.h>
#include "../view/Drawable.hpp"

class Border : public Drawable {
public:
    Border(int y, int x) {
        this->y = y;
        this->x = x;
        this->icon = 'X';
    }
};