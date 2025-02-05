#include "include/cell.h"
#include "include/minesweeperwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {

  QApplication app(argc, argv);
  MinesweeperWindow window;
  window.show();

  Cell cell[16][30];

  return app.exec();
}
