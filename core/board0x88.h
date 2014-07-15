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
  uint blackKingIndex() const;
  bool canBlackCastleQueenSide() const;
  bool canBlackCastleKingSide() const;
  bool canWhiteCastleQueenSide() const;
  bool canWhiteCastleKingSide() const;
  uint enPassantIndex() const;
  bool isBlackChecked() const;
  bool isBlackToMove() const;
  bool isWhiteChecked() const;
  bool isWhiteToMove() const;
  uint whiteKingIndex() const;

public:
  uint generateLegalMoves(MoveList & moveList);
  uint generateLegalMoves(uint row, uint col, MoveList & moveList);
  PieceType getPieceType(uint row, uint col) const;
  bool makeMove(const Move & newMove);
  std::string getFenString() const;
  void setPosition(const std::string & fenString);
  bool unmakeMove(const Move & undoMove);

private:
  typedef std::vector<PieceType> PieceToPieceType;
  typedef std::vector<Piece> PieceTypeToPiece;
  typedef std::list<ulong> StateList;

public:
  void generatePawnMoves(uint index, MoveList & moveList);
  void generatePawnCaptures(uint index, MoveList & moveList);
  uint getKingIndex(uchar kingColor) const;
  uint getIndex(uint row, uint col) const;
  uint getRow(uint index) const;
  uint getCol(uint index) const;
  void initAttacks();
  void initBoard();
  void initMoves();
  void initPieceMap();
  bool isCellAttacked(uint index, uchar attackingColor) const;
  bool isValidSquare(uint index) const;
  bool leapAttacks(uint index, uchar attackingColor) const;
  void loadState();
  bool sliderAttack(uint index, uchar attackingColor, int attackDirection, bool diag) const;
  void saveState();

public:
  void printColors();

public:
  bool mBlackChecked;
  bool mWhiteChecked;
  uchar mSideToMove;
  uint mCastlingRights;
  uint mEnPassantIndex;
  uint mBlackKingIndex;
  uint mWhiteKingIndex;
  uint mHalfMoveClock;
  uint mFullMoveCounter;
  PieceType mPieceTypes[128];
  Piece mPieces[128];
  uchar mColors[128];
  uint mNumKnightAttacks[128];
  uint mNumKingAttacks[128];
  uint mNumDiagAttacks[128][4];
  uint mNumStraightAttacks[128][4];
  uint mKnightAttacks[128][8];
  uint mKingAttacks[128][8];
  uint mStraightAttacks[128][4][8];
  uint mDiagAttacks[128][4][8];
  PieceToPieceType mWhitePieceMap;
  PieceToPieceType mBlackPieceMap;
  PieceTypeToPiece mPieceMap;
  StateList mSavedStates;
};

#endif // #ifndef BOARD0X88_H
