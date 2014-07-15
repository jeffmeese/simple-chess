#include "move.h"

#include <sstream>
#include <iostream>
#include <cassert>

Move::Move()
  : mMove(0)
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
