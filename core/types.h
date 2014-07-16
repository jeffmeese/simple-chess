#ifndef TYPES_H
#define TYPES_H

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ulonglong;

enum Color
{
  White = 0,
  Black,
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
  PieceEmpty
};

enum Castling
{
  CastleWhiteKing = 0x0001,
  CastleWhiteQueen = 0x0002,
  CastleBlackKing = 0x0004,
  CastleBlackQueen = 0x0008
};

struct sMove
{
  uchar fromSquare;
  uchar toSquare;
  uchar halfMove;
  uchar epIndex;
  uchar fromPiece;
  uchar toPiece;
  uchar capturedPiece;
  uchar castlingRights;
  uchar flags;
};

enum MoveFlags
{
  MoveNormal = 0,
  MoveCapture = 1,
  MoveEpCapture = 2,
  MoveCastle = 4,
  MoveEp = 8,
  MovePromotion = 16,
  MoveNull = 32
};

#endif // TYPES_H
