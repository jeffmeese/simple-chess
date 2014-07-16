#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "engine.h"

#include <memory>

#include <core/types.h>
#include <core/piecetype.h>

class Move;
class MoveList;

class Game
{
public:
  Game();
  ~Game();

public:
  const Board * board() const;

public:
  bool doMove(const sMove & newMove);
  bool executeEngineMove();
  uchar generateMoves(uchar row, uchar col, sMove * moves);
  bool isBlackMated() const;
  bool isStalemate() const;
  bool isWhiteMated() const;
  bool isWhiteToMove() const;
  bool setBoardPosition(const std::string & fenString);
  void startNewGame();
  bool undoLastMove();

private:
  void checkMateCondition();

protected:
  std::unique_ptr<Board> mBoard;
  std::unique_ptr<Engine> mEngine;
  bool mWhiteMated;
  bool mBlackMated;
  bool mGameOver;
  sMove mCompletedMoves[256];
  uchar mTotalCompletedMoves;
};

#endif // GAME_H
