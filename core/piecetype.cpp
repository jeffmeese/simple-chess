#include "piecetype.h"

bool isPiece(PieceType pieceType)
{
  return (pieceType > 0);
}

bool isWhitePiece(PieceType pieceType)
{
  return ( (pieceType > 0) && (pieceType < 0x07) );
}

bool isBlackPiece(PieceType pieceType)
{
  return ( (pieceType > 0x07) && (pieceType < 0x0E) );
}

bool isFriendlyPiece(PieceType pieceType, PieceType otherPiece)
{
  return ( (isPiece(pieceType)      && isPiece(otherPiece))      &&
           (isWhitePiece(pieceType) && isWhitePiece(otherPiece)) ||
           (isBlackPiece(pieceType) && isBlackPiece(otherPiece))
         );
}

bool isEnemyPiece(PieceType pieceType, PieceType otherPiece)
{
  return ( (isPiece(pieceType)      && isPiece(otherPiece))      &&
           (isWhitePiece(pieceType) && isBlackPiece(otherPiece)) ||
           (isBlackPiece(pieceType) && isWhitePiece(otherPiece))
         );
}
