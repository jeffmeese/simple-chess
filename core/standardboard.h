#ifndef STANDARDBOARD_H
#define STANDARDBOARD_H

#include "abstractboard.h"
#include "stalematecondition.h"

#include <list>

//template <class MoveGenerator, class S
//class Move;
//class MoveList;

//class StandardBoard : public AbstractBoard<StandardBoard>
//{
//  friend class AbstractBoard<StandardBoard>;

//public:
//  StandardBoard();

//  bool makeMove(Move &newMove);
//  void saveState();

//private:
//  std::list<uint> mSavedStates;

//  uint generateLegalMoves(MoveList & moveList)
//  {
//    // Generate the list of candidate moves
//    MoveList candidateMoves;
//    generateCandidateMoves(candidateMoves);

//    // Trim the candidate moves based on the legality of the candidate moves
//    trimCandidateMoves(candidateMoves, moveList);

//    return moveList.totalMoves();
//  }


//  bool isLegal() const
//  {
//    if (!isWhiteToMove() && isWhiteChecked())
//      return false;

//    if (isWhiteToMove() && isBlackChecked())
//      return false;

//    return true;
//  }

//  void trimCandidateMoves(MoveList & candidateMoves, MoveList & legalMoves)
//  {
//    for (uint i = 0; i < candidateMoves.totalMoves(); ++i) {
//      Move currentMove(candidateMoves[i]);
//      makeMove(currentMove);
//      if (isLegal())
//        legalMoves.addMove(currentMove);
//      unmakeMove(currentMove);
//    }
//  }

//  bool canBlackCastleKingSide() const;
//  bool canBlackCastleQueenSide() const;
//  bool canWhiteCastleKingSide() const;
//  bool canWhiteCastleQueenSide() const;
//  PieceType getPieceType(uint row, uint col) const;
//  uint getBlackKingRow() const;
//  uint getBlackKingCol() const;
//  uint getWhiteKingRow() const;
//  uint getWhiteKingCol() const;
//  bool isBlackChecked() const;
//  bool isBlackMated() const;
//  bool isCellAttacked(uint row, uint col, bool isWhitePiece) const;
//  bool isWhiteChecked() const;
//  bool isWhiteMated() const;
//  bool isWhiteToMove() const;
//  bool makeMove(Move & newMove);
//  bool unmakeMove(Move & oldMove);

//protected:
//  void generateCandidateMoves(MoveList & moveList);
//  void generateCandidateMoves(uint row, uint col, MoveList & moveList);

//public:
//  void generateBishopMoves(uint row, uint col, MoveList & moveList);
//  void generateBlackPawnMoves(uint row, uint col, MoveList & moveList);
//  void generateKingMoves(uint row, uint col, MoveList & moveList);
//  void generateKnightMoves(uint row, uint col, MoveList & moveList);
//  void generateMoves(std::list<uint> & indexList, uint row, uint col, MoveList & moveList, bool isSlider);
//  void generateRookMoves(uint row, uint col, MoveList & moveList);
//  void generateQueenMoves(uint row, uint col, MoveList & moveList);
//  void generateWhitePawnMoves(uint row, uint col, MoveList & moveList);
//  uint getIndex(uint row, uint col) const;
//  void getRowCol(uint index, uint & row, uint & col) const;
//  void initBoard();
//  void initMoves();
//  void loadState();
//  void saveState();

//private:
//  bool mBlackCastleKingSide;
//  bool mBlackCastleQueenSide;
//  bool mBlackChecked;
//  bool mBlackEnPassant;
//  bool mBlackMated;
//  bool mStalemate;
//  bool mWhiteChecked;
//  bool mWhiteCastleKingSide;
//  bool mWhiteCastleQueenSide;
//  bool mWhiteEnPassant;
//  bool mWhiteMated;
//  bool mWhiteToMove;
//  uint mEnPassantCaptureCol;
//  uint mBlackKingRow;
//  uint mBlackKingCol;
//  uint mHalfMoveClock;
//  uint mTotalBlackBishops;
//  uint mTotalBlackKnights;
//  uint mTotalBlackPawns;
//  uint mTotalBlackPieces;
//  uint mTotalBlackRooks;
//  uint mTotalBlackQueens;
//  uint mTotalPieces;
//  uint mTotalWhiteBishops;
//  uint mTotalWhiteKnights;
//  uint mTotalWhitePawns;
//  uint mTotalWhitePieces;
//  uint mTotalWhiteRooks;
//  uint mTotalWhiteQueens;
//  uint mWhiteKingRow;
//  uint mWhiteKingCol;
//  //StalemateCondition mStalemateCondition;
//  std::list<uint> mRookMovesNorth[64];
//  std::list<uint> mRookMovesSouth[64];
//  std::list<uint> mRookMovesWest[64];
//  std::list<uint> mRookMovesEast[64];
//  std::list<uint> mBishopMovesNorthEast[64];
//  std::list<uint> mBishopMovesSouthEast[64];
//  std::list<uint> mBishopMovesNorthWest[64];
//  std::list<uint> mBishopMovesSouthWest[64];
//  std::list<uint> mKingMoves[64];
//  std::list<uint> mKnightMoves[64];
//  std::list<uint> mWhitePawnMoves[64];
//  std::list<uint> mBlackPawnMoves[64];
  //std::list<uint> mSavedStates;
  //PieceType mPieceType[64];
//};

class StandardBoard
    //: public AbstractBoard
{
public:
  StandardBoard();

public:
  bool canBlackCastleKingSide() const;
  bool canBlackCastleQueenSide() const;
  bool canWhiteCastleKingSide() const;
  bool canWhiteCastleQueenSide() const;
  uint generateLegalMoves(MoveList & moveList);
  uint generateLegalMoves(uint row, uint col, MoveList & moveList);
  void getCastlingRights(uint & castlingRights) const;
  PieceType getPieceType(uint row, uint col) const;
  uint getBlackKingRow() const;
  uint getBlackKingCol() const;
  uint getWhiteKingRow() const;
  uint getWhiteKingCol() const;
  bool isBlackChecked() const;
  bool isBlackMated() const;
  bool isCellAttacked(uint row, uint col, bool isWhitePiece) const;
  bool isWhiteChecked() const;
  bool isWhiteMated() const;
  bool isWhiteToMove() const;
  bool makeMove(Move & newMove);
  void setPosition(const std::string & fenString);
  bool unmakeMove(Move & undoMove);

protected:
  bool isLegal() const;

private:
  void generateBishopMoves(uint row, uint col, MoveList & moveList);
  void generateBlackPawnMoves(uint row, uint col, MoveList & moveList);
  void generateCandidateMoves(MoveList & moveList);
  void generateCandidateMoves(uint row, uint col, MoveList & moveList);
  void generateKingMoves(uint row, uint col, MoveList & moveList);
  void generateKnightMoves(uint row, uint col, MoveList & moveList);
  void generateMoves(std::list<uint> & indexList, uint row, uint col, MoveList & moveList, bool isSlider);
  void generateRookMoves(uint row, uint col, MoveList & moveList);
  void generateQueenMoves(uint row, uint col, MoveList & moveList);
  void generateWhitePawnMoves(uint row, uint col, MoveList & moveList);
  uint getIndex(uint row, uint col) const;
  void getRowCol(uint index, uint & row, uint & col) const;
  void initBoard();
  void initMoves();
  void loadState();
  void saveState();
  void trimCandidateMoves(MoveList & candidateMoves, MoveList & legalMoves);

private:
  StandardBoard(const StandardBoard &);
  StandardBoard & operator=(const StandardBoard &);

private:
  bool mBlackCastleKingSide;
  bool mBlackCastleQueenSide;
  bool mBlackChecked;
  bool mBlackEnPassant;
  bool mBlackMated;
  bool mStalemate;
  bool mWhiteChecked;
  bool mWhiteCastleKingSide;
  bool mWhiteCastleQueenSide;
  bool mWhiteEnPassant;
  bool mWhiteMated;
  bool mWhiteToMove;
  uint mEnPassantCaptureCol;
  uint mBlackKingRow;
  uint mBlackKingCol;
  uint mHalfMoveClock;
  uint mTotalBlackBishops;
  uint mTotalBlackKnights;
  uint mTotalBlackPawns;
  uint mTotalBlackPieces;
  uint mTotalBlackRooks;
  uint mTotalBlackQueens;
  uint mTotalPieces;
  uint mTotalWhiteBishops;
  uint mTotalWhiteKnights;
  uint mTotalWhitePawns;
  uint mTotalWhitePieces;
  uint mTotalWhiteRooks;
  uint mTotalWhiteQueens;
  uint mWhiteKingRow;
  uint mWhiteKingCol;
  StalemateCondition mStalemateCondition;
  std::list<uint> mSavedStates;          //!< List of saved states generated by makeMove
  std::list<uint> mRookMovesNorth[64];
  std::list<uint> mRookMovesSouth[64];
  std::list<uint> mRookMovesWest[64];
  std::list<uint> mRookMovesEast[64];
  std::list<uint> mBishopMovesNorthEast[64];
  std::list<uint> mBishopMovesSouthEast[64];
  std::list<uint> mBishopMovesNorthWest[64];
  std::list<uint> mBishopMovesSouthWest[64];
  std::list<uint> mKingMoves[64];
  std::list<uint> mKnightMoves[64];
  std::list<uint> mWhitePawnMoves[64];
  std::list<uint> mBlackPawnMoves[64];
  PieceType mPieceType[64];
};

#endif // STANDARDBOARD_H
