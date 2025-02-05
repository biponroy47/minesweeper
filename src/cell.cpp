#include "../include/cell.h"
#include <iostream>

Cell::Cell() {}

bool Cell::get_revealed() {
  std::cout << "revealed = " << revealed << std::endl;
  return revealed;
}

void Cell::set_revealed(bool new_revealed) {
  revealed = new_revealed;
  std::cout << "new revealed = " << revealed << std::endl;
}

int Cell::get_type() {
  std::cout << "type = " << type << std::endl;
  return type;
}

void Cell::set_type(int new_type) {
  type = new_type;
  std::cout << "new type = " << type << std::endl;
}

bool Cell::is_bomb() {
  std::cout << "is bomb? " << (type == 1) << std::endl;
  return (type == 1);
}
