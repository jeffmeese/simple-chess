#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"

class Engine
{
public:
  Engine();

public:
  bool getMove(Board * board, Move & selectedMove);
  int evaluatePosition(Board * board);

private:
  int negaMax(Board * board, int depth);
  int alphaBeta(Board * board, int depth, int alpha, int beta);
};

#endif // ENGINE_H
