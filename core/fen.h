#ifndef FENPARSER_H
#define FENPARSER_H

#include "types.h"

#include <string>

struct FenData
{
  bool mBlackCastleKingSide;
  bool mBlackCastleQueenSide;
  bool mWhiteCastleKingSide;
  bool mWhiteCastleQueenSide;
  bool mWhiteToMove;
  uint mEnPassantCol;
  uint mHalfMoveClock;
  uint mFullMoveCounter;
  PieceType mPieceType[8][8];
};

bool readFenString(const std::string & fenString, FenData & fenData);

#endif // FENPARSER_H
