#ifndef MINESWEEPERWINDOW_H
#define MINESWEEPERWINDOW_H

#include <QGridLayout>
#include <QPushButton>
#include <QWidget>

class MinesweeperWindow : public QWidget {
  Q_OBJECT

public:
  explicit MinesweeperWindow(QWidget *parent = nullptr);

protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
  void onLeftClick(int row, int col);
  void onRightClick(int row, int col);

private:
  QGridLayout *gridLayout;
};

#endif