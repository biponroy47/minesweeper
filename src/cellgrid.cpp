#include "../include/cellgrid.h"
#include <iomanip>
#include <iostream>
#include <random>

CellGrid::CellGrid() {
  cells = new Cell *[16];
  for (int row = 0; row < 16; row++) {
    cells[row] = new Cell[30];
  }
  // std::cout << "CellGrid initialized." << std::endl;
}

Cell &CellGrid::getCell(int row, int col) {
  // std::cout << "cell (" << row << ", " << col << ") : selected" << std::endl;
  return cells[row][col];
}

int CellGrid::getRandomNumber(int min, int max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(min, max);
  return dist(gen);
}

void CellGrid::placeBombs(int numBombs) {
  int remaining = numBombs;
  while (remaining != 0) {
    int x = getRandomNumber(0, 15);
    int y = getRandomNumber(0, 29);
    // std::cout << "cell (" << x << ", " << y << ") : trying to place bomb"
    //           << std::endl;
    if (!cells[x][y].is_bomb()) {
      cells[x][y].set_type(1);
      remaining--;
      // std::cout << "cell (" << x << ", " << y
      //           << ") : successfully placed bomb, remaining = " << remaining
      //           << std::endl;
    }
  }
}

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

bool CellGrid::hasBombsNearby(int x, int y) {
  // std::cout << "enters hasBombsNearby" << std::endl;
  for (int row = x - 1; row <= x + 1; row++) {
    for (int col = y - 1; col <= y + 1; col++) {
      // std::cout << "for loop index (" << row << ", " << col << ")" <<
      // std::endl;
      if (row >= 0 && row < 16 && col >= 0 && col < 30 &&
          !(row == x && col == y)) {
        // std::cout << "checking cell (" << row << ", " << col << ")"
        //           << std::endl;
        if (cells[row][col].getType() == 1) {
          // std::cout << "cell (" << row << ", " << col << ") bombs nearby =
          // TRUE"
          //           << std::endl;
          return true;
        }
      }
    }
  }
  // std::cout << "cell (" << x << ", " << y << ") bombs nearby = FALSE"
  //           << std::endl;
  return false;
}

int CellGrid::countBombsNearby(int x, int y) {
  int count = 0;

  for (int row = x - 1; row <= x + 1; row++) {
    for (int col = y - 1; col <= y + 1; col++) {
      if (row >= 0 && row < 16 && col >= 0 && col < 30 &&
          !(row == x && col == y)) {
        if (cells[row][col].getType() == 1) {
          // std::cout << "cell (" << row << ", " << col << ") contains a bomb "
          //           << std::endl;
          count += 1;
        }
      }
    }
  }
  // std::cout << "cell (" << x << ", " << y << ") has " << count
  //           << " bombs nearby" << std::endl;
  return count;
}
