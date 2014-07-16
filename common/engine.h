#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"

class Engine
{
public:
  Engine();

public:
  bool getMove(Board * board, sMove & engineMove);
  int evaluatePosition(Board * board);

private:
  int negaMax(Board * board, int depth);
};

#endif // ENGINE_H
