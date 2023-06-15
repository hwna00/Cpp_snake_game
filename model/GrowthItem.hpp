#pragma once
#include <ncurses.h>
#include "../view/Drawable.hpp"

class GrowthItem : public Drawable {
public:
    GrowthItem(int x, int y) {
        this->y = y;
        this->x = x;
        this->icon = 'A';
    }
};