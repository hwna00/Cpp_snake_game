#include "../view/Board.hpp"
#pragma once
#include <ncurses.h>

class GameManager {
private:
  Board board;
  bool game_over;

public:
  GameManager(int height, int width);

  void processInput() {
    chtype input = board.getInput(); // wgetch();
    // process input
  }

  void updateState() {
    // update state
  }

  void redraw() { board.refresh(); }
  bool isOver() { return game_over; }
};

GameManager::GameManager(int height, int width) {
  //
  board = Board(height, width);
  board.initalize();
  game_over = false;
}
