#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"

#include <core/movelist.h>

#include <map>
#include <fstream>

class Engine
{
public:
  Engine();

public:
  bool getMove(Board * board, Move & selectedMove);
  double evaluatePosition(Board * board);

private:
  double negaMax(Board * board, int depth);
  int alphaBeta(Board * board, int depth, int alpha, int beta);

private:
  typedef std::pair<std::string, int> LineScore;
  typedef std::vector<LineScore> ScoreVector;

private:
  MoveList mCurrentLine;
  int mMaxDepth;
  ScoreVector mScoreVector;
  std::ofstream mEngineFile;
};

#endif // ENGINE_H
