#include "../include/cellgrid.h"
#include <iomanip>
#include <iostream>
#include <random>

// initialize a CellGrid object composed of 16x30 Cell objects
CellGrid::CellGrid() {
  cells = new Cell *[16];
  for (int row = 0; row < 16; row++) {
    cells[row] = new Cell[30];
  }
}

Cell &CellGrid::getCell(int row, int col) { return cells[row][col]; }

// random number generator used to determine random indices for bomb placement
int CellGrid::getRandomNumber(int min, int max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(min, max);
  return dist(gen);
}

// populate grid wit bombs
void CellGrid::placeBombs(int numBombs) {
  int remaining = numBombs;
  while (remaining != 0) {
    int x = getRandomNumber(0, 15);
    int y = getRandomNumber(0, 29);
    if (!cells[x][y].isBomb()) {
      cells[x][y].set_type(1);
      remaining--;
    }
  }
}

// print current state of grid to console where 0 = no bomb, 1 = bomb
void CellGrid::printCellGrid() {
  std::cout << std::setw(3) << "";
  for (int col = 0; col < 30; col++) {
    std::cout << std::setw(2) << col << " ";
  }
  std::cout << std::endl;

  for (int row = 0; row < 16; row++) {
    std::cout << std::setw(2) << row << " ";
    for (int col = 0; col < 30; col++) {
      std::cout << std::setw(2) << cells[row][col].getType() << " ";
    }
    std::cout << std::endl;
  }
}

// determine if any neighbouring cells contain a bomb
bool CellGrid::hasBombsNearby(int x, int y) {
  // only check neighbours within grid dimmensions, and exclude own cell
  for (int row = x - 1; row <= x + 1; row++) {
    for (int col = y - 1; col <= y + 1; col++) {
      if (row >= 0 && row < 16 && col >= 0 && col < 30 &&
          !(row == x && col == y)) {
        if (cells[row][col].getType() == 1) {
          return true;
        }
      }
    }
  }
  return false;
}

// count nearby bombs given at least one bomb neighbours
int CellGrid::countBombsNearby(int x, int y) {
  int count = 0;

  for (int row = x - 1; row <= x + 1; row++) {
    for (int col = y - 1; col <= y + 1; col++) {
      if (row >= 0 && row < 16 && col >= 0 && col < 30 &&
          !(row == x && col == y)) {
        if (cells[row][col].getType() == 1) {
          count += 1;
        }
      }
    }
  }
  return count;
}

// delete cell objects
void CellGrid::resetCellGrid() {
  for (int row = 0; row < 16; row++) {
    delete[] cells[row];
  }
  delete[] cells;

  cells = new Cell *[16];
  for (int row = 0; row < 16; row++) {
    cells[row] = new Cell[30];
  }
}