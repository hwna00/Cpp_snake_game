#pragma once
#include <ncurses.h>
// for random
#include <stdlib.h>
#include <time.h>

#include "../model/Empty.hpp"
#include "../model/GrowthItem.hpp"
#include "../model/Snake.hpp"
#include "../view/Board.hpp"
#include "../view/Drawable.hpp"

class GameManager {
public:
  GameManager(int height, int width) {
    board = Board(height, width);
    initailize();
  }

  ~GameManager() { delete growthitem; }

  void initailize() {
    growthitem = NULL;
    board.initalize();
    game_over = false;
    srand(time(NULL));

    snake.setDirection(down);
    SnakePiece next = SnakePiece(1, 2);
    board.add(next);
    snake.addPiece(next);

    next = snake.nextHead();
    board.add(next);
    snake.addPiece(next);

    next = snake.nextHead();
    board.add(next);
    snake.addPiece(next);

    snake.setDirection(right);
    next = snake.nextHead();
    board.add(next);
    snake.addPiece(next);
  }

  void processInput() {
    chtype input = board.getInput();
    // process input
  }

  void updateState() {
    // update status
    if (growthitem == NULL) {
      int y, x;
      board.getEmptyCoordinates(y, x);
      growthitem = new GrowthItem(y * 2.5, x);
      board.add(*growthitem);
      growthitem = NULL;
    }

    SnakePiece next = snake.nextHead();
    if (next.getX() != growthitem->getX() &&
        next.getY() != growthitem->getY()) {
      int emptyRow = snake.tail().getY();
      int emptyCol = snake.tail().getX();
      board.add(Empty(emptyRow, emptyCol));
      snake.removePiece();
    }

    board.add(next);
    snake.addPiece(next);
  }

  void redraw() { board.refresh(); }

  bool isOver() { return game_over; }

private:
  Board board;
  bool game_over;
  GrowthItem *growthitem;
  Snake snake;
};