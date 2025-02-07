#ifndef MINESWEEPERWINDOW_H
#define MINESWEEPERWINDOW_H

#include "./cellgrid.h"
#include <QGridLayout>
#include <QPushButton>
#include <QWidget>
#include <vector>

class MinesweeperWindow : public QWidget {
  Q_OBJECT

public:
  explicit MinesweeperWindow(QWidget *parent = nullptr);

protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
  void revealAndDecrement(int x, int y);
  void revealNearby(int x, int y);
  void onLeftClick(int x, int y);
  void onRightClick(int x, int y);
  void hideButton(int x, int y);
  void setDigitIcon(int x, int y, int digit);
  void changeIcon(int x, int y, std::string iconType);
  void checkIfGameWon();

private:
  QGridLayout *gridLayout;
  std::vector<std::vector<QPushButton *>> buttonGrid;
  CellGrid cellGrid;
  int cellCount;
};

#endif