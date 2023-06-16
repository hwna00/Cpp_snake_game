#pragma once
#include "../view/Drawable.hpp"
#include <ncurses.h>
#include <queue>

enum Direction { up = -1, down = 1, left = -2, right = 2 };

class SnakePiece : public Drawable {
public:
  SnakePiece(int row = 0, int col = 0) {
    this->row = row;
    this->col = col;
    this->icon = '#';
  }
  void setCoordinates(int row, int col) {
    this->row = row;
    this->col = col;
  }
};

class Snake {
  std::queue<SnakePiece> prev_pieces;
  Direction cur_direction;

public:
  Snake() { cur_direction = down; }
  void addPiece(SnakePiece piece) { prev_pieces.push(piece); }
  void removePiece() { prev_pieces.pop(); }

  int getBodyLength() { return prev_pieces.size(); }

  SnakePiece tail() { return prev_pieces.front(); }
  SnakePiece head() { return prev_pieces.back(); }
  Direction getDirection() { return cur_direction; }

  void setDirection(Direction d) {
    if (cur_direction + d != 0)
      cur_direction = d;
  }

  SnakePiece nextHead() {
    int row = head().getRow();
    int col = head().getCol();

    switch (cur_direction) {
    case down:
      row++;
      break;
    case up:
      row--;
      break;
    case left:
      col--;
      break;
    case right:
      col++;
      break;
    }

    return SnakePiece(row, col);
  }
};