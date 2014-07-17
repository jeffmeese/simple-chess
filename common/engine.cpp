#include "engine.h"

#include <core/movelist.h>

#include <iostream>

static const int INFINITY = 1e10;
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
  int materialScore = 0;
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
  if ( !board->isWhiteToMove())
    materialScore = -materialScore;

  //std::cout << "material score: " << materialScore << "\n";
  return materialScore;
}

bool Engine::getMove(Board * board, Move & selectedMove)
{
  MoveList moveList;
  uchar totalMoves = board->generateMoves(moveList);

  std::cout << "\nEngine Move\n";
  int maxScore = -INFINITY;
  for (uchar i = 0; i < totalMoves; i++) {
    Move currentMove = moveList[i];
    board->makeMove(currentMove);
    if (!board->isKingAttacked(!board->sideToMove())) {
      int score = -negaMax(board, 4);
      if (score > maxScore) {
        maxScore = score;
        selectedMove = currentMove;
      }
      std::cout << "move: " << getSmithNotation(currentMove) << " score: " << score << " max: " << maxScore << "\n";
    }
    board->unmakeMove(currentMove);
  }
  return true;
}

int Engine::negaMax(Board * board, int depth)
{
  if (depth == 0)
    return evaluatePosition(board);

  MoveList moveList;
  uchar totalMoves = board->generateMoves(moveList);

  int maxScore = -INFINITY;
  for (uchar i = 0; i < totalMoves; i++) {
    Move currentMove = moveList[i];
    board->makeMove(currentMove);
    if (!board->isKingAttacked(!board->sideToMove())) {
      int score = -negaMax(board, depth-1);
      if (score > maxScore)
        maxScore = score;
    }
    board->unmakeMove(currentMove);
  }
  return maxScore;
}

