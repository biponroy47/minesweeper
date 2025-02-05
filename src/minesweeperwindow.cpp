#include "../include/minesweeperwindow.h"
#include <QDebug>
#include <QMouseEvent>

MinesweeperWindow::MinesweeperWindow(QWidget *parent)
    : QWidget(parent), gridLayout(new QGridLayout(this)) {
  setWindowTitle("Minesweeper");
  gridLayout->setSpacing(1);

  const int rows = 16;
  const int cols = 30;

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      QPushButton *button = new QPushButton();
      button->setFixedSize(25, 35);
      button->setProperty("row", row);
      button->setProperty("col", col);
      button->installEventFilter(this);
      gridLayout->addWidget(button, row, col);
    }
  }

  setLayout(gridLayout);
}

bool MinesweeperWindow::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::MouseButtonPress) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    QPushButton *button = qobject_cast<QPushButton *>(obj);

    if (button) {
      int row = button->property("row").toInt();
      int col = button->property("col").toInt();

      if (mouseEvent->button() == Qt::LeftButton) {
        onLeftClick(row, col);
      } else if (mouseEvent->button() == Qt::RightButton) {
        onRightClick(row, col);
      }
    }
  }

  return QWidget::eventFilter(obj, event);
}

void MinesweeperWindow::onLeftClick(int row, int col) {
  qDebug() << "Left click at row:" << row << ", col:" << col;
  // Add your logic for left-click here
}

void MinesweeperWindow::onRightClick(int row, int col) {
  qDebug() << "Right click at row:" << row << ", col:" << col;
  // Add your logic for right-click here
}