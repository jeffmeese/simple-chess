#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include "boardview.h"
#include "imagemap.h"
#include "guigame.h"

#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>

MainWindow::MainWindow(Application & application, QWidget *parent)
  : QMainWindow(parent)
  , mUi(new Ui::MainWindow)
  , mApplication(application)
  , mImageMap(new ImageMap)
  , mBoardView(new BoardView)
{
  mUi->setupUi(this);

  mBoardView->setGame(mApplication.game());
  mBoardView->setImageMap(mImageMap.get());

  resize(800,700);
  setCentralWidget(mBoardView.get());

  connect(mUi->actionStart_New_Game, SIGNAL(triggered()), SLOT(handleStartNewGame()));
  connect(mUi->actionExit, SIGNAL(triggered()), SLOT(handleExit()));
  connect(mUi->actionUndo_Last_Move, SIGNAL(triggered()), SLOT(handleUndoLastMove()));
  connect(mUi->actionSet_Board_Position, SIGNAL(triggered()), SLOT(handleSetBoardPosition()));
  connect(mBoardView.get(), SIGNAL(moveCompleted(const Move&)), SLOT(handleMoveCompleted()));
}

MainWindow::~MainWindow()
{
  delete mUi;
}

void MainWindow::handleExit()
{
  QApplication::quit();
}

void MainWindow::handleMoveCompleted()
{
  update();

  bool gameOver = mApplication.game()->isGameOver();
  if (gameOver) {

    bool isBlackMate = mApplication.game()->isBlackMated();
    if (isBlackMate) {
      QString message("Checkmate\r\nWhite wins\r\n\r\nWould you like to play again?");
      QMessageBox::information(this, "Checkmate", message);
    }

    bool isWhiteMate = mApplication.game()->isWhiteMated();
    if (isWhiteMate) {
      QString message("Checkmate\r\nBlack wins\r\n\r\nWould you like to play again?");
      QMessageBox::information(this, "Checkmate", message);
    }

    bool isStalemate = mApplication.game()->isStalemate();
    if (isStalemate) {
      QString message("Stalemate\r\nWould you like to play again?");
      QMessageBox::information(this, "Stalemate", message);
    }
    return;
  }

  bool blackMove = !mApplication.game()->isWhiteToMove();
  if (blackMove) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (mApplication.game()->executeEngineMove()) {
      handleMoveCompleted();
    }
    QApplication::restoreOverrideCursor();
  }
}

void MainWindow::handleSetBoardPosition()
{
  QString fenString = QInputDialog::getText(this, "Set Board Position", "Enter fen string");
  if (fenString.isEmpty())
    return;

  mApplication.game()->setBoardPosition(fenString.toStdString());
  update();
}

void MainWindow::handleStartNewGame()
{
  mApplication.startNewGame();
  mBoardView->setGame(mApplication.game());

  update();
  handleMoveCompleted();
}

void MainWindow::handleUndoLastMove()
{
  mApplication.game()->undoLastMove();
  update();
}
