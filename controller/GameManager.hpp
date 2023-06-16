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
    gLimit = board.getGrowthCnt(); pLimit = board.getPoisonCnt();
    gCount = pCount = 0;
    initailize();
  }

  void initailize() {
    board.initialize();
    game_over = false;
    srand(time(NULL));
    snake.setDirection(down);
    initSnake(SnakePiece(1, 2));
    initSnake(SnakePiece(snake.nextHead()));
    initSnake(SnakePiece(snake.nextHead()));
    snake.setDirection(right);
    initSnake(SnakePiece(snake.nextHead()));
  
    createItem();
    resetTimmer();
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

    if (checkTimmer(30)) { // 30초 지날 때 마다 item reset 구현
      resetItem();
      createItem();
    }

    if (snake.getBodyLength() <= 3) {
      game_over = true;
    }
  }

  void redraw() { board.refrash(); }

  bool isOver() { return game_over; }

private:
  Board board;
  bool game_over;
  Snake snake;
  int gLimit, pLimit, gCount, pCount;
  clock_t starttimer, currenttimer; // 타이머

  void resetTimmer() { starttimer = time(NULL); }
  bool checkTimmer(int sec) {
    currenttimer = time(NULL);
    if((currenttimer - starttimer) > sec) {
      starttimer = currenttimer;
      return true;
    }
    return false;
  }

  void initSnake(SnakePiece next) { // init 단계에서 snake를 추가하는 함수
    board.add(next);
    snake.addPiece(next);
  }

  void handleNextPiece(SnakePiece next) { // 진행 단계에서 snake를 이동시키는 함수
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

    board.add(next);
    snake.addPiece(next);
  }

  void resetItem() {
    int row = board.getRow(), col = board.getCol();
    for (int i = 0; i < row; i++)
      for (int j = 0; j < col; j++)
        if (board.getChatAt(i, j) == 'A' || board.getChatAt(i, j) == 'P')
          board.add(Empty(i, j));
  }

  void createItem() {
    int gc = 0, pc = 0;
    while (gc < gLimit) {
      int y, x;
      board.getEmptyCoordinates(y, x);
      board.add(GrowthItem(y, x));
      gc++;
    }
    while (pc < pLimit) {
      int y, x;
      board.getEmptyCoordinates(y, x);
      board.add(PoisonItem(y, x));
      pc++;
    }
  }

  void destroyItem(char itemType) {
    switch (itemType) {
    case 'A':
      gCount--;
      break;
    case 'P':
      pCount--;
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
