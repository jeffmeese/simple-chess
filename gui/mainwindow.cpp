#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include "boardview.h"
#include "imagemap.h"
#include "guigame.h"

#include <QMessageBox>
#include <QDebug>

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
  connect(mUi->actionUndo_Last_Move, SIGNAL(triggered()), SLOT(handleUndoLastMove()));
  connect(mBoardView.get(), SIGNAL(moveCompleted(const Move&)), SLOT(handleMoveCompleted()));
}

MainWindow::~MainWindow()
{
  delete mUi;
}

void MainWindow::handleMoveCompleted()
{
  repaint();

  bool gameOver = false;

  bool isBlackMate = mApplication.game()->isBlackMated();
  if (isBlackMate) {
    QString message("Checkmate\r\nWhite wins\r\n\r\nWould you like to play again?");
    QMessageBox::information(this, "Checkmate", message);
    gameOver = true;
  }

  bool isWhiteMate = mApplication.game()->isWhiteMated();
  if (isWhiteMate) {
    QString message("Checkmate\r\nBlack wins\r\n\r\nWould you like to play again?");
    QMessageBox::information(this, "Checkmate", message);
    gameOver = true;
  }

  bool isStalemate = mApplication.game()->isStalemate();
  if (isStalemate) {
    QString message("Stalemate\r\nWould you like to play again?");
    QMessageBox::information(this, "Stalemate", message);
    gameOver = true;
  }

//  if (!gameOver) {
//    bool blackMove = !mApplication.game()->isWhiteToMove();
//    if (blackMove) {
//      QApplication::setOverrideCursor(Qt::WaitCursor);
//      if (mApplication.game()->executeEngineMove()) {
//        handleMoveCompleted();
//      }
//      QApplication::restoreOverrideCursor();
//    }
//  }

  mBoardView->repaint();
}

void MainWindow::handleStartNewGame()
{
  mApplication.startNewGame();
  mBoardView->setGame(mApplication.game());

  repaint();
  handleMoveCompleted();
}

void MainWindow::handleUndoLastMove()
{
  mApplication.game()->undoLastMove();
  repaint();
}
