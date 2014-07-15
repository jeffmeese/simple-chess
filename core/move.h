#ifndef MOVE_H
#define MOVE_H

#include "piecetype.h"
#include "types.h"

#include <string>

//! Encapsulates a chess move
//!
//! The Move class encapsulates a chess move. The storage scheme is
//! designed with memory efficiency in mind so all information is stored in
//! a single 32-bit uinteger. Only 16 bits are actually
//! used but a 32-bit word is used to fit a 32 bit register exactly.
//! Bitmasks and shifting operations are used by the interface for the
//! proper extraction of internal values.
//! This class is derived from the article at:
//! http://chessprogramming.wikispaces.com/Encoding+Moves
//!
//! The memory layout is as follows:
//!
//! Bits (0-2) Source Row
//! Bits (3-5) Source Column
//! Bits (6-8) Destination Row
//! Bits (9-11) Destination Column
//! Bits (12-15) Flags corresponding to special moves
//! Bits (16-23) Captured piece, if any
//! Bits (24-31) Promoted piece, if any
//!
//! The flag values for special moves are below: (numbers in () are hex):
//!   0000 (0) Quiet Move
//!   0001 (1) Double Pawn Push
//!   0010 (2) King Castle
//!   0011 (3) Queen Castle
//!   0100 (4) Capture
//!   0101 (5) En Passant Capture
//!   1000 (8) Knight Promotion
//!   1001 (9) Bishop Promotion
//!   1010 (A) Rook Promotion
//!   1011 (B) Queen Promotion
//!   1100 (C) Knight Promotion Capture
//!   1101 (D) Bishop Promotion Capture
//!   1110 (E) Rook Promotion Capture
//!   1111 (F) Queen Promotion Capture
class Move
{
public:
  Move();
  Move(uint sourceRow, uint sourceCol, uint destRow, uint destCol, PieceType capturedPiece = NoPiece, PieceType promotedPiece = NoPiece);

  Move(const Move & otherMove);
  Move & operator=(const Move & otherMove);

  // Properties
public:
  bool isQuiet() const;
  bool isCapture() const;
  bool isDoublePawnPush() const;
  bool isKingCastle() const;
  bool isQueenCastle() const;
  bool isEnPassant() const;
  bool isPromotion() const;
  bool isCapturePromotion() const;
  uint sourceRow() const;
  uint sourceColumn() const;
  uint destinationRow() const;
  uint destinationColumn() const;
  PieceType capturedPiece() const;
  PieceType promotedPiece() const;
  void setCapture();
  void setDoublePawnPush();
  void setKingCastle();
  void setQueenCastle();
  void setEnPassant();
  void setSourceRow(uint sourceRow);
  void setSourceColumn(uint sourceCol);
  void setDestinationRow(uint destRow);
  void setDestinationColumn(uint destCol);
  void setCapturedPiece(PieceType pieceType);
  void setPromotedPiece(PieceType pieceType);

public:
  static Move fromSmithNotation(const std::string & moveString, bool whiteMove);
  std::string getSmithNotation() const;

  // Operators
public:
  bool operator==(const Move & otherMove) const;
  bool operator!=(const Move & otherMove) const;
  bool operator<(const Move & otherMove) const;

  // Member variables
private:
  uint mMove; //!< Single uinteger to hold all data
};

inline bool Move::isQuiet() const
{
  return !( (mMove >> 12) & 0xf);
}

inline bool Move::isDoublePawnPush() const
{
  return ( ( (mMove >> 12)) == 0x0001);
}

inline bool Move::isKingCastle() const
{
  return ( ( (mMove >> 12)) == 0x0002);
}

inline bool Move::isQueenCastle() const
{
  return ( ( (mMove >> 12)) == 0x0003);
}

inline bool Move::isCapture() const
{
  return  (mMove >> 16) & 0x00ff;
}

inline bool Move::isPromotion() const
{
  return (mMove >> 24) & 0x00ff;
}

inline bool Move::isEnPassant() const
{
  return ( ((mMove >> 12) & 0x000f) == 0x0005);
}

inline uint Move::sourceRow() const
{
  return (mMove & 0x0007);
}

inline uint Move::sourceColumn() const
{
  return ( (mMove >> 3) & 0x0007);
}

inline uint Move::destinationRow() const
{
  return ( (mMove >> 6) & 0x0007);
}

inline uint Move::destinationColumn() const
{
  return ( (mMove >> 9) & 0x0007);
}

inline PieceType Move::capturedPiece() const
{
  return PieceType( (mMove >> 16) & 0x00ff);
}

inline PieceType Move::promotedPiece() const
{
  return PieceType( (mMove >> 24) & 0x00ff);
}

inline bool Move::operator==(const Move & otherMove) const
{
  return ((mMove & 0xffff) == (otherMove.mMove & 0xffff));
}

inline bool Move::operator!=(const Move & otherMove) const
{
  return !(*this == otherMove);
}

inline void Move::setDoublePawnPush()
{
  mMove |= 0x1000;
}

inline void Move::setKingCastle()
{
  mMove |= 0x2000;
}

inline void Move::setQueenCastle()
{
  mMove |= 0x3000;
}

inline void Move::setCapture()
{
  mMove |= 0x4000;
}

inline void Move::setEnPassant()
{
  mMove |= 0x5000;
}

inline void Move::setSourceRow(u_int sourceRow)
{
  mMove |= (sourceRow & 0x0007);
}

inline void Move::setSourceColumn(u_int sourceCol)
{
  mMove |= ((sourceCol & 0x0007) << 3);
}

inline void Move::setDestinationRow(u_int destRow)
{
  mMove |= ((destRow & 0x0007) << 6);
}

inline void Move::setDestinationColumn(u_int destCol)
{
  mMove |= ((destCol & 0x0007) << 9);
}

#endif // #ifndef MOVE_H
