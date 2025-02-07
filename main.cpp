#include "include/cell.h"
#include "include/minesweeperwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {

  // create main game window
  QApplication app(argc, argv);
  MinesweeperWindow window;
  window.show();

  return app.exec();
}
