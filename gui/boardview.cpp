#include "boardview.h"
#include "imagemap.h"
#include "guigame.h"

#include <QPaintEvent>
#include <QPainter>
#include <QRect>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QStringList>

BoardView::BoardView()
  : mGame(NULL)
  , mImageMap(NULL)
{
  mPieceSelected = false;
  mShowValidMoves = true;
  mSelectedRow = mSelectedCol = -1;
  //mTotalMoves = 0;

  mWhiteCellColor.setRgb(243, 213, 168);
  mBlackCellColor.setRgb(196, 153, 87);
  mSelectedCellColor.setRgb(0, 255, 255);
  mValidMoveCellColor.setRgb(0, 194, 0);
  mAttackedCellColor.setRgb(255, 0, 0);
  mEnPassantCellColor.setRgb(127, 0, 127);
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
  mPieceSelected = false;
  mSelectedRow = mSelectedCol = -1;
  mMoveList.clear();
  //mTotalMoves = 0;
  update();
}

void BoardView::drawBoard(QPainter &painter, int cellWidth, int cellHeight, const QRect & boardRect)
{
  // Draw the board outline
  QPen outlinePen(Qt::black);
  outlinePen.setWidth(3);
  //painter.setPen(outlinePen);
  //painter.drawRect(boardRect);

  // Draw the board cells
  //QRect cellsRect(boardRect);
  //cellsRect.adjust(2, -2, -2, 2);
  //int cellWidth = cellsRect.width() / 9;
  //int cellHeight = cellsRect.height() / 9;

  //boardRect.adjust(cellWidth, cellHeight, -cellWidth, -cellHeight);
  bool whiteSquare = false;
  for (uint row = 0; row < 8; row++) {
    for (uint col = 0; col < 8; col++) {
      QRect cellRect;
      getCellRect(row, col, cellWidth, cellHeight, boardRect, cellRect);

      // Determine the cell color
      QColor cellColor = whiteSquare ? mWhiteCellColor : mBlackCellColor;
      if (row == mSelectedRow && col == mSelectedCol)
        cellColor = mSelectedCellColor;

      // Draw the cell
      QBrush cellBrush(cellColor);
      painter.fillRect(cellRect, cellBrush);

      // Draw the cell outline
      QColor borderColor(0,0,0);
      QPen borderPen(borderColor);
      //borderPen.setWidth(2);
      painter.setPen(borderPen);
      painter.drawRect(cellRect);

      whiteSquare = !whiteSquare;
    }
    whiteSquare = !whiteSquare;
  }

  if (mShowValidMoves) {
    for (uchar i = 0; i < mMoveList.size(); i++) {
      //sMove move = mMoveList[i];
      //uchar destRow = mGame->board()->getMoveDestinationRow(move);
      //uchar destCol = mGame->board()->getMoveDestinationCol(move);
      Move move = mMoveList[i];
      uchar destRow = move.destRow;
      uchar destCol = move.destCol;

      QRect cellRect;
      getCellRect(destRow, destCol, cellWidth, cellHeight, boardRect, cellRect);

      QColor cellColor = mValidMoveCellColor;
      if (move.flags & MoveCapture)
        cellColor = mAttackedCellColor;

      if (move.flags & MoveEpCapture)
        cellColor = mEnPassantCellColor;

      QBrush cellBrush(cellColor);
      painter.fillRect(cellRect, cellBrush);

      // Draw the cell outline
      QColor borderColor(0,0,0);
      painter.setPen(borderColor);
      painter.drawRect(cellRect);
    }
  }
}

void BoardView::drawCoordinates(QPainter & painter, int cellWidth, int cellHeight) const
{
  QFont font;
  font.setPointSize(24);
  font.setBold(true);
  painter.setFont(font);

  for (int i = 0; i < 8; i++) {
    int left = 0;
    int top = i*cellHeight;
    QRect textRect(left, top, cellWidth, cellHeight);
    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignCenter, QString::number(8-i) );
  }

  static const QChar letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  for (int i = 0; i < 8; i++) {
    int left = (i+1)*cellWidth;
    int top = 8*cellHeight;
    QRect textRect(left, top, cellWidth, cellHeight);
    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignCenter, QString(letters[i]) );
  }
}

void BoardView::drawPieces(QPainter & painter, int cellWidth, int cellHeight, const QRect &boardRect)
{
  if (mGame == NULL || mImageMap == NULL)
    return;

  //int cellWidth = boardRect.width() / 9;
  //int cellHeight = boardRect.height() / 9;

  for (uint row = 0; row < 8; row++) {
    for (uint col = 0; col < 8; col++) {
      QRect cellRect;
      getCellRect(row, col, cellWidth, cellHeight, boardRect, cellRect);

      PieceType pieceType = mGame->board()->pieceType(row, col);
      if (isPiece(pieceType)) {
        QImage * pieceImage = mImageMap->getImage(pieceType);
        painter.drawImage(cellRect, *pieceImage);
      }
    }
  }
}

void BoardView::getCellRect(uchar row, uchar col, int width, int height, const QRect & boardRect, QRect & cellRect) const
{
  int left = boardRect.left() + (col*width);
  int top = (boardRect.bottom() - (row*height)) - height;
  //if (mIsFlipped)
  //  top = boardRect.top() + (row*cellHeight);
  cellRect.setRect(left, top, width, height);
}

