#ifndef BOARD0X88_H
#define BOARD0X88_H

#include "types.h"
#include "stalematecondition.h"
#include "array2d.h"

#include <string>
#include <vector>
#include <list>

#define USEMACROS
#ifdef USEMACROS
#define getIndex(row,col) ((row)*16+(col))
#define getRow(index) ( (index >> 4) )
#define getCol(index) ( (index & 7) )
#define isValidSquare(x)  ( (x) & 0x88 ) ? (0) : (1)
#endif // #ifdef USEMACROS

class Move;
class MoveList;

class Board0x88
{
public:
  Board0x88(void);
  ~Board0x88(void);

public:
  bool canBlackCastleQueenSide() const;
  bool canBlackCastleKingSide() const;
  bool canWhiteCastleQueenSide() const;
  bool canWhiteCastleKingSide() const;
  int enPassantCol() const;
  int enPassantRow() const;
  int fullMoveCounter() const;
  int halfMoveClock() const;
  bool isWhiteToMove() const;
  char sideToMove() const;

public:
  uchar generateMoves(MoveList & moveList);
  uchar generateOpponentMoves(MoveList & moveList);
  uchar generateMoves(uchar row, uchar col, MoveList & moveList);
  bool isKingAttacked(char kingColor) const;
  bool isCellAttacked(uchar row, uchar col, char attackingColor) const;
  uchar kingRow(char color) const;
  uchar kingCol(char color) const;
  void makeMove(const Move & newMove);
  PieceType pieceType(uchar row, uchar col) const;
  bool setPosition(const std::string & fenString);
  void unmakeMove(const Move & undoMove);

private:
  void generateCastlingMoves(MoveList & moveList);
  void generatePawnCaptures(uchar index, MoveList & moveList);
  void generatePawnMoves(uchar index, MoveList & moveList);
  void initAttacks();
  void initBoard();
  void initMoves();
  bool isCellAttacked(uchar index, char attackingColor) const;
  uchar kingIndex(char color) const;
  void pushMove(uchar fromSquare, uchar toSquare, uchar pieceFrom, uchar pieceCapture, char flags, MoveList & moveList);

#ifndef USEMACROS
  uchar getCol(uchar index) const;
  uchar getIndex(uchar row, uchar col) const;
  uchar getRow(uchar index) const;
  bool isValidSquare(uchar index) const;
#endif // #ifndef USEMACROS

private:
  char mSideToMove;
  char mCastlingRights;
  char mEpIndex;
  uchar mHalfMoveClock;
  uchar mFullMoveCounter;
  uchar mKingIndex[2];
  uchar mPieces[128];
  uchar mColors[128];
  bool mSliders[5];
  uchar mNumDirections[5];
  uchar mNumKnightAttacks[128];
  uchar mNumKingAttacks[128];
  Array2D<uchar> mNumStraightAttacks;
  Array2D<uchar> mNumDiagAttacks;
  Array2D<uchar> mKnightAttacks;
  Array2D<uchar> mKingAttacks;
  Array2D<uchar> mStraightAttacks[4];
  Array2D<uchar> mDiagAttacks[4];
  Array2D<uchar> mDirectionVectors;
};

//////////////////////////////////////////////////////////////
/// Inline functions

inline bool Board0x88::canBlackCastleQueenSide() const
{
  return (mCastlingRights & CastleBlackQueen);
}

inline bool Board0x88::canBlackCastleKingSide() const
{
  return (mCastlingRights & CastleBlackKing);
}

inline bool Board0x88::canWhiteCastleQueenSide() const
{
  return (mCastlingRights & CastleWhiteQueen);
}

inline bool Board0x88::canWhiteCastleKingSide() const
{
  return (mCastlingRights & CastleWhiteKing);
}

inline int Board0x88::enPassantCol() const
{
  return ( (mEpIndex == 0x7f) ? -1 : getCol(mEpIndex) );
}

inline int Board0x88::enPassantRow() const
{
  return ( (mEpIndex == 0x7f) ? -1 : getRow(mEpIndex) );
}

inline int Board0x88::fullMoveCounter() const
{
  return mFullMoveCounter;
}

#ifndef USEMACROS
inline uchar Board0x88::getCol(uchar index) const
{
  return (index & 7);
}

inline uchar Board0x88::getIndex(uchar row, uchar col) const
{
  return (row * 16 + col);
}

inline uchar Board0x88::getRow(uchar index) const
{
  return (index >> 4);
}

inline bool Board0x88::isValidSquare(uchar index) const
{
  return !(index & 0x88);
}
#endif // #ifndef USEMACROS

inline int Board0x88::halfMoveClock() const
{
  return mHalfMoveClock;
}

inline bool Board0x88::isCellAttacked(uchar row, uchar col, char attackingColor) const
{
  return isCellAttacked(getIndex(row, col), attackingColor);
}

inline bool Board0x88::isKingAttacked(char kingColor) const
{
  return isCellAttacked(kingIndex(kingColor), !kingColor);
}

inline uchar Board0x88::kingRow(char color) const
{
  return getRow(kingIndex(color));
}

inline uchar Board0x88::kingIndex(char color) const
{
  return mKingIndex[color];
}

inline uchar Board0x88::kingCol(char color) const
{
  return getCol(kingIndex(color));
}

inline bool Board0x88::isWhiteToMove() const
{
  return (mSideToMove == White);
}

inline char Board0x88::sideToMove() const
{
  return mSideToMove;
}

#endif // #ifndef BOARD0X88_H
