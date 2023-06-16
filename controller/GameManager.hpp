#pragma once

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "../model/Item.hpp"
#include "../model/Snake.hpp"
#include "../model/Empty.hpp"
#include "../view/Board.hpp"
#include "../view/Drawable.hpp"

class GameManager {
public:
  GameManager(int level) {
    board = Board(level);
    growthitem = NULL;
    poisonitem = NULL;
    initailize();
  }

  ~GameManager() { delete growthitem; delete poisonitem; }

  void initailize() {
    board.initialize();
    game_over = false;
    srand(time(NULL));

    snake.setDirection(down);
    handleNextPiece(SnakePiece(1, 2));
    handleNextPiece(SnakePiece(snake.nextHead()));
    handleNextPiece(SnakePiece(snake.nextHead()));
    snake.setDirection(right);
    handleNextPiece(SnakePiece(snake.nextHead()));

    if (growthitem == NULL) createGrowthItem();
    if (poisonitem == NULL) createPoisonItem();
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

    if (growthitem == NULL) createGrowthItem();
    if (poisonitem == NULL) createPoisonItem();
  }

  void redraw() { board.refrash(); }

  bool isOver() { return game_over; }

private:
  Board board;
  bool game_over;
  GrowthItem *growthitem;
  PoisonItem *poisonitem;
  Snake snake;

  void handleNextPiece(SnakePiece next) {
    if (growthitem != NULL) {
      if (next.getCol() != growthitem->getCol() || next.getRow() != growthitem->getRow()) {
        int emptyRow = snake.tail().getRow();
        int emptyCol = snake.tail().getCol();
        board.add(Empty(emptyRow, emptyCol));
        snake.removePiece();
      }else {
        delete growthitem;
        growthitem = NULL;
      }
    }
    if (poisonitem != NULL) {
      if (next.getCol() == poisonitem->getCol() && next.getRow() == poisonitem->getRow()) {
        int emptyRow = snake.tail().getRow();
        int emptyCol = snake.tail().getCol();
        board.add(Empty(emptyRow, emptyCol));
        snake.removePiece();
        delete poisonitem;
        poisonitem = NULL;
      }
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

  void createPoisonItem() {
    if (poisonitem == NULL) {
        int y, x;
        board.getEmptyCoordinates(y, x);
        poisonitem = new PoisonItem(y, x);
        board.add(*poisonitem);
    }
  }
};
