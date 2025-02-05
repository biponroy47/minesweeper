#ifndef CELL_H
#define CELL_H

class Cell {

private:
  bool revealed;
  int type;

public:
  Cell();
  bool get_revealed();
  void set_revealed(bool new_reveal);
  int get_type();
  void set_type(int new_type);
  bool is_bomb();
};

#endif