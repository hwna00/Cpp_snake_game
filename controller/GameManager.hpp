#pragma once

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "../model/Empty.hpp"
#include "../model/Gate.hpp"
#include "../model/Item.hpp"
#include "../model/Snake.hpp"
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

  ~GameManager() {
    delete growthitem;
    delete poisonitem;
  }

  void initailize() {
    board.initialize();
    game_over = false;
    isGateOpen = false;
    srand(time(NULL));

    snake.setDirection(down);
    handleNextPiece(SnakePiece(1, 2));
    handleNextPiece(SnakePiece(snake.nextHead()));
    handleNextPiece(SnakePiece(snake.nextHead()));
    snake.setDirection(right);
    handleNextPiece(SnakePiece(snake.nextHead()));

    if (growthitem == NULL)
      createGrowthItem();
    if (poisonitem == NULL)
      createPoisonItem();
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

    if (growthitem == NULL)
      createGrowthItem();
    if (poisonitem == NULL)
      createPoisonItem();

    if (snake.getBodyLength() <= 3) {
      game_over = true;
    }

    if (snake.getBodyLength() >= 5 && !isGateOpen) {
      isGateOpen = true;

      int inGateRow, inGateCol;
      int outGateRow, outGateCol;
      board.getNormalWallCoordinates(inGateRow, inGateCol);
      board.getNormalWallCoordinates(outGateRow, outGateCol);
      inGate = Gate(inGateRow, inGateCol);
      outGate = Gate(outGateRow, outGateCol);
      board.add(inGate);
      board.add(outGate);
    }
  }

  void redraw() { board.refrash(); }

  bool isOver() { return game_over; }

private:
  Board board;
  bool game_over;
  GrowthItem *growthitem;
  PoisonItem *poisonitem;
  Snake snake;
  Gate inGate, outGate;
  bool isGateOpen;

  void handleNextPiece(SnakePiece next) {
    if (growthitem != NULL) {
      switch (board.getChatAt(next.getRow(), next.getCol())) {
      case 'A': //* 성장 아이템을 먹었을 때
        destroyItem('A');
        break;
      case 'P': //* 독 아이템을 먹었을 때
      {
        insertEmpty();
        insertEmpty();
        destroyItem('P');
        break;
      }
      case '0': //* 빈 공간을 지나갈 때
      {
        insertEmpty();
        break;
      }
      default: //* 빈 공간도 아니고,아이템도 아닌 지역 == 벽 or 스네이크 몸체
        game_over = true;
        break;
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

  void destroyItem(char itemType) {
    switch (itemType) {
    case 'A':
      delete growthitem;
      growthitem = NULL;
      break;
    case 'P':
      delete poisonitem;
      poisonitem = NULL;
      break;
    default:
      break;
    }
  }

  void insertEmpty() {
    int emptyRow = snake.tail().getRow();
    int emptyCol = snake.tail().getCol();
    board.add(Empty(emptyRow, emptyCol));
    snake.removePiece();
  }
};
