#include "../include/cell.h"
#include <iostream>

Cell::Cell() {}

bool Cell::getRevealed() {
  // std::cout << "cell revealed = " << revealed << std::endl;
  return revealed;
}

void Cell::setRevealed(bool new_revealed) {
  revealed = new_revealed;
  // std::cout << "cell new revealed = " << revealed << std::endl;
}

int Cell::getType() {
  // std::cout << "cell type = " << type << std::endl;
  return type;
}

void Cell::set_type(int new_type) {
  type = new_type;
  // std::cout << "cell new type = " << type << std::endl;
}

bool Cell::is_bomb() {
  // std::cout << "cell is a bomb = " << ((type == 1) ? "true" : "false")
  // << std::endl;
  return (type == 1);
}
