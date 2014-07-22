#ifndef MOVE_H
#define MOVE_H

#include "types.h"

#include <string>

struct Move
{
  uchar mSourceRow;
  uchar mSourceCol;
  uchar mDestRow;
  uchar mDestCol;
  uchar mFromPiece;
  uchar mToPiece;
  uchar mCapturePiece;
  uchar mHalfMoveClock;
  char mEnPassantCol;
  char mCastlingRights;
  char mFlags;
};

std::string getSmithNotation(const Move & move);

#endif // #ifndef MOVE_H
