#include "../include/minesweeperwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPixmap>
#include <QString>
#include <iostream>

MinesweeperWindow::MinesweeperWindow(QWidget *parent)
    : QWidget(parent), gridLayout(new QGridLayout(this)) {
  setWindowTitle("Minesweeper");
  gridLayout->setSpacing(1);

  const int rows = 16;
  const int cols = 30;
  const int numBombs = 5;

  buttonGrid.resize(rows, std::vector<QPushButton *>(cols));

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      QPushButton *button = new QPushButton();
      button->setFixedSize(25, 35);
      button->setProperty("row", row);
      button->setProperty("col", col);
      button->installEventFilter(this);
      gridLayout->addWidget(button, row, col);
      buttonGrid[row][col] = button;
    }
  }
  setLayout(gridLayout);
  cellGrid = CellGrid();
  cellGrid.placeBombs(numBombs);
  cellCount = ((16 * 30) - numBombs) - 1;
  // cellCount = (16 * 30) - 1;
  cellGrid.printCellGrid();
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

void MinesweeperWindow::checkIfGameWon() {
  if (cellCount == 0) {

    for (int row = 0; row < 16; row++) {
      for (int col = 0; col < 30; col++) {
        if (!cellGrid.getCell(row, col).getRevealed()) {
          changeIcon(row, col, "bomb");
        }
      }
    }

    QMessageBox winMessage;
    winMessage.setWindowTitle("Congratulations!");
    winMessage.setText("Woohoo! You solved this sudoku puzzle!");
    winMessage.setIcon(QMessageBox::Information);

    QPushButton *exitButton =
        winMessage.addButton("Quit", QMessageBox::RejectRole);
    QPushButton *playAgainButton =
        winMessage.addButton("Play Again", QMessageBox::AcceptRole);

    winMessage.exec();

    if (winMessage.clickedButton() == playAgainButton) {
      // resetGame();
    } else if (winMessage.clickedButton() == exitButton) {
      QApplication::quit();
    }
  }
}

void MinesweeperWindow::hideButton(int x, int y) {
  QPushButton *button = buttonGrid[x][y];
  // std::cout << "cell (" << x << ", " << y << ") : icon removed " <<
  // std::endl;
  button->hide();
}

void MinesweeperWindow::setDigitIcon(int x, int y, int digit) {
  // std::cout << "cell (" << x << ", " << y << ") : icon changed to = " <<
  // digit
  //           << std::endl;
  hideButton(x, y);
  QLabel *label = new QLabel(this);
  label->setAlignment(Qt::AlignCenter);
  label->setFixedSize(25, 20);
  label->setText(QString::number(digit));

  const std::string colors[] = {"#1e9400", "#498700", "#627800", "#736900",
                                "#815800", "#8c4400", "#932c00", "#960000"};
  QString color = QString::fromStdString(colors[digit - 1]);
  label->setStyleSheet(QString("color: %1; font-weight: bold;").arg(color));
  gridLayout->addWidget(label, x, y);
  checkIfGameWon();
}

void MinesweeperWindow::changeIcon(int x, int y, std::string iconType) {
  QPushButton *button = buttonGrid[x][y];
  // std::cout << "cell (" << x << ", " << y
  //           << ") : icon changed to = " << iconType << std::endl;
  std::string iconUrl = "./icons/" + iconType + ".png";
  // std::cout << "icon url : " << iconUrl << std::endl;
  if (!QFile::exists(QString::fromStdString(iconUrl))) {
    qWarning() << "Icon file not found:" << QString::fromStdString(iconUrl);
    return;
  }
  QIcon icon(QString::fromStdString(iconUrl));
  if (icon.isNull()) {
    qWarning() << "Failed to load icon from:"
               << QString::fromStdString(iconUrl);
    return;
  }
  button->setIcon(icon);
  button->setIconSize(QSize(20, 20));
  button->setProperty("iconType", QString::fromStdString(iconType));
}

void MinesweeperWindow::revealAndDecrement(int row, int col) {
  std::cout << "revealing (" << row << ", " << col
            << "), cell count = " << cellCount << std::endl;
  cellGrid.getCell(row, col).setRevealed(true);
  cellCount -= 1;
}

void MinesweeperWindow::revealNearby(int x, int y) {
  // std::cout << "begining of revealNearby(" << x << ", " << y << ")"
  //           << std::endl;
  if (cellGrid.getCell(x, y).getRevealed()) {
    // std::cout << "cell (" << x << ", " << y << ") already revealed, return"
    //           << std::endl;
    return;
  }

  if (cellGrid.hasBombsNearby(x, y)) {
    // std::cout << "cell (" << x << ", " << y << ") has bombs nearby"
    //           << std::endl;
    revealAndDecrement(x, y);
    int bombsNearby = cellGrid.countBombsNearby(x, y);
    setDigitIcon(x, y, bombsNearby);
    return;
  } else {
    revealAndDecrement(x, y);
    hideButton(x, y);

    for (int row = x - 1; row <= x + 1; row++) {
      for (int col = y - 1; col <= y + 1; col++) {
        if (row >= 0 && row < 16 && col >= 0 && col < 30 &&
            !(row == x && col == y)) {
          revealNearby(row, col);
        }
      }
    }
  }
}

void MinesweeperWindow::onLeftClick(int row, int col) {
  // std::cout << "cell (" << row << ", " << col << ") : left click" <<
  // std::endl;
  if (cellGrid.getCell(row, col).getType() == 1) {
    changeIcon(row, col, "bomb");
    // GAME OVER CODE HERE, SHOW POPUP
    // IF NO, CLOSE GU, END PROGRAM
    // IF YES, RESET GUI AND CELLGRID
  } else if (cellGrid.hasBombsNearby(row, col)) {
    revealAndDecrement(row, col);
    int bombsNearby = cellGrid.countBombsNearby(row, col);
    setDigitIcon(row, col, bombsNearby);
  } else {
    revealAndDecrement(row, col);
    hideButton(row, col);

    for (int x = row - 1; x <= row + 1; x++) {
      for (int y = col - 1; y <= col + 1; y++) {
        if (x >= 0 && x < 16 && y >= 0 && y < 30 && !(x == row && y == col)) {
          // std::cout << "initial revealNearby(" << x << ", " << y << ")"
          //           << std::endl;
          revealNearby(x, y);
        }
      }
    }
  }
}

void MinesweeperWindow::onRightClick(int row, int col) {
  // std::cout << "cell (" << row << ", " << col << ") : right click" <<
  // std::endl;
  QPushButton *button = buttonGrid[row][col];
  if (button->icon().isNull()) {
    // std::cout << "cell (" << row << ", " << col << ") : setting flag icon"
    //           << std::endl;
    changeIcon(row, col, "flag");
    return;
  }
  QString iconType = button->property("iconType").toString();
  if (iconType == "flag") {
    // std::cout << "cell (" << row << ", " << col << ") : change flag to
    // question"
    //           << std::endl;
    changeIcon(row, col, "question_mark");
  } else if (iconType == "question_mark") {
    // std::cout << "cell (" << row << ", " << col << ") : remove icon"
    //           << std::endl;
    button->setIcon(QIcon());
  }
}