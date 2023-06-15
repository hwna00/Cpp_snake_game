#pragma once
#include <ncurses.h>
// for random
#include <time.h>
#include <stdlib.h>

#include "../view/Board.hpp"
#include "../view/Drawable.hpp"
#include "../model/GrowthItem.hpp"
#include "../model/Empty.hpp"

class GameManager {
public:
    GameManager(int height, int width) {
        board = Board(height, width);
        board.initalize();
        game_over = false;
        srand(time(NULL));
    }

    ~GameManager() {delete growthitem;}

    void processInput() {
        chtype input = board.getInput();
        // process input
    }

    void updateState() {
        // update status
        int y, x;
        board.getEmptyCoordinates(y, x);
        growthitem = new GrowthItem(y*2.5, x);
        if (growthitem != NULL) board.add(Empty(growthitem->getY(), growthitem->getX()));
        board.add(*growthitem);
    }

    void redraw() {
        board.refresh();
    }

    bool isOver() {
        return game_over;
    }
private:
    Board board;
    bool game_over;
    GrowthItem *growthitem;
};