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

enum PieceType
{
  NoPiece = 0,
  WhitePawn = 0x01,
  WhiteKnight = 0x02,
  WhiteBishop = 0x03,
  WhiteRook = 0x04,
  WhiteQueen = 0x05,
  WhiteKing = 0x06,
  BlackPawn = 0x08,
  BlackKnight = 0x09,
  BlackBishop = 0x0A,
  BlackRook = 0x0B,
  BlackQueen = 0x0C,
  BlackKing = 0x0D
};

enum Castling
{
  CastleWhiteKing = 0x0001,
  CastleWhiteQueen = 0x0002,
  CastleBlackKing = 0x0004,
  CastleBlackQueen = 0x0008
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

inline bool isPiece(PieceType pieceType)
{ return pieceType > 0; }

inline bool isWhitePiece(PieceType pieceType)
{ return ( (pieceType > 0) && (pieceType < 0x07) ); }

inline bool isBlackPiece(PieceType pieceType)
{ return ( (pieceType > 0x07) && (pieceType < 0x0E) ); }

#endif // TYPES_H
