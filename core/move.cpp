#include "move.h"

#include <sstream>
#include <iostream>
#include <cassert>

Move::Move()
{
}

Move::Move(uint sourceRow, uint sourceCol, uint destRow, uint destCol, PieceType capturedPiece, PieceType promotedPiece)
{
  mMove = (sourceRow & 0x0007) | ((sourceCol & 0x0007) << 3) | ((destRow & 0x0007) << 6) | ((destCol & 0x0007) << 9);
  mMove |= ( (capturedPiece & 0x00ff) << 16);
  mMove |= ( (promotedPiece & 0x00ff) << 24);

  if (capturedPiece != NoPiece)
    setCapture();
}

Move::Move(const Move & otherMove)
  : mMove(otherMove.mMove)
{
}

Move & Move::operator=(const Move & otherMove)
{
  mMove = otherMove.mMove;
  return *this;
}

bool Move::isQuiet() const
{
  return !( (mMove >> 12) & 0xf);
}

bool Move::isDoublePawnPush() const
{
  return ( ( (mMove >> 12)) == 0x0001);
}

bool Move::isKingCastle() const
{
  return ( ( (mMove >> 12)) == 0x0002);
}

bool Move::isQueenCastle() const
{
  return ( ( (mMove >> 12)) == 0x0003);
}

bool Move::isCapture() const
{
  return  (mMove >> 16) & 0x00ff;
}

bool Move::isPromotion() const
{
  return (mMove >> 24) & 0x00ff;
}

bool Move::isEnPassant() const
{
  return ( ((mMove >> 12) & 0x000f) == 0x0005);
}

uint Move::sourceRow() const
{
  return (mMove & 0x0007);
}

uint Move::sourceColumn() const
{
  return ( (mMove >> 3) & 0x0007);
}

uint Move::destinationRow() const
{
  return ( (mMove >> 6) & 0x0007);
}

uint Move::destinationColumn() const
{
  return ( (mMove >> 9) & 0x0007);
}

PieceType Move::capturedPiece() const
{
  return PieceType( (mMove >> 16) & 0x00ff);
}

PieceType Move::promotedPiece() const
{
  return PieceType( (mMove >> 24) & 0x00ff);
}

bool Move::operator==(const Move & otherMove) const
{
  return ((mMove & 0xffff) == (otherMove.mMove & 0xffff));
}

bool Move::operator!=(const Move & otherMove) const
{
  return !(*this == otherMove);
}

