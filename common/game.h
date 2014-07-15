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
  bool doMove(const Move & newMove);
  bool executeEngineMove();
  bool generateMoves(uint row, uint col, MoveList & moveList);
  PieceType getPieceType(uint row, uint col) const;
  bool isBlackMated() const;
  bool isStalemate() const;
  bool isWhiteMated() const;
  bool isWhiteToMove() const;
  void setBoardPosition(const std::string & fenString);
  void startNewGame();
  bool undoLastMove();

private:
  void checkMateCondition();

protected:
  std::unique_ptr<Board> mBoard;
  std::unique_ptr<Engine> mEngine;
  //std::unique_ptr<MoveList> mCompletedMoves;
  bool mWhiteMated;
  bool mBlackMated;
};

#endif // GAME_H
