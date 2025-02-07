#include "../include/cell.h"
#include <iostream>

// cell object, no comments since its quite obvious what each method does...
Cell::Cell() {}

bool Cell::getRevealed() { return revealed; }

void Cell::setRevealed(bool new_revealed) { revealed = new_revealed; }

int Cell::getType() { return type; }

void Cell::set_type(int new_type) { type = new_type; }

bool Cell::isBomb() { return (type == 1); }