// This will likely need to be moved since the game state is actually
// important to know to generate the move properly
Move Move::fromSmithNotation(const std::string & moveString, bool whiteMove)
{
  static const char colLetter[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

  assert(moveString.size() >= 4);

  int sourceColNumber = 0;
  char sourceCol = moveString.at(0);
  for (int i = 0; i < 8; i++)
    if (sourceCol == colLetter[i])
      sourceColNumber = i;

  char sourceRow = moveString.at(1);
  int sourceRowNumber = sourceRow - '0';
  sourceRowNumber--;

  int destColNumber = 0;
  char destCol = moveString.at(2);
  for (int i = 0; i < 8; i++)
    if (destCol == colLetter[i])
      destColNumber = i;

  char destRow = moveString.at(3);
  int destRowNumber = destRow - '0';
  destRowNumber--;

  Move newMove(sourceRowNumber, sourceColNumber, destRowNumber, destColNumber);
  if (moveString.size() > 4) {
    char specialChar = moveString.at(4);
    switch (specialChar) {
    case 'p':
      newMove.setCapturedPiece(whiteMove ? BlackPawn : WhitePawn);
      newMove.setCapture();
      break;
    case 'n':
      newMove.setCapturedPiece(whiteMove ? BlackKnight : WhiteKnight);
      newMove.setCapture();
      break;
    case 'b':
      newMove.setCapturedPiece(whiteMove ? BlackBishop : WhiteBishop);
      newMove.setCapture();
      break;
    case 'r':
      newMove.setCapturedPiece(whiteMove ? BlackRook : WhiteRook);
      newMove.setCapture();
      break;
    case 'q':
      newMove.setCapturedPiece(whiteMove ? BlackQueen : WhiteQueen);
      newMove.setCapture();
      break;
    case 'k':
      newMove.setCapturedPiece(whiteMove ? BlackKing : WhiteKing);
      newMove.setCapture();
      break;
    case 'c':
      newMove.setKingCastle();
      break;
    case 'C':
      newMove.setQueenCastle();
      break;
    case 'E':
      newMove.setCapture();
      newMove.setCapturedPiece(whiteMove ? BlackPawn : WhitePawn);
      newMove.setEnPassant();
      break;
    case 'N':
      newMove.setPromotedPiece(whiteMove ? WhiteKnight : BlackKnight);
      break;
    case 'B':
      newMove.setPromotedPiece(whiteMove ? WhiteBishop : BlackBishop);
      break;
    case 'R':
      newMove.setPromotedPiece(whiteMove ? WhiteRook : BlackRook);
      break;
    case 'Q':
      newMove.setPromotedPiece(whiteMove ? WhiteQueen : BlackQueen);
      break;
    }
  }

  if (moveString.size() > 5) {
    char specialChar = moveString.at(5);
    switch (specialChar) {
    case 'N':
      newMove.setPromotedPiece(whiteMove ? WhiteKnight : BlackKnight);
      break;
    case 'B':
      newMove.setPromotedPiece(whiteMove ? WhiteBishop : BlackBishop);
      break;
    case 'R':
      newMove.setPromotedPiece(whiteMove ? WhiteRook : BlackRook);
      break;
    case 'Q':
      newMove.setPromotedPiece(whiteMove ? WhiteQueen : BlackQueen);
      break;
    }
  }

  return newMove;
}

std::string Move::getSmithNotation() const
{
  static const char colLetter[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

  std::stringstream oss;
  oss << colLetter[sourceColumn()]
      << sourceRow()+1
      << colLetter[destinationColumn()]
      << destinationRow()+1;

  if (isCapture() && !isEnPassant()) {
    switch (capturedPiece()) {
      case WhitePawn:
      case BlackPawn:
        oss << 'p';
        break;
      case WhiteKnight:
      case BlackKnight:
        oss << 'n';
        break;
      case WhiteRook:
      case BlackRook:
        oss << 'r';
        break;
      case WhiteBishop:
      case BlackBishop:
        oss << 'b';
        break;
      case WhiteQueen:
      case BlackQueen:
        oss << 'q';
        break;
      case WhiteKing:
      case BlackKing:
        oss << 'k';
        break;
      default:
        break;
      }
    }

    if (isEnPassant())
      oss << 'E';

    if (isKingCastle())
      oss << 'c';

    if (isQueenCastle())
      oss << 'C';

    switch (promotedPiece()) {
    case WhiteQueen:
    case BlackQueen:
      oss << 'Q';
      break;
    case WhiteBishop:
    case BlackBishop:
      oss << 'B';
      break;
    case WhiteRook:
    case BlackRook:
      oss << 'R';
      break;
    case WhiteKnight:
    case BlackKnight:
      oss << 'N';
      break;
    default:
      break;
    }

    return oss.str();
}

bool Move::operator<(const Move & otherMove) const
{
  if (sourceColumn() < otherMove.sourceColumn())
    return true;

  if (sourceRow() < otherMove.sourceRow())
    return true;

  if (destinationColumn() < otherMove.destinationColumn())
    return true;

  if (destinationRow() < otherMove.destinationRow())
    return true;

  return false;
}

void Move::setDoublePawnPush()
{
  mMove |= 0x1000;
}

void Move::setKingCastle()
{
  mMove |= 0x2000;
}

void Move::setQueenCastle()
{
  mMove |= 0x3000;
}

void Move::setCapture()
{
  mMove |= 0x4000;
}

void Move::setEnPassant()
{
  mMove |= 0x5000;
}

//void Move::setKnightPromotion()
//{
//	mMove |= 0x8000;
//}
//
//void Move::setBishopPromotion()
//{
//	mMove |= 0x9000;
//}
//
//void Move::setRookPromotion()
//{
//	mMove |= 0xa000;
//}
//
//void Move::setQueenPromotion()
//{
//	mMove |= 0xb000;
//}
//
//void Move::setKnightPromotionCapture()
//{
//	mMove |= 0xc000;
//}
//
//void Move::setBishopPromotionCapture()
//{
//	mMove |= 0xd000;
//}
//
//void Move::setRookPromotionCapture()
//{
//	mMove |= 0xe000;
//}
//
//void Move::setQueenPromotionCapture()
//{
//	mMove |= 0xf000;
//}

void Move::setSourceRow(u_int sourceRow)
{
  mMove |= (sourceRow & 0x0007);
}

void Move::setSourceColumn(u_int sourceCol)
{
  mMove |= ((sourceCol & 0x0007) << 3);
}

void Move::setDestinationRow(u_int destRow)
{
  mMove |= ((destRow & 0x0007) << 6);
}

void Move::setDestinationColumn(u_int destCol)
{
  mMove |= ((destCol & 0x0007) << 9);
}

void Move::setCapturedPiece(PieceType pieceType)
{
  mMove &= 0xff00ffff;                    // Clear any existing value
  mMove |= ( (pieceType & 0x00ff) << 16); // Update with new value
  setCapture();
}

void Move::setPromotedPiece(PieceType pieceType)
{
  mMove &= 0x00ffffff;                    // Clear any existing value
  mMove |= ( (pieceType & 0x00ff) << 24); // Update with new value
}
