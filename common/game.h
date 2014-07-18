#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "engine.h"

#include <memory>

#include <core/types.h>

enum GameType
{
  HumanVsComputer,
  ComputerVsHuman,
  HumanVsHuman
};

class Move;
class MoveList;

class Game
{
public:
  Game();
  ~Game();

public:
  const Board * board() const;
  GameType gameType() const;
  void setGameType(GameType gameType);

public:
  bool doMove(const Move & newMove);
  bool executeEngineMove();
  uchar generateMoves(uchar row, uchar col, MoveList & moveList);
  bool isBlackMated() const;
  bool isGameOver() const;
  bool isStalemate() const;
  bool isWhiteMated() const;
  bool isWhiteToMove() const;
  bool setBoardPosition(const std::string & fenString);
  void startNewGame();
  bool undoLastMove();

private:
  void checkMateCondition();

protected:
  bool mWhiteMated;
  bool mBlackMated;
  bool mStalemate;
  GameType mGameType;
  std::unique_ptr<Board> mBoard;
  std::unique_ptr<Engine> mEngine;
  std::unique_ptr<MoveList> mCompletedMoves;
};

#endif // GAME_H
