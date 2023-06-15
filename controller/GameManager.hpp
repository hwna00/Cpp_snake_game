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
    growthitem = NULL;
    initailize();
  }

  ~GameManager() { delete growthitem; }

  void initailize() {
    board.initalize();
    game_over = false;
    srand(time(NULL));

    snake.setDirection(down);
    handleNextPiece(SnakePiece(1, 2));
    handleNextPiece(SnakePiece(snake.nextHead()));
    handleNextPiece(SnakePiece(snake.nextHead()));
    snake.setDirection(right);
    handleNextPiece(SnakePiece(snake.nextHead()));

    if (growthitem == NULL) {
      createGrowthItem();
    }
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
    handleNextPiece(snake.nextHead());

    if (growthitem == NULL) {
      createGrowthItem();
    }
  }

  void redraw() { board.refresh(); }

  bool isOver() { return game_over; }

private:
  Board board;
  bool game_over;
  GrowthItem *growthitem;
  Snake snake;

  void handleNextPiece(SnakePiece next) {
    if (growthitem != NULL && (next.getX() != growthitem->getX() ||
                               next.getY() != growthitem->getY())) {
      int emptyRow = snake.tail().getY();
      int emptyCol = snake.tail().getX();
      board.add(Empty(emptyRow, emptyCol));
      snake.removePiece();
    } else {
      delete growthitem;
      growthitem = NULL;
    }
    board.add(next);
    snake.addPiece(next);
  }

  void createGrowthItem() {
    if (growthitem == NULL) {
      int y, x;
      board.getEmptyCoordinates(y, x);
      growthitem = new GrowthItem(y, x);
      board.add(*growthitem);
    }
  }
};
