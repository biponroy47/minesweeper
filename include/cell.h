#ifndef CELL_H
#define CELL_H

class Cell {

private:
  bool revealed;
  int type;

public:
  Cell();
  bool getRevealed();
  void setRevealed(bool new_reveal);
  int getType();
  void set_type(int new_type);
  bool isBomb();
};

#endif