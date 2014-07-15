#include "boardview.h"
#include "imagemap.h"
#include "guigame.h"

#include <QPaintEvent>
#include <QPainter>
#include <QRect>
#include <QDebug>
#include <QMessageBox>

BoardView::BoardView()
  : mGame(NULL)
  , mImageMap(NULL)
{
//  mPieceSelected = false;
//  mShowValidMoves = true;
//  mSelectedRow = mSelectedCol = -1;

//  mWhiteCellColor.setRgb(243, 213, 168);
//  mBlackCellColor.setRgb(196, 153, 87);
//  mSelectedCellColor.setRgb(0, 255, 255);
//  mValidMoveCellColor.setRgb(0, 194, 0);
//  mAttackedCellColor.setRgb(255, 0, 0);
//  mEnPassantCellColor.setRgb(127, 0, 127);
}

Game * BoardView::game() const
{
  return mGame;
}

ImageMap * BoardView::imageMap() const
{
  return mImageMap;
}

void BoardView::deselectPiece()
{
  //mPieceSelected = false;
  //mSelectedRow = mSelectedCol = -1;

  //mMoveList.clearMoves();
  //update();
}

void BoardView::drawBoard(QPainter &painter, const QRect & boardRect)
{
//  // Draw the board outline
//  QPen outlinePen(Qt::black);
//  outlinePen.setWidth(3);
//  painter.setPen(outlinePen);
//  painter.drawRect(boardRect);

//  // Draw the board cells
//  QRect cellsRect(boardRect);
//  cellsRect.adjust(2, -2, -2, 2);
//  int cellWidth = cellsRect.width() / 8;
//  int cellHeight = cellsRect.height() / 8;

//  bool whiteSquare = false;
//  for (uint row = 0; row < 8; row++) {
//    for (uint col = 0; col < 8; col++) {
//      QRect cellRect;
//      getCellRect(row, col, cellWidth, cellHeight, boardRect, cellRect);

//      // Determine the cell color
//      QColor cellColor = whiteSquare ? mWhiteCellColor : mBlackCellColor;
//      if (row == mSelectedRow && col == mSelectedCol)
//        cellColor = mSelectedCellColor;

//      // Draw the cell
//      QBrush cellBrush(cellColor);
//      painter.fillRect(cellRect, cellBrush);

//      // Draw the cell outline
//      QColor borderColor(0,0,0);
//      QPen borderPen(borderColor);
//      //borderPen.setWidth(2);
//      painter.setPen(borderPen);
//      painter.drawRect(cellRect);

//      whiteSquare = !whiteSquare;
//    }
//    whiteSquare = !whiteSquare;
//  }

//  if (mShowValidMoves) {
//    for (uint i = 0; i < mMoveList.totalMoves(); i++) {
//      Move currentMove(mMoveList.moveAt(i));
//      uint destRow = currentMove.destinationRow();
//      uint destCol = currentMove.destinationColumn();

//      QRect cellRect;
//      getCellRect(destRow, destCol, cellWidth, cellHeight, boardRect, cellRect);

//      QColor cellColor = mValidMoveCellColor;
//      if (currentMove.isCapture())
//        cellColor = mAttackedCellColor;

//      if (currentMove.isEnPassant())
//        cellColor = mEnPassantCellColor;

//      QBrush cellBrush(cellColor);
//      painter.fillRect(cellRect, cellBrush);

//      // Draw the cell outline
//      QColor borderColor(0,0,0);
//      painter.setPen(borderColor);
//      painter.drawRect(cellRect);
//    }
//  }
}

void BoardView::drawPieces(QPainter & painter, const QRect &boardRect)
{
//  if (mGame == NULL || mImageMap == NULL)
//    return;

//  int cellWidth = boardRect.width() / 8;
//  int cellHeight = boardRect.height() / 8;

//  for (uint row = 0; row < 8; row++) {
//    for (uint col = 0; col < 8; col++) {
//      QRect cellRect;
//      getCellRect(row, col, cellWidth, cellHeight, boardRect, cellRect);

//      PieceType pieceType = mGame->getPieceType(row, col);
//      if (isPiece(pieceType)) {
//        QImage * pieceImage = mImageMap->getImage(pieceType);
//        painter.drawImage(cellRect, *pieceImage);
//      }
//    }
//  }
}

void BoardView::getCellRect(uint row, uint col, int width, int height, const QRect & boardRect, QRect & cellRect) const
{
//  int left = boardRect.left() + (col*width);
//  int top = (boardRect.bottom() - (row*height)) - height;
//  //if (mIsFlipped)
//  //  top = boardRect.top() + (row*cellHeight);
//  cellRect.setRect(left, top, width, height);
}

PieceType BoardView::getPieceAt(const QPoint & point, int & selectedRow, int & selectedCol) const
{
//  QRect boardRect = rect();
//  int cellWidth = boardRect.width() / 8;
//  int cellHeight = boardRect.height() / 8;

//  for (u_int row = 0; row < 8; row++)
//  {
//    for (u_int col = 0; col < 8; col++)
//    {
//      QRect cellRect;
//      getCellRect(row, col, cellWidth, cellHeight, boardRect, cellRect);
//      if (cellRect.contains(mapFromGlobal(point)))
//      {
//        selectedRow = row;
//        selectedCol = col;
//        return mGame->getPieceType(row,col);
//      }
//    }
//  }

  return NoPiece;
}

bool BoardView::isCurrentMove(uint row, uint col)
{
//  for (uint i = 0; i < mMoveList.totalMoves(); i++) {
//    Move currentMove(mMoveList.moveAt(i));
//    uint destRow = currentMove.destinationRow();
//    uint destCol = currentMove.destinationColumn();
//    if (row == destRow && col == destCol)
//      return true;
//  }

  return false;
}

void BoardView::mousePressEvent(QMouseEvent *mouseEvent)
{
  //qDebug() << QObject::tr(__FUNCTION__);

//  if (mGame == NULL)
//    return;

//  if (mouseEvent->button() != Qt::LeftButton)
//    return;

//  QPoint mousePos = mouseEvent->globalPos();
//  int row = -1, col = -1;
//  PieceType pieceType = getPieceAt(mousePos, row, col);
//  if (row == -1 || col == -1)
//  {
//    deselectPiece();
//    return;
//  }

//  bool whiteMove = mGame->isWhiteToMove();
//  bool whitePiece = isWhitePiece(pieceType);

//  if (mPieceSelected)
//  {
//    if (row == mSelectedRow && col == mSelectedCol)
//    {
//      deselectPiece();
//      return;
//    }
//    else if (isCurrentMove(row, col))
//    {
//      performMove(mSelectedRow, mSelectedCol, row, col);
//      return;
//    }
//    else if ( (whiteMove && whitePiece) || (!whiteMove && !whitePiece) )
//    {
//      deselectPiece();
//      selectPiece(row, col);
//    }
//    else
//    {
//      deselectPiece();
//      QMessageBox::warning(this, QObject::tr("Invalid Move"), QObject::tr("You have selected an invalid move"));
//      return;
//    }
//  }
//  else
//  {
//    if (pieceType == NoPiece)
//      return;

//    if ( (whiteMove && whitePiece) || (!whiteMove && !whitePiece))
//    {
//      selectPiece(row, col);
//      return;
//    }

//    if (whiteMove && !whitePiece)
//    {
//      QMessageBox::warning(this, QObject::tr("Invalid Move"), QObject::tr("White to Move"));
//      return;
//    }

//    if (!whiteMove && whitePiece)
//    {
//      QMessageBox::warning(this, QObject::tr("Invalid Move"), QObject::tr("Black to Move"));
//      return;
//    }
//  }
}

void BoardView::paintEvent(QPaintEvent *paintEvent)
{
  QPainter painter(this);

  if (mGame == NULL || mImageMap == NULL)
    return;

  QRect windowRect = paintEvent->rect();
  windowRect.adjust(6,6,-6,-6);

  drawBoard(painter, windowRect);
  drawPieces(painter, windowRect);
}

bool BoardView::performMove(uint sourceRow, uint sourceCol, uint destRow, uint destCol)
{
//  for (uint i = 0; i < mMoveList.totalMoves(); i++) {
//    Move currentMove(mMoveList[i]);
//    u_int moveSourceRow = currentMove.sourceRow();
//    u_int moveSourceCol = currentMove.sourceColumn();
//    u_int moveDestRow = currentMove.destinationRow();
//    u_int moveDestCol = currentMove.destinationColumn();
//    if (moveSourceRow == sourceRow && moveSourceCol == sourceCol && moveDestRow == destRow && moveDestCol == destCol) {
//      //if (currentMove.isPromotion())
//      //{
//        //if (currentMove.isCapturePromotion())
//        //	currentMove.setQueenPromotionCapture();
//        //	currentMove.setBishopPromotionCapture();
//        //else
//        //	currentMove.setBishopPromotion();
//          //currentMove.setQueenPromotion();
//      //}

//      /*if (currentMove.isPromotion())
//      {
//        PromotionDialog promotionDialog(this);
//        int dlgResult = promotionDialog.exec();
//        if (dlgResult == QDialog::Rejected)
//          return false;

//        bool whiteMove = mChessGame->WhiteToMove();
//        PieceType pieceType = NoPiece;

//        if (promotionDialog.isQueen())
//          pieceType = (whiteMove) ? WhiteQueen : BlackQueen;

//        if (promotionDialog.isRook())
//          pieceType = (whiteMove) ? WhiteRook : BlackRook;

//        if (promotionDialog.isBishop())
//          pieceType = (whiteMove) ? WhiteBishop : BlackBishop;

//        if (promotionDialog.isKnight())
//          pieceType = (whiteMove) ? WhiteKnight : BlackKnight;

//        movePtr->SetPromotedPiece(pieceType);
//      }*/

//      deselectPiece();
//      mGame->doMove(currentMove);
//      emit moveCompleted(currentMove);

//      return true;
//    }
//  }
  return false;
}

void BoardView::selectPiece(uint row, uint col)
{
//  mMoveList.clearMoves();
//  mGame->generateMoves(row, col, mMoveList);
//  //mBoard->generateLegalMoves(row, col, mMoveList);

//  mPieceSelected = true;
//  mSelectedRow = row;
//  mSelectedCol = col;

//  update();
}

void BoardView::setGame(Game *game)
{
  mGame = game;
}

void BoardView::setImageMap(ImageMap *imageMap)
{
  mImageMap = imageMap;
  repaint();
}
