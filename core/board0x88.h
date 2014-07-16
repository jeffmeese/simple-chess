#ifndef BOARD0X88_H
#define BOARD0X88_H

#include "types.h"
#include "move.h"
#include "piecetype.h"
#include "stalematecondition.h"
#include "array2d.h"

#include <string>
#include <vector>
#include <list>

//#define USEMACROS
#ifdef USEMACROS
#define getIndex(row,col) ((row)*16+(col))
#define getRow(index) ( (index >> 4) )
#define getCol(index) ( (index & 7) )
#define isValidSquare(x)  ( (x) & 0x88 ) ? (0) : (1)
#endif // #ifdef USEMACROS

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
  uchar sideToMove() const;

public:
  uchar generateMoves(sMove * moveList);
  uchar generateMoves(uchar row, uchar col, sMove * moveList);
  std::string getSmithNotation(const sMove & move) const;
  uchar getMoveDestinationCol(const sMove & move) const;
  uchar getMoveDestinationRow(const sMove & move) const;
  uchar getMoveSourceCol(const sMove & move) const;
  uchar getMoveSourceRow(const sMove & move) const;
  bool isKingAttacked(uchar kingColor) const;
  bool isCellAttacked(uchar row, uchar col, uchar attackingColor) const;
  uchar kingRow(uchar color) const;
  uchar kingCol(uchar color) const;
  void makeMove(const sMove & newMove);
  bool setPosition(const std::string & fenString);
  void unmakeMove(const sMove & undoMove);
  PieceType pieceType(uchar row, uchar col) const;

private:
  void generateCastlingMoves();
  void generatePawnCaptures(uchar index);
  void generatePawnMoves(uchar index);
  void initAttacks();
  void initBoard();
  bool isCellAttacked(uchar index, uchar attackingColor) const;
  uchar kingIndex(uchar color) const;
  void pushMove(uchar fromSquare, uchar toSquare, uchar pieceFrom, uchar pieceCapture, uchar flags);

#ifndef USEMACROS
  uchar getCol(uchar index) const;
  uchar getIndex(uchar row, uchar col) const;
  uchar getRow(uchar index) const;
  bool isValidSquare(uchar index) const;
#endif // #ifndef USEMACROS

private:
  uchar mSideToMove;
  uchar mCastlingRights;
  uchar mHalfMoveClock;
  uchar mFullMoveCounter;
  uchar mEpIndex;
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
  mutable uchar mMoveCount;
  mutable sMove * mMoves;
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

inline uchar Board0x88::getMoveDestinationCol(const sMove & move) const
{
  return getCol(move.toSquare);
}

inline uchar Board0x88::getMoveDestinationRow(const sMove & move) const
{
  return getRow(move.toSquare);
}

inline uchar Board0x88::getMoveSourceCol(const sMove & move) const
{
  return getCol(move.fromSquare);
}

inline uchar Board0x88::getMoveSourceRow(const sMove & move) const
{
  return getRow(move.fromSquare);
}

inline int Board0x88::halfMoveClock() const
{
  return mHalfMoveClock;
}

inline bool Board0x88::isCellAttacked(uchar row, uchar col, uchar attackingColor) const
{
  return isCellAttacked(getIndex(row, col), attackingColor);
}

inline bool Board0x88::isKingAttacked(uchar kingColor) const
{
  return isCellAttacked(kingIndex(kingColor), !kingColor);
}

inline uchar Board0x88::kingRow(uchar color) const
{
  return getRow(kingIndex(color));
}

inline uchar Board0x88::kingIndex(uchar color) const
{
  return mKingIndex[color];
}

inline uchar Board0x88::kingCol(uchar color) const
{
  return getCol(kingIndex(color));
}

inline bool Board0x88::isWhiteToMove() const
{
  return (mSideToMove == White);
}

inline uchar Board0x88::sideToMove() const
{
  return mSideToMove;
}

#endif // #ifndef BOARD0X88_H
