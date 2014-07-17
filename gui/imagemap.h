#ifndef IMAGE_MAP_H
#define IMAGE_MAP_H

#include <QImage>
#include <QObject>
#include <map>

#include <core/types.h>

class ImageMap
    : public QObject
{
public:
  ImageMap(void);
  virtual ~ImageMap(void);

  QImage * getImage(PieceType pieceType) const;

private:
  typedef std::map<PieceType, QImage*> PieceMap;
  typedef PieceMap::iterator PieceItr;

private:
  QImage mBlackPawnImage;
  QImage mBlackKnightImage;
  QImage mBlackBishopImage;
  QImage mBlackRookImage;
  QImage mBlackQueenImage;
  QImage mBlackKingImage;
  QImage mWhitePawnImage;
  QImage mWhiteKnightImage;
  QImage mWhiteBishopImage;
  QImage mWhiteRookImage;
  QImage mWhiteQueenImage;
  QImage mWhiteKingImage;
  PieceMap mPieceMap;
};

#endif // #ifndef IMAGE_MAP_H
