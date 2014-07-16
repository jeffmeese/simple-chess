#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

namespace Ui {
  class MainWindow;
}

class Application;
class BoardView;
class ImageMap;

class MainWindow
    : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(Application & application, QWidget *parent = 0);
  ~MainWindow();

private Q_SLOTS:
  void handleMoveCompleted();
  void handleSetBoardPosition();
  void handleStartNewGame();
  void handleUndoLastMove();

private:
  Ui::MainWindow * mUi;
  Application & mApplication;
  std::unique_ptr<ImageMap> mImageMap;
  std::unique_ptr<BoardView> mBoardView;
};

#endif // MAINWINDOW_H
