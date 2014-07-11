#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <memory>

class GuiGame;

class Application
    : public QApplication
{
  Q_OBJECT

public:
  explicit Application(int & argc, char ** argv);
  ~Application();

public:
  GuiGame * game() const;

public:
  void startNewGame();

private:
  std::unique_ptr<GuiGame> mGame;
};

#endif // APPLICATION_H
