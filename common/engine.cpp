#include "engine.h"

//#include <cmath>

//static const int INFINITY = 1e10;
static const int PawnWeight = 100;
static const int KnightWeight = 200;
static const int BishopWeight = 400;
static const int RookWeight = 550;
static const int QueenWeight = 950;
static const int KingWeight = 1950;

Engine::Engine()
{
  srand((unsigned int)time(NULL));
}

int Engine::evaluatePosition(Board * board)
{
  return 0;

//  int materialScore = 0;
//  for (uint row = 0; row < 8; row++) {
//    for (uint col = 0; col < 8; col++) {
//      PieceType pieceType = board->getPieceType(row, col);
//      switch (pieceType) {
//      case WhitePawn:
//        materialScore += PawnWeight;
//        break;
//      case WhiteKnight:
//        materialScore += KnightWeight;
//        break;
//      case WhiteBishop:
//        materialScore += BishopWeight;
//        break;
//      case WhiteRook:
//        materialScore += RookWeight;
//        break;
//      case WhiteQueen:
//        materialScore += QueenWeight;
//        break;
//      case WhiteKing:
//        materialScore += KingWeight;
//        break;
//      case BlackPawn:
//        materialScore -= PawnWeight;
//        break;
//      case BlackKnight:
//        materialScore -= KnightWeight;
//        break;
//      case BlackBishop:
//        materialScore -= BishopWeight;
//        break;
//      case BlackRook:
//        materialScore -= RookWeight;
//        break;
//      case BlackQueen:
//        materialScore -= QueenWeight;
//        break;
//      case BlackKing:
//        materialScore -= KingWeight;
//        break;
//      default:
//        break;
//      }
//    }
//  }

//  return (board->isWhiteToMove() ? materialScore : -materialScore);
}

bool Engine::getMove(Board * board, Move & newMove)
{
//  MoveList moveList;
//  uint totalMoves = board->generateLegalMoves(moveList);
//  if (totalMoves == 0)
//    return false;

//  int maxScore = -INFINITY;
//  for (uint i = 0; i < totalMoves; i++) {
//    Move move(moveList[i]);
//    board->makeMove(move);
//    int score = negaMax(board, 3);
//    if (score > maxScore) {
//      maxScore = score;
//      newMove = move;
//    }
//    board->unmakeMove(move);
//  }

  return true;
}

int Engine::negaMax(Board * board, int depth)
{
  return 0;

//  if (depth == 0)
//    return evaluatePosition(board);

//  int maxScore = -INFINITY;

//  MoveList moveList;
//  uint totalMoves = board->generateLegalMoves(moveList);
//  for (uint i = 0; i < totalMoves; i++) {
//    int score = -negaMax(board, depth-1);
//    if (score > maxScore)
//      maxScore = score;
//  }
//  return maxScore;
}

