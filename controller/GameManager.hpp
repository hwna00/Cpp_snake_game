#pragma once

#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "../model/Border.hpp"
#include "../model/Empty.hpp"
#include "../model/Gate.hpp"
#include "../model/Item.hpp"
#include "../model/Snake.hpp"
#include "../view/Board.hpp"
#include "../view/Drawable.hpp"
#include "../view/MissionBoard.hpp"
#include "../view/ScoreBoard.hpp"

class GameManager {
private:
  Board board;
  ScoreBoard scoreboard;
  MissionBoard missionboard;
  bool game_over;
  Snake snake;
  int bLimit, gLimit, pLimit, gateLimit, gCount, pCount, gateCount, gateCnt;
  clock_t starttimer, currenttimer; // 타이머
  Gate firstGate, secondGate;
  bool isGateOpen, isEnterGate, isMb, isMgi, isMpi, isMgate;

public:
  GameManager(int level) {
    board = Board(level);
    scoreboard = ScoreBoard();
    bLimit = board.getBCnt();
    gLimit = board.getGrowthCnt();
    pLimit = board.getPoisonCnt();
    gateLimit = board.getGateCnt();
    missionboard = MissionBoard("20", std::to_string(gLimit).c_str(),
                                std::to_string(pLimit).c_str(), "2");
    gCount = pCount = gateCount = gateCnt = 0;
    isMb = isMgi = isMpi = isMgate = FALSE;
    initailize();
  }

  void initailize() {
    board.initialize();
    game_over = false;
    isGateOpen = isEnterGate = false;
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
    try {
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
    } catch (int e) {
      game_over = true;
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

    if (isGateOpen && isEnterGate && gateCnt >= 0) {
      gateCnt--;
    }
    if (isGateOpen && isEnterGate && gateCnt == 0) {
      isGateOpen = false;
      isEnterGate = false;
      Border Wall1(firstGate.getRow(), firstGate.getCol());
      Border Wall2(secondGate.getRow(), secondGate.getCol());
      board.add(Wall1);
      board.add(Wall2);
    }

    if (snake.getBodyLength() >= 4 && !isGateOpen) {
      isGateOpen = true;
      //* snake 몸체의 길이 + 5 만큼 움직일 동안 게이트를 오픈한다.
      gateCnt = snake.getBodyLength() + 5;

      int firstGateRow, firstGateCol;
      int secondGateRow, secondGateCol;
      board.getNormalWallCoordinates(firstGateRow, firstGateCol);
      board.getNormalWallCoordinates(secondGateRow, secondGateCol);
      firstGate = Gate(firstGateRow, firstGateCol);
      secondGate = Gate(secondGateRow, secondGateCol);
      board.add(firstGate);
      board.add(secondGate);
    }
    checkMission();
    scoreboard.loadScore(std::to_string(snake.getBodyLength()).c_str(),
                         std::to_string(gCount).c_str(),
                         std::to_string(pCount).c_str(),
                         std::to_string(gateCount).c_str());
    missionboard.loadMission(isMb, isMgi, isMpi, isMgate);
  }

  void redraw() {
    board.refresh();
    scoreboard.refrash();
    missionboard.refrash();
  }

  bool isOver() { return game_over; }

private:
  void checkMission() {
    if (snake.getBodyLength() > bLimit - 1)
      isMb = true;
    if (gCount > gLimit - 1)
      isMgi = true;
    if (pCount > pLimit - 1)
      isMpi = true;
    if (gateCount > gateLimit - 1)
      isMgate = true;
  }

  void resetTimmer() { starttimer = time(NULL); }
  bool checkTimmer(int sec) {
    currenttimer = time(NULL);
    if ((currenttimer - starttimer) > sec) {
      starttimer = currenttimer;
      return true;
    }
    return false;
  }

  void initSnake(SnakePiece next) { // init 단계에서 snake를 추가하는 함수
    board.add(next);
    snake.addPiece(next);
  }

  void
  handleNextPiece(SnakePiece next) { // 진행 단계에서 snake를 이동시키는 함수
    switch (board.getChatAt(next.getRow(), next.getCol())) {
    case 'A': //* 성장 아이템을 먹었을 때
      gCount++;
      destroyItem('A');
      break;
    case 'P': //* 독 아이템을 먹었을 때
    {
      pCount++;
      insertEmpty();
      insertEmpty();
      destroyItem('P');
      break;
    }
    case 'G': //* 게이트를 지나는 경우
    {
      gateCount++;
      isEnterGate = true;
      int nextRow;
      int nextCol;
      if (firstGate.getRow() == next.getRow() &&
          firstGate.getCol() == next.getCol()) {
        //* firstGate로 들어온 상황
        nextRow = secondGate.getRow();
        nextCol = secondGate.getCol();
      } else {
        nextRow = firstGate.getRow();
        nextCol = firstGate.getCol();
      }

      //! 이슈: firstGate와 secondGate가 같은 벽에 있다면 에러가 발생한다.
      //* 출구 게이트가 벽의 가장자리에 있는 경우
      if (nextRow == 0) {
        // 상단 게이트에서 아래로 내려오는 경우
        snake.setDirection(down);
        next.setCoordinates(nextRow + 1, nextCol);
      } else if (nextRow == 31) {
        // 하단 게이트에서 위로 올라오는 경우
        snake.setDirection(up);
        next.setCoordinates(nextRow - 1, nextCol);
      } else if (nextCol == 1) {
        // 좌측 게이트에서 나와 우측으로 이동하는 경우
        snake.setDirection(right);
        next.setCoordinates(nextRow, nextCol + 1);
      } else if (nextCol == 59) {
        // 우측 게이트에서 나와 좌측으로 이동하는 경우
        snake.setDirection(left);
        next.setCoordinates(nextRow, nextCol - 1);
      }

      // Todo: 출구 게이트가 중간 벽에 있는 경우
      // ...

      insertEmpty();
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
    while (gc < 3) {
      int y, x;
      board.getEmptyCoordinates(y, x);
      board.add(GrowthItem(y, x));
      gc++;
    }
    while (pc < 3) {
      int y, x;
      board.getEmptyCoordinates(y, x);
      board.add(PoisonItem(y, x));
      pc++;
    }
  }

  void destroyItem(char itemType) {
    switch (itemType) {
    case 'A':
      break;
    case 'P':
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
