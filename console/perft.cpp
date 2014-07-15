#include "perft.h"

#include <core/move.h>
#include <core/movelist.h>

#include <iostream>
#include <iomanip>
#include <map>

Perft::Perft(Board * board)
  : mBoard(board)//mBoard(new Board)
  , mSaveTableResults(false)
{
  resetCounters();
}

void Perft::divide(uint perftDepth)
{
  MoveList moveList;
  uint totalMoves = mBoard->generateLegalMoves(moveList);

  //std::map<std::string, ulonglong> results;
  //std::vector<std::string> results;

  ulonglong totalNodes = 0;
  for (uint i = 0; i < moveList.totalMoves(); i++) {
    Move move = moveList[i];
    mBoard->makeMove(move);
    ulonglong nodes = executePerft(perftDepth - 1);
    mBoard->unmakeMove(move);

    totalNodes += nodes;

    std::string moveString = move.getSmithNotation();
    std::cout << moveString << " " << nodes << "\n";
    //results.push_back(moveString);
    //results[moveString] = nodes;
  }

  //for (std::map<std::string, ulonglong>::iterator itr = results.begin(); itr != results.end(); ++itr) {
  //  std::cout << itr->first << " " << itr->second << "\n";
 // }

  std::cout << "\n";
  std::cout << "Moves: " << totalMoves << "\n";
  std::cout << "Nodes: " << totalNodes << "\n";
}

ulonglong Perft::execute(uint perftDepth)
{
  resetCounters();
  return executePerft(perftDepth);
}

ulonglong Perft::executePerft(uint perftDepth)
{
  if (perftDepth == 0)
    return 1;

  MoveList moveList;
  uint totalMoves = mBoard->generateLegalMoves(moveList);

  uint totalNodes = 0;
  for (uint i = 0; i < totalMoves; i++) {
    Move move = moveList[i];

    mBoard->makeMove(move);
    if (!mBoard->isCellAttacked(mBoard->getKingIndex(!mBoard->mSideToMove), mBoard->mSideToMove))
      totalNodes += executePerft(perftDepth-1);
    mBoard->unmakeMove(move);

    // Save the table results
    // This will slow down the perft a bit
//    if (mSaveTableResults) {
//      if (move.isCapture())
//        mTotalCaptures++;
//      if (move.isKingCastle() || move.isQueenCastle())
//        mTotalCastles++;
//      if (move.isEnPassant())
//        mTotalEnpassants++;
//      if (move.isPromotion())
//        mTotalPromotions++;
//      if (mBoard->isWhiteChecked() || mBoard->isBlackChecked() )
//        mTotalChecks++;

//      bool whiteMove = mBoard->isWhiteToMove();
//      bool whiteChecked = mBoard->isWhiteChecked();
//      bool blackChecked = mBoard->isBlackChecked();
//      if ( (whiteMove && whiteChecked) || (!whiteMove && blackChecked) ) {
//        MoveList checkMoves;
//        mBoard->generateLegalMoves(checkMoves);
//        if (checkMoves.totalMoves() == 0) {
//          mTotalCheckmates++;
//        }
//      }
//    }
//    totalNodes += executePerft(perftDepth-1);
//    mBoard->unmakeMove(move);
  }

  return totalNodes;
}

void Perft::resetCounters()
{
  mTotalCaptures = 0;
  mTotalCastles = 0;
  mTotalCheckmates = 0;
  mTotalChecks = 0;
  mTotalEnpassants = 0;
  mTotalPromotions = 0;
}

bool Perft::saveTableResults() const
{
  return mSaveTableResults;
}

void Perft::setSaveTableResults(bool saveResults)
{
  mSaveTableResults = saveResults;
}


ulonglong Perft::totalCaptures() const
{
  return mTotalCaptures;
}

ulonglong Perft::totalCastles() const
{
  return mTotalCastles;
}

ulonglong Perft::totalChecks() const
{
  return mTotalChecks;
}

ulonglong Perft::totalCheckmates() const
{
  return mTotalCheckmates;
}

ulonglong Perft::totalEnpassants() const
{
  return mTotalEnpassants;
}

ulonglong Perft::totalPromotions() const
{
  return mTotalPromotions;
}
