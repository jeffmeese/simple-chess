#ifndef PERFT_H
#define PERFT_H

#include <core/types.h>
#include <core/move.h>
#include <core/movelist.h>

#include <common/board.h>

#include <iostream>
#include <iomanip>
#include <map>
#include <memory>

class Perft
{
public:
  Perft(Board * board);

public:
  bool saveTableResults() const;
  void setSaveTableResults(bool saveResults);
  ulonglong totalCaptures() const;
  ulonglong totalCastles() const;
  ulonglong totalChecks() const;
  ulonglong totalCheckmates() const;
  ulonglong totalEnpassants() const;
  ulonglong totalPromotions() const;

public:
  void divide(uint perftDepth);
  ulonglong execute(uint perftDepth);

private:
  ulonglong executePerft(uint perftDepth);
  void resetCounters();

private:
  Board * mBoard;
  //std::unique_ptr<Board> mBoard;
  bool mSaveTableResults;
  ulonglong mTotalCaptures;
  ulonglong mTotalCastles;
  ulonglong mTotalChecks;
  ulonglong mTotalCheckmates;
  ulonglong mTotalEnpassants;
  ulonglong mTotalPromotions;
};

#endif // PERFT_H
