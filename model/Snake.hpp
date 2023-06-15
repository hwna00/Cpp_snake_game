#pragma once
#include "../view/Drawable.hpp"
#include <ncurses.h>
#include <queue>

enum Direction { up, down, left, right };

class SnakePiece : public Drawable {

public:
  SnakePiece() {
    this->x = this->y = 0;
    this->icon = '#';
  };

  SnakePiece(int y, int x) {
    this->x = x;
    this->y = y;
    this->icon = '#';
  };
};

class Snake {
  std::queue<SnakePiece> prev_pieces;
  Direction cur_direction;

public:
  Snake() { cur_direction = down; }
  void addPiece(SnakePiece piece);
  void removePiece();
  SnakePiece tail();
  SnakePiece head();
  Direction getDirection() { return cur_direction; }
  void setDirection(Direction d) { cur_direction = d; }
  SnakePiece nextHead() {
    int row = head().getY();
    int col = head().getX();

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
  };
};

void Snake::addPiece(SnakePiece piece) { prev_pieces.push(piece); }
void Snake::removePiece() { prev_pieces.pop(); }
SnakePiece Snake::tail() { return prev_pieces.front(); }
SnakePiece Snake::head() { return prev_pieces.back(); }
