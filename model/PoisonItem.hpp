#pragma once
#include <ncurses.h>
#include "../view/Drawable.hpp"

class PoisonItem : public Drawable {
public:
    PoisonItem(int y, int x) {
        this->y = y;
        this->x = x;
        this->icon = 'P';
    }
};