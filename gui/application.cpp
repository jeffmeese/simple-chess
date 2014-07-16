#include "application.h"
#include "guigame.h"

Application::Application(int & argc, char ** argv)
  : QApplication(argc, argv)
  , mGame(new GuiGame)
{
}

Application::~Application()
{
}

GuiGame * Application::game() const
{
  return mGame.get();
}

void Application::startNewGame()
{
  mGame.reset(new GuiGame);
}
