#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QWidget>
#include <QColor>
#include <QPainter>

#include <core/movelist.h>
#include <core/types.h>

#include <common/board.h>

class ImageMap;
class Game;

class BoardView
    : public QWidget
{
  Q_OBJECT

public:
  BoardView();

public:
  Game * game() const;
  ImageMap * imageMap() const;
  void setGame(Game * game);
  void setImageMap(ImageMap * imageMap);

protected:
  virtual void mousePressEvent(QMouseEvent * mouseEvent);
  virtual void paintEvent(QPaintEvent * paintEvent);

private:
  void deselectPiece();
  PieceType getPieceAt(const QPoint & point, int & selectedRow, int & selectedCol) const;
  void drawBoard(QPainter & painter, const QRect & boardRect);
  void drawPieces(QPainter & painter, const QRect & boardRect);
  void getCellRect(uchar row, uchar col, int width, int height, const QRect & boardRect, QRect & cellRect) const;
  bool isValidDestination(uchar row, uchar col);
  bool movePiece(uchar sourceRow, uchar sourceCol, uchar destRow, uchar destCol);
  void selectPiece(uchar row, uchar col);
  Piece showPromotionDialog();

Q_SIGNALS:
  void moveCompleted(const Move & newMove);
  //void moveCompleted(const sMove & newMove);

private:
  bool mPieceSelected;
  bool mShowValidMoves;
  uint mSelectedRow;
  uint mSelectedCol;
  MoveList mMoveList;
  //sMove mMoveList[256];
  //uchar mTotalMoves;
  QColor mWhiteCellColor;
  QColor mBlackCellColor;
  QColor mSelectedCellColor;
  QColor mValidMoveCellColor;
  QColor mAttackedCellColor;
  QColor mEnPassantCellColor;
  Game * mGame;
  ImageMap * mImageMap;
};

#endif // BOARDVIEW_H
