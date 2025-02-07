#ifndef CELLGRID_H
#define CELLGRID_H
#include "include/cell.h"
#include <vector>

class CellGrid {

private:
  Cell **cells;

public:
  CellGrid();
  int getRandomNumber(int min, int max);
  Cell &getCell(int row, int col);
  void placeBombs(int numBombs);
  void printCellGrid();
  bool hasBombsNearby(int row, int col);
  int countBombsNearby(int row, int col);
};

#endif