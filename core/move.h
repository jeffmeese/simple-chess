#ifndef MOVE_H
#define MOVE_H

#include "types.h"

#include <string>

struct Move
{
  uchar sourceRow;
  uchar sourceCol;
  uchar destRow;
  uchar destCol;
  uchar fromPiece;
  uchar toPiece;
  uchar capturePiece;
  uchar halfMoveClock;
  char enPassantCol;
  char castlingRights;
  char flags;
};

std::string getSmithNotation(const Move & move);

#endif // #ifndef MOVE_H
