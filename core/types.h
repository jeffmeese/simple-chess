#ifndef TYPES_H
#define TYPES_H

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ulonglong;

enum Color
{
  ColorWhite = 0,
  ColorBlack,
  ColorEmpty
};

enum Piece
{
  King = 0,
  Queen,
  Rook,
  Bishop,
  Knight,
  Pawn,
  Empty
};

enum Castling
{
  CastleWhiteKing = 0x0001,
  CastleWhiteQueen = 0x0002,
  CastleBlackKing = 0x0004,
  CastleBlackQueen = 0x0008
};

#endif // TYPES_H
