#include "imagemap.h"

ImageMap::ImageMap(void)
  : mBlackPawnImage(QObject::tr(":/images/black-pawn.png"))
  , mBlackKnightImage(QObject::tr(":/images/black-knight.png"))
  , mBlackBishopImage(QObject::tr(":/images/black-bishop.png"))
  , mBlackRookImage(QObject::tr(":/images/black-rook.png"))
  , mBlackQueenImage(QObject::tr(":/images/black-queen.png"))
  , mBlackKingImage(QObject::tr(":/images/black-king.png"))
  , mWhitePawnImage(QObject::tr(":/images/white-pawn.png"))
  , mWhiteKnightImage(QObject::tr(":/images/white-knight.png"))
  , mWhiteBishopImage(QObject::tr(":/images/white-bishop.png"))
  , mWhiteRookImage(QObject::tr(":/images/white-rook.png"))
  , mWhiteQueenImage(QObject::tr(":/images/white-queen.png"))
  , mWhiteKingImage(QObject::tr(":/images/white-king.png"))
{
  mPieceMap.insert(std::make_pair(BlackPawn, &mBlackPawnImage));
  mPieceMap.insert(std::make_pair(BlackKnight, &mBlackKnightImage));
  mPieceMap.insert(std::make_pair(BlackBishop, &mBlackBishopImage));
  mPieceMap.insert(std::make_pair(BlackRook, &mBlackRookImage));
  mPieceMap.insert(std::make_pair(BlackQueen, &mBlackQueenImage));
  mPieceMap.insert(std::make_pair(BlackKing, &mBlackKingImage));

  mPieceMap.insert(std::make_pair(WhitePawn, &mWhitePawnImage));
  mPieceMap.insert(std::make_pair(WhiteKnight, &mWhiteKnightImage));
  mPieceMap.insert(std::make_pair(WhiteBishop, &mWhiteBishopImage));
  mPieceMap.insert(std::make_pair(WhiteRook, &mWhiteRookImage));
  mPieceMap.insert(std::make_pair(WhiteQueen, &mWhiteQueenImage));
  mPieceMap.insert(std::make_pair(WhiteKing, &mWhiteKingImage));
}

ImageMap::~ImageMap(void)
{
}

QImage * ImageMap::getImage(PieceType pieceType) const
{
  PieceMap::const_iterator itr = mPieceMap.find(pieceType);
  if (itr == mPieceMap.end())
    return NULL;

  return itr->second;
}
