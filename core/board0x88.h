#ifndef BOARD0X88_H
#define BOARD0X88_H

#include "types.h"
#include "piecetype.h"
#include "stalematecondition.h"

#include <string>
#include <vector>
#include <list>

class Move;
class MoveList;

class Board0x88 
{
public:
	Board0x88(void);
	~Board0x88(void);

public:
  bool canBlackCastleQueenSide() const;
  bool canBlackCastleKingSide() const;
  bool canWhiteCastleQueenSide() const;
  bool canWhiteCastleKingSide() const;
  bool isBlackChecked() const;
  bool isWhiteChecked() const;
  bool isWhiteToMove() const;

public:
  uint generateLegalMoves(MoveList & moveList);
  uint generateLegalMoves(uint row, uint col, MoveList & moveList);
  PieceType getPieceType(uint row, uint col) const;
  bool makeMove(const Move & newMove);
  void setPosition(const std::string & fenString);
  bool unmakeMove(const Move & undoMove);

private:
  enum Piece {King = 0, Queen, Rook, Bishop, Knight, Pawn, Empty};
  typedef std::vector<PieceType> PieceToPieceType;
  typedef std::vector<Piece> PieceTypeToPiece;
  typedef std::list<uint> StateList;

private:
  void generatePawnMoves(uint index, MoveList & moveList);
  void generatePawnCaptures(uint index, MoveList & moveList);
  uint getIndex(uint row, uint col) const;
  uint getRow(uint index) const;
  uint getCol(uint index) const;
  void initBoard();
  void initPieceMap();
  bool isCellAttacked(uint index, Color attackingColor) const;
  bool isValidSquare(uint index) const;
  bool leapAttack(uint index, Color attackingColor, Piece pieceType) const;
  void loadState();
  bool sliderAttack(uint index, Color attackingColor, int attackDirection, bool diag) const;
  void saveState();

public:
  void printColors();

private:
  bool mBlackChecked;
  bool mWhiteChecked;
  uint mCastlingRights;
  uint mEnPassantIndex;
  uint mBlackKingIndex;
  uint mWhiteKingIndex;
  uint mHalfMoveClock;
  PieceType mPieceTypes[128];
  Piece mPieces[128];
  uchar mColors[128];
  uchar mSideToMove;
  PieceToPieceType mWhitePieceMap;
  PieceToPieceType mBlackPieceMap;
  PieceTypeToPiece mPieceMap;
  StateList mSavedStates;
};

#endif // #ifndef BOARD0X88_H
