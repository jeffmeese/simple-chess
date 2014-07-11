#ifndef CONSOLE_GAME_H
#define CONSOLE_GAME_H

#include <common/game.h>

#include <sstream>
#include <memory>

class ConsoleGame
    : public Game
{
public:
  ConsoleGame();
  ~ConsoleGame();

public:
  int run();

private:
  void handleHelp() const;
  void handleMove(std::istringstream & iss);
  void handleDivide(std::istringstream & iss) const;
  void handlePerft(std::istringstream & iss) const;
  void handleTable(std::istringstream & iss) const;
  void handleNewGame();
  void handlePrint() const;
  void handleSetBoard(std::istringstream & iss);
  void handleShow() const;
  void handleUndo();
  bool isGameOver();

private:
  bool mGameOver;
};

#endif // #ifndef CONSOLE_GAME_H
