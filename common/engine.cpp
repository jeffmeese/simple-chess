#include "engine.h"

#include <iostream>

static const int INFINITY = 1e10;
static const int PawnWeight = 100;
static const int KnightWeight = 200;
static const int BishopWeight = 400;
static const int RookWeight = 550;
static const int QueenWeight = 950;
static const int KingWeight = 1950;

Engine::Engine()
  : mMaxDepth(3)
{
  srand((unsigned int)time(NULL));
}

double Engine::evaluatePosition(Board * board)
{
  double materialScore = 0;
  for (uint row = 0; row < 8; row++) {
    for (uint col = 0; col < 8; col++) {
      PieceType pieceType = board->pieceType(row, col);
      switch (pieceType) {
      case WhitePawn:
        materialScore += PawnWeight;
        break;
      case WhiteKnight:
        materialScore += KnightWeight;
        break;
      case WhiteBishop:
        materialScore += BishopWeight;
        break;
      case WhiteRook:
        materialScore += RookWeight;
        break;
      case WhiteQueen:
        materialScore += QueenWeight;
        break;
      case WhiteKing:
        materialScore += KingWeight;
        break;
      case BlackPawn:
        materialScore -= PawnWeight;
        break;
      case BlackKnight:
        materialScore -= KnightWeight;
        break;
      case BlackBishop:
        materialScore -= BishopWeight;
        break;
      case BlackRook:
        materialScore -= RookWeight;
        break;
      case BlackQueen:
        materialScore -= QueenWeight;
        break;
      case BlackKing:
        materialScore -= KingWeight;
        break;
      default:
        break;
      }
    }
  }

  //if ( board->isWhiteToMove())
  //  materialScore = -materialScore;

  MoveList moveList;
  uchar sideMobility = board->generateMoves(moveList);
  uchar opponentMobility = board->generateOpponentMoves(moveList);
//  double whiteMobility = sideMobility;
//  double blackMobility = opponentMobility;
//  if (!board->isWhiteToMove()) {
//    whiteMobility = opponentMobility;
//    blackMobility = sideMobility;
//  }

  double mobilityScore = (sideMobility - opponentMobility);
  double totalScore = materialScore + 0.1*mobilityScore;
  if (board->sideToMove() != White)
    totalScore = -totalScore;

  return totalScore;
}

bool Engine::getMove(Board * board, Move & selectedMove)
{
  mEngineFile.open("debug/engine.dat");

  MoveList moveList;
  uchar totalMoves = board->generateMoves(moveList);

  std::cout << "\nEngine Move\n";
  double maxScore = -INFINITY;
  mCurrentLine.clear();
  mScoreVector.clear();
  for (uchar i = 0; i < totalMoves; i++) {
    Move currentMove = moveList[i];

    board->makeMove(currentMove);
    if (!board->isKingAttacked(!board->sideToMove())) {
      mCurrentLine.addMove(currentMove);
      //mEngineFile << getSmithNotation(currentMove) << "\n";
      double score = negaMax(board, mMaxDepth);
      //mEngineFile <<  "Score: " << score << "\n";
      if (score > maxScore) {
        maxScore = score;
        selectedMove = currentMove;
      }
      for (uchar i = 0; i < mCurrentLine.size(); i++) {
        if (i > 0) mEngineFile << "-";
        mEngineFile << getSmithNotation(mCurrentLine[i]);
      }
      mEngineFile << " " << score << "\n";
      mCurrentLine.removeLast();
    }
    board->unmakeMove(currentMove);
  }

  mEngineFile.close();
  return true;
}

double Engine::negaMax(Board * board, int depth)
{
  if (depth == 0) {

    double finalScore = evaluatePosition(board);
    //for (int i = 0; i < mMaxDepth; i++) mEngineFile << "  ";
    //mEngineFile << " Score: " << finalScore << "\n";
    //mEngineFile << " Score: " << finalScore << "\n";
    //std::cout << "Best Line: ";
//    for (uchar i = 0; i < mCurrentLine.size(); i++) {
//      if (i > 0) mEngineFile << "-";
//      mEngineFile << getSmithNotation(mCurrentLine[i]);
//    }
//    mEngineFile << " " << finalScore << "\n";
    //std::cout << " Score: " << finalScore << "\n";

//    if (finalScore > mMaxScore) {
//      //mMaxScore = finalScore;
//      //std::cout << "Best Line: ";
//      for (uchar i = 0; i < mCurrentLine.size(); i++) {
//        //if (i > 0) std::cout << "-";
//        //std::cout << getSmithNotation(mCurrentLine[i]);
//      }
//      //std::cout << " Score: " << finalScore << "\n";
//    }

    return finalScore;
  }

  MoveList moveList;
  uchar totalMoves = board->generateMoves(moveList);

  double maxScore = -INFINITY;
  for (uchar i = 0; i < totalMoves; i++) {
    Move currentMove = moveList[i];

    board->makeMove(currentMove);
    if (!board->isKingAttacked(!board->sideToMove())) {
      mCurrentLine.addMove(currentMove);

      //for (int i = 0; i < mMaxDepth - (depth-1); i++) mEngineFile << "  ";
      //mEngineFile << getSmithNotation(currentMove) << "\n";
      //mEngineFile << "-" << getSmithNotation(currentMove);

      double score = -negaMax(board, depth-1);
      if (score > maxScore) {
        maxScore = score;
      }

//      for (int i = 0; i < mMaxDepth - (depth-1); i++) mEngineFile << "  ";
//      for (uchar i = 0; i < mCurrentLine.size(); i++) {
//        if (i > 0) mEngineFile << "-";
//        mEngineFile << getSmithNotation(mCurrentLine[i]);
//      }
//      mEngineFile << " " << score << "\n";


      //mEngineFile <<  "Score: " << score << "\n";
      mCurrentLine.removeLast();
    }
    board->unmakeMove(currentMove);
  }
  return maxScore;
}

