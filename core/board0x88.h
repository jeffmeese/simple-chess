#ifndef BOARD0X88_H
#define BOARD0X88_H

#include "types.h"
#include "move.h"
#include "piecetype.h"
#include "stalematecondition.h"
#include "array2d.h"

#include <string>
#include <vector>
#include <list>

#define getIndex(row,col) ((row)*16+(col))
#define getRow(index) ( (index >> 4) )
#define getCol(index) ( (index & 7) )
#define isValidSquare(x)  ( (x) & 0x88 ) ? (0) : (1)

class Board0x88;

class sMove
{
  friend class Board0x88;

public:
  uchar fromSquare;
  uchar toSquare;
  uchar halfMove;
  uchar epIndex;
  uchar fromPiece;
  uchar toPiece;
  uchar capturedPiece;
  uchar castlingRights;
  uchar flags;
};

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
  int enPassantCol() const;
  int enPassantRow() const;
  int fullMoveCounter() const;
  int halfMoveClock() const;
  uchar sideToMove() const;

public:
  uchar generateMoves(sMove * moveList);
  std::string getSmithNotation(const sMove & move) const;
  //uchar getCol(uchar index) const;
  //uchar getIndex(uchar row, uchar col) const;
  //uchar getRow(uchar index) const;
  bool isCellAttacked(uchar index, uchar attackingColor) const;
  uchar kingIndex(uchar color) const;
  void makeMove(const sMove & newMove);
  void unmakeMove(const sMove & undoMove);
  //uchar generateMoves(uchar row, uchar col, MoveList & moveList);
  PieceType pieceType(uchar row, uchar col) const;

private:
  enum MoveFlags
  {
    MoveNormal = 0,
    MoveCapture = 1,
    MoveEpCapture = 2,
    MoveCastle = 4,
    MoveEp = 8,
    MovePromotion = 16,
    MoveNull = 32
  };

private:
  void generatePawnCaptures(uchar index);
  void generatePawnMoves(uchar index);
  void initAttacks();
  void initBoard();
  //bool isValidSquare(uchar index) const;
  void pushMove(uchar fromSquare, uchar toSquare, uchar pieceFrom, uchar pieceCapture, uchar flags);

private:
  uchar mSideToMove;
  uchar mMoveCount;
  uchar mCastlingRights;
  uchar mHalfMoveClock;
  uchar mFullMoveCounter;
  uchar mEpIndex;
  uchar mKingIndex[2];
  uchar mPieces[128];
  uchar mColors[128];
  bool mSliders[5];
  uchar mNumDirections[5];
  uchar mNumKnightAttacks[128];
  uchar mNumKingAttacks[128];
  Array2D<uchar> mNumStraightAttacks;
  Array2D<uchar> mNumDiagAttacks;
  Array2D<uchar> mKnightAttacks;
  Array2D<uchar> mKingAttacks;
  Array2D<uchar> mStraightAttacks[4];
  Array2D<uchar> mDiagAttacks[4];
  Array2D<uchar> mDirectionVectors;
  sMove * mMoves;
};

//class Board0x88
//{
//public:
//	Board0x88(void);
//	~Board0x88(void);

//public:
////  uint blackKingIndex() const;
////  bool canBlackCastleQueenSide() const;
////  bool canBlackCastleKingSide() const;
////  bool canWhiteCastleQueenSide() const;
////  bool canWhiteCastleKingSide() const;
////  uint enPassantIndex() const;
////  bool isBlackChecked() const;
////  bool isBlackToMove() const;
////  bool isWhiteChecked() const;
////  bool isWhiteToMove() const;
////  uint whiteKingIndex() const;

//public:
////  uint generateLegalMoves(MoveList & moveList);
////  uint generateLegalMoves(uint row, uint col, MoveList & moveList);
////  PieceType getPieceType(uint row, uint col) const;
////  bool makeMove(const Move & newMove);
////  std::string getFenString() const;
////  void setPosition(const std::string & fenString);
////  bool unmakeMove(const Move & undoMove);

//private:
//  struct sMove
//  {
//    uchar fromSquare;
//    uchar toSquare;
//    uchar halfMove;
//    uchar epIndex;
//    uchar fromPiece;
//    uchar toPiece;
//    uchar capturedPiece;
//    uchar castlingRights;
//    uchar flags;
//  };

//  enum MoveFlags
//  {
//    MoveNormal = 0,
//    MoveCapture = 1,
//    MoveEpCapture = 2,
//    MoveCastle = 4,
//    MoveEp = 8,
//    MovePromotion = 16,
//    MoveNull = 32
//  };

//private:
//  //typedef std::vector<PieceType> PieceToPieceType;
//  //typedef std::vector<Piece> PieceTypeToPiece;
//  //typedef std::list<ulong> StateList;

//private:
//  void pawnMoves(uchar index);
//  void pawnCaptures(uint index);
//  void pushMove(uchar from, uchar to, uchar pieceFrom, uchar pieceTo, uchar pieceCapture, uchar flags);
//  //void generatePawnMoves(uint index, MoveList & moveList);
//  //void generatePawnCaptures(uint index, MoveList & moveList);
//  //uint getKingIndex(uchar kingColor) const;
//  //uint getIndex(uint row, uint col) const;
//  //uint getRow(uint index) const;
//  //uint getCol(uint index) const;
//  //void initAttacks();
//  //void initBoard();
//  //void initMoves();
//  //void initPieceMap();
//  //bool isCellAttacked(uint index, uchar attackingColor) const;
//  //bool isValidSquare(uint index) const;
//  //bool leapAttacks(uint index, uchar attackingColor) const;
//  //void loadState();
//  //void printColors();
//  //void pawnMoves(uchar index);
//  //bool sliderAttack(uint index, uchar attackingColor, int attackDirection, bool diag) const;
//  //void saveState();

//public:
//  smove mMoves[256];
//  uint mMoveCount;
////  bool mBlackChecked;
////  bool mWhiteChecked;
////  uchar mSideToMove;
////  uint mCastlingRights;
////  uint mEnPassantIndex;
////  uint mBlackKingIndex;
////  uint mWhiteKingIndex;
////  uint mHalfMoveClock;
////  uint mFullMoveCounter;
////  PieceType mPieceTypes[128];
////  Piece mPieces[128];
////  uchar mColors[128];
////  uint mNumKnightAttacks[128];
////  uint mNumKingAttacks[128];
////  uint mNumDiagAttacks[128][4];
////  uint mNumStraightAttacks[128][4];
////  uint mKnightAttacks[128][8];
////  uint mKingAttacks[128][8];
////  uint mStraightAttacks[128][4][8];
////  uint mDiagAttacks[128][4][8];
////  PieceToPieceType mWhitePieceMap;
////  PieceToPieceType mBlackPieceMap;
////  PieceTypeToPiece mPieceMap;
////  StateList mSavedStates;
////  smove mMoves[256];
////  uint mMoveCounter;
//};

#endif // #ifndef BOARD0X88_H