PieceType BoardView::getPieceAt(const QPoint & point, int & selectedRow, int & selectedCol) const
{
  QRect boardRect = rect();
  int cellWidth = boardRect.width() / 9;
  int cellHeight = boardRect.height() / 9;
  boardRect.adjust(cellWidth, cellHeight, -cellWidth, -cellHeight);

  for (u_int row = 0; row < 8; row++) {
    for (u_int col = 0; col < 8; col++) {
      QRect cellRect;
      getCellRect(row, col, cellWidth, cellHeight, boardRect, cellRect);
      if (cellRect.contains(mapFromGlobal(point))) {
        selectedRow = row;
        selectedCol = col;
        return mGame->board()->pieceType(row,col);
      }
    }
  }

  return NoPiece;
}

bool BoardView::isValidDestination(uchar row, uchar col)
{
  for (uchar i = 0; i < mMoveList.size(); i++) {
//    sMove move = mMoveList[i];
//    uchar destRow = mGame->board()->getMoveDestinationRow(move);
//    uchar destCol = mGame->board()->getMoveDestinationCol(move);
    Move move = mMoveList[i];
    uchar destRow = move.destRow;
    uchar destCol = move.destCol;
    if (row == destRow && col == destCol)
      return true;
  }

  return false;
}

void BoardView::mousePressEvent(QMouseEvent *mouseEvent)
{
  //qDebug() << QObject::tr(__FUNCTION__);

  if (mGame == NULL)
    return;

  if (mouseEvent->button() != Qt::LeftButton)
    return;

  QPoint mousePos = mouseEvent->globalPos();
  int row = -1, col = -1;
  PieceType pieceType = getPieceAt(mousePos, row, col);
  if (row == -1 || col == -1) {
    deselectPiece();
    return;
  }

  bool whiteMove = (mGame->board()->sideToMove() == White);
  bool whitePiece = isWhitePiece(pieceType);

  if (mPieceSelected) {
    if (row == mSelectedRow && col == mSelectedCol) {
      deselectPiece();
      return;
    }
    else if (isValidDestination(row, col)) {
      movePiece(mSelectedRow, mSelectedCol, row, col);
      return;
    }
    else if ( (whiteMove && whitePiece) || (!whiteMove && !whitePiece) ) {
      deselectPiece();
      selectPiece(row, col);
    }
    else {
      deselectPiece();
      QMessageBox::warning(this, QObject::tr("Invalid Move"), QObject::tr("You have selected an invalid move"));
      return;
    }
  } else {
    if (pieceType == NoPiece)
      return;

    if ( (whiteMove && whitePiece) || (!whiteMove && !whitePiece)) {
      selectPiece(row, col);
      return;
    }

    if (whiteMove && !whitePiece) {
      QMessageBox::warning(this, QObject::tr("Invalid Move"), QObject::tr("White to Move"));
      return;
    }

    if (!whiteMove && whitePiece) {
      QMessageBox::warning(this, QObject::tr("Invalid Move"), QObject::tr("Black to Move"));
      return;
    }
  }
}

void BoardView::paintEvent(QPaintEvent *paintEvent)
{
  QPainter painter(this);

  if (mGame == NULL || mImageMap == NULL)
    return;

  QRect windowRect = paintEvent->rect();
  //windowRect.adjust(6,6,-6,-6);

  int cellWidth = windowRect.width() / 9;
  int cellHeight = windowRect.height() / 9;
  //windowRect.adjust(cellWidth, cellHeight, -cellWidth, -cellHeight);

  QRect boardRect(windowRect);
  boardRect.adjust(cellWidth, cellHeight, -cellWidth, -cellHeight);

  drawBoard(painter, cellWidth, cellHeight, boardRect);
  drawCoordinates(painter, cellWidth, cellHeight);
  drawPieces(painter, cellWidth, cellHeight, boardRect);
}

bool BoardView::movePiece(uchar sourceRow, uchar sourceCol, uchar destRow, uchar destCol)
{
  for (uint i = 0; i < mMoveList.size(); i++) {
    Move move = mMoveList[i];
    uchar moveSourceRow = move.sourceRow;
    uchar moveSourceCol = move.sourceCol;
    uchar moveDestRow = move.destRow;
    uchar moveDestCol = move.destCol;
    if (moveSourceRow == sourceRow && moveSourceCol == sourceCol && moveDestRow == destRow && moveDestCol == destCol) {

      if (move.flags & MovePromotion) {
        Piece piece = showPromotionDialog();
        if (piece == PieceEmpty)
          return false;

        move.toPiece = piece;
      }

      deselectPiece();
      mGame->doMove(move);
      emit moveCompleted(move);
      return true;
    }
  }

  return false;
}

void BoardView::selectPiece(uchar row, uchar col)
{
  mPieceSelected = true;
  mSelectedRow = row;
  mSelectedCol = col;
  mGame->generateMoves(row, col, mMoveList);
  update();
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

Piece BoardView::showPromotionDialog()
{
  Piece piece = PieceEmpty;

  QStringList promotedPieces;
  promotedPieces << "Queen";
  promotedPieces << "Rook";
  promotedPieces << "Bishop";
  promotedPieces << "Knight";

  QString promotedPiece = QInputDialog::getItem(this, "Piece Promotion", "Select Piece", promotedPieces, 0, false);
  if (promotedPiece.isEmpty())
    return piece;

  if (promotedPiece == "Queen")
    piece = Queen;
  else if (promotedPiece == "Rook")
    piece = Rook;
  else if (promotedPiece == "Bishop")
    piece = Bishop;
  else if (promotedPiece == "Knight")
    piece = Knight;

  return piece;
}
