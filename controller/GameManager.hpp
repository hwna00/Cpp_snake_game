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
    switch (input) {
    case 'w':
    case KEY_UP:
      snake.setDirection(up);
      break;
    case 'a':
    case KEY_LEFT:
      snake.setDirection(left);
      break;
    case 's':
    case KEY_DOWN:
      snake.setDirection(down);
      break;
    case 'd':
    case KEY_RIGHT:
      snake.setDirection(right);
      break;
    case 'p':
      board.setTimeout(-1); // -1은 유저가 다음 key를 입력할 때까지 blocking
      while (board.getInput() != 'p') {
      }
      board.setTimeout(500);
      break;

    default:
      break;
    }
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