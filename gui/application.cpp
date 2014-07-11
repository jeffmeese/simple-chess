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

//#include "application.h"
//#include "game.h"

//Application::Application(int argc, char ** argv)
//  : QApplication(argc, argv)
  //, mGame(new ChessGame)
  /*, mBoard(new Board15x12)
  , mEngine(new StandardEngine(mBoard.get()))
  , mGame(new Game(mBoard.get(), mEngine.get()))*/
//{
//  startNewGame();
//}

//Board * Application::board() const
//{
//  return mGame->board();

//Engine * Application::engine() const
//{
//  return mEngine.get();
//}

//Game * Application::game() const
//{
//  return mGame.get();
//}

//void Application::startNewGame()
//{
//  mGame.reset(new ChessGame);
  //mBoard.reset(new Board15x12);
  //mEngine.reset(new StandardEngine(mBoard.get()));
  //mGame.reset(new Game(mBoard.get(), mEngine.get()));
//}
