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
  void doPerft(int perftLevel) const;
  int run();

private:
  void handleDivide(std::istringstream & iss) const;
  void handleFen() const;
  void handleHelp() const;
  void handleMove(std::istringstream & iss);
  void handleNewGame();
  void handlePerft(std::istringstream & iss) const;
  void handlePrint() const;
  void handleSetBoard(std::istringstream & iss);
  void handleShow() const;
  void handleSinglePlayer();
  void handleTable(std::istringstream & iss) const;
  void handleTestMoveGen() const;
  void handleTwoPlayer();
  void handleUndo();

private:
  void showEndGame();
};

#endif // #ifndef CONSOLE_GAME_H
