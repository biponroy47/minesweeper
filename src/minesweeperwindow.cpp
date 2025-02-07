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
#include <QTimer>
#include <iostream>

// initialize main game gui
MinesweeperWindow::MinesweeperWindow(QWidget *parent)
    : QWidget(parent), gridLayout(new QGridLayout(this)) {
  setWindowTitle("Minesweeper");
  gridLayout->setSpacing(1);

  // iniitalize 16x30 buttons and labels
  buttonGrid.resize(16, std::vector<QPushButton *>(30));
  labelGrid.resize(16, std::vector<QLabel *>(30, nullptr));
  for (int row = 0; row < 16; row++) {
    for (int col = 0; col < 30; col++) {
      QPushButton *button = new QPushButton();
      button->setFixedSize(25, 35);
      button->setProperty("row", row);
      button->setProperty("col", col);
      button->installEventFilter(this);
      button->setStyleSheet("");
      gridLayout->addWidget(button, row, col);
      buttonGrid[row][col] = button;
    }
  }
  setLayout(gridLayout);
  cellGrid = CellGrid();
  cellGrid.placeBombs(99);
  cellCount = ((16 * 30) - 99) - 1;
  cellGrid.printCellGrid();
}

// add row and col property to buttons for event handler access
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

// method to check if all valid cells have been revealed
void MinesweeperWindow::checkIfGameWon() {
  if (cellCount == 0) {

    for (int row = 0; row < 16; row++) {
      for (int col = 0; col < 30; col++) {
        if (!cellGrid.getCell(row, col).getRevealed()) {
          changeIcon(row, col, "bomb");
        }
      }
    }

    // queue win screen
    QMessageBox winMessage;
    winMessage.setWindowTitle("Congratulations!");
    winMessage.setText("Woohoo! You solved this sudoku puzzle!");

    QPushButton *exitButton =
        winMessage.addButton("Quit", QMessageBox::RejectRole);
    QPushButton *playAgainButton =
        winMessage.addButton("Play Again", QMessageBox::AcceptRole);
    winMessage.exec();

    // reset if chosen to play again
    if (winMessage.clickedButton() == playAgainButton) {
      resetGame();
    }
    // else quit
    else if (winMessage.clickedButton() == exitButton) {
      QApplication::quit();
    }
  }
}

// method hides the button, used for cells with no bombs nearby
void MinesweeperWindow::hideButton(int x, int y) {
  QPushButton *button = buttonGrid[x][y];
  button->hide();
}

// sets numerical label for nearby bombs
void MinesweeperWindow::setDigitIcon(int x, int y, int digit) {
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
  labelGrid[x][y] = label;
  checkIfGameWon();
}

// changes icon to eiher flag, bomb, or question mark
void MinesweeperWindow::changeIcon(int x, int y, std::string iconType) {
  QPushButton *button = buttonGrid[x][y];
  std::string iconUrl = "./icons/" + iconType + ".png";
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

// method decrements valid cell counter and sets reveal value in cell object to
// true
void MinesweeperWindow::revealAndDecrement(int row, int col) {
  cellGrid.getCell(row, col).setRevealed(true);
  cellCount -= 1;
}

// method to begin revealing nearby cells no adjacent bombs
void MinesweeperWindow::revealNearby(int x, int y) {
  // returns if cell already revealed
  if (cellGrid.getCell(x, y).getRevealed()) {
    return;
  }
  // sets number of bombs nearby if there are any
  if (cellGrid.hasBombsNearby(x, y)) {
    revealAndDecrement(x, y);
    int bombsNearby = cellGrid.countBombsNearby(x, y);
    setDigitIcon(x, y, bombsNearby);
    return;
  } else {
    revealAndDecrement(x, y);
    hideButton(x, y);
    // recursively calls reveal on all valid cells nearby excluding its own
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

// left click handler
void MinesweeperWindow::onLeftClick(int row, int col) {
  // if user clicks bomb, change icon and end game
  if (cellGrid.getCell(row, col).getType() == 1) {
    changeIcon(row, col, "bomb");

    QPushButton *button = buttonGrid[row][col];
    button->setDown(false);
    button->update();
    QApplication::processEvents();

    // queue lose screen
    QTimer::singleShot(50, this, [this, row, col]() {
      QMessageBox loseMessage;
      loseMessage.setWindowTitle("Game Over!");
      loseMessage.setText("You lose! You found a bomb!");
      QPushButton *exitButton =
          loseMessage.addButton("Quit", QMessageBox::RejectRole);
      QPushButton *playAgainButton =
          loseMessage.addButton("Play Again", QMessageBox::AcceptRole);

      loseMessage.exec();

      if (loseMessage.clickedButton() == playAgainButton) {
        resetGame();
      } else if (loseMessage.clickedButton() == exitButton) {
        QApplication::quit();
      }
    });
  } else if (cellGrid.hasBombsNearby(row, col)) {
    revealAndDecrement(row, col);
    int bombsNearby = cellGrid.countBombsNearby(row, col);
    setDigitIcon(row, col, bombsNearby);
  } else {
    revealAndDecrement(row, col);
    hideButton(row, col);

    // initial reveal call if first cell clicked with no adjacent bombs
    for (int x = row - 1; x <= row + 1; x++) {
      for (int y = col - 1; y <= col + 1; y++) {
        if (x >= 0 && x < 16 && y >= 0 && y < 30 && !(x == row && y == col)) {
          revealNearby(x, y);
        }
      }
    }
  }
}

// method to chang icon of button to flag, question mark, or neither
void MinesweeperWindow::onRightClick(int row, int col) {
  QPushButton *button = buttonGrid[row][col];
  if (button->icon().isNull()) {
    changeIcon(row, col, "flag");
    return;
  }
  QString iconType = button->property("iconType").toString();
  if (iconType == "flag") {
    changeIcon(row, col, "question_mark");
  } else if (iconType == "question_mark") {

    button->setIcon(QIcon());
  }
}

// reset grid, buttons, labels, and cellGrid
void MinesweeperWindow::resetGame() {
  for (int row = 0; row < 16; row++) {
    for (int col = 0; col < 30; col++) {
      QPushButton *button = buttonGrid[row][col];
      button->setEnabled(true);
      button->setIcon(QIcon());
      button->setDown(false);
      button->show();
      if (labelGrid[row][col]) {
        gridLayout->removeWidget(labelGrid[row][col]);
        delete labelGrid[row][col];
        labelGrid[row][col] = nullptr;
      }
    }
  }
  cellGrid.resetCellGrid();
  cellGrid.placeBombs(99);
  cellCount = ((16 * 30) - 99) - 1;
}

// destructor
MinesweeperWindow::~MinesweeperWindow() {

  for (int row = 0; row < 16; row++) {
    for (int col = 0; col < 30; col++) {
      QPushButton *button = buttonGrid[row][col];
      if (button) {
        delete button;
        buttonGrid[row][col] = nullptr;
      }
    }
  }

  for (int row = 0; row < 16; row++) {
    for (int col = 0; col < 30; col++) {
      QLabel *label = labelGrid[row][col];
      if (label) {
        delete label;
        labelGrid[row][col] = nullptr;
      }
    }
  }
  if (gridLayout) {
    delete gridLayout;
    gridLayout = nullptr;
  }
}