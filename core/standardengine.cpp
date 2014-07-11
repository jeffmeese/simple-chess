//#include "standardengine.h"
//#include "abstractboard.h"

//#include <cmath>

////static const int INFINITY = 1e10;
//static const int PawnWeight = 100;
//static const int KnightWeight = 200;
//static const int BishopWeight = 400;
//static const int RookWeight = 550;
//static const int QueenWeight = 950;
//static const int KingWeight = 1950;

//StandardEngine::StandardEngine(AbstractBoard * chessBoard)
//  : mBoard(chessBoard)
//{
//  srand((unsigned int)time(NULL));
//}

//AbstractBoard * StandardEngine::board() const
//{
//  return mBoard;
//}

//int StandardEngine::evaluatePosition()
//{
//  int materialScore = 0;
//  for (uint row = 0; row < 8; row++) {
//    for (uint col = 0; col < 8; col++) {
//      PieceType pieceType = mBoard->getPieceType(row, col);
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

//  return (mBoard->isWhiteToMove() ? materialScore : -materialScore);
//}

//bool StandardEngine::getMove(Move & newMove)
//{
//  MoveList moveList;
//  uint totalMoves = mBoard->generateLegalMoves(moveList);
//  if (totalMoves == 0)
//    return false;

//  int maxScore = -INFINITY;
//  for (uint i = 0; i < totalMoves; i++) {
//    Move move(moveList[i]);
//    mBoard->makeMove(move);
//    int score = negaMax(3);
//    if (score > maxScore) {
//      maxScore = score;
//      newMove = move;
//    }
//    mBoard->unmakeMove(move);
//  }

//  return true;
//}

//int StandardEngine::negaMax(int depth)
//{
//  if (depth == 0)
//    return evaluatePosition();

//  int maxScore = -INFINITY;

//  MoveList moveList;
//  uint totalMoves = mBoard->generateLegalMoves(moveList);
//  for (uint i = 0; i < totalMoves; i++) {
//    int score = -negaMax(depth-1);
//    if (score > maxScore)
//      maxScore = score;
//  }
//  return maxScore;
//}

//void StandardEngine::setBoard(AbstractBoard * chessBoard)
//{
//  mBoard = chessBoard;
//}

