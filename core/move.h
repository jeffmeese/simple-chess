#ifndef MOVE_H
#define MOVE_H

#include "piecetype.h"
#include "types.h"

#include <QString>

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

#endif // #ifndef MOVE_H
