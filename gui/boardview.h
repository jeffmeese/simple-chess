#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QWidget>
#include <QColor>
#include <QPainter>

#include <core/movelist.h>
#include <core/types.h>

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
  void getCellRect(uint row, uint col, int width, int height, const QRect & boardRect, QRect & cellRect) const;
  bool isCurrentMove(uint row, uint col);
  bool performMove(uint sourceRow, uint sourceCol, uint destRow, uint destCol);
  void selectPiece(uint row, uint col);

Q_SIGNALS:
  void moveCompleted(const Move & newMove);

private:
  bool mPieceSelected;
  bool mShowValidMoves;
  uint mSelectedRow;
  uint mSelectedCol;
  MoveList mMoveList;
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
