#ifndef BOARD15x12_H
#define BOARD15x12_H

#include "types.h"
#include "piecetype.h"
#include "stalematecondition.h"

#include <list>
#include <map>
#include <vector>

class Move;
class MoveList;

class Board15x12
{
public:
	Board15x12(void);
	~Board15x12(void);

public:
  bool canBlackCastleQueenSide() const;
  bool canBlackCastleKingSide() const;
  bool canBlackEnPassant() const;
  bool canWhiteCastleQueenSide() const;
  bool canWhiteCastleKingSide() const;
  bool canWhiteEnPassant() const;
  uint getBlackKingRow() const;
  uint getBlackKingCol() const;
  uint getWhiteKingRow() const;
  uint getWhiteKingCol() const;
  bool isBlackChecked() const;
  bool isBlackMated() const;
  bool isBlackToMove() const;
  bool isWhiteChecked() const;
  bool isWhiteMated() const;
  bool isWhiteToMove() const;
  bool isStalemate() const;
  uint enPassantCaptureCol() const;
  uint halfMoveClock() const;
  uint totalWhiteBishops() const;
  uint totalWhiteQueens() const;
  uint totalWhiteRooks() const;
  uint totalWhitePawns() const;
  uint totalWhiteKnights() const;
  uint totalBlackBishops() const;
  uint totalBlackQueens() const;
  uint totalBlackRooks() const;
  uint totalBlackPawns() const;
  uint totalBlackKnights() const;

public:
	//! Computes a hash checksum for the board
  //std::string computeChecksum() const;

	//! Generates the legal moves for a single piece based on board position
  uint generateLegalMoves(uint row, uint col, MoveList & moveList);

	//! Generates the legal moves for all pieces based on board position
  uint generateLegalMoves(MoveList & moveList);

  void getCastlingRights(uint & castlingRights) const;

	//! Gets the piece type at the given row and column
  PieceType getPieceType(uint row, uint col) const;

  bool isCellAttacked(uint row, uint col, bool whitePiece) const;

  //! Makes a move
  bool makeMove(const Move & newMove);

	//! Sets the position of the board with the given fen string
  void setPosition(const std::string & fenString);

  //! Unmakes a move
  bool unmakeMove(const Move & undoMove);

private:
  void generateBishopMoves(uint index, uint sourceRow, uint sourceCol, PieceType pieceType, MoveList & moveList);
  void generateBlackPawnMoves(uint index, uint sourceRow, uint sourceCol, MoveList & moveList);
  void generateKingMoves(uint index, uint sourceRow, uint sourceCol, PieceType pieceType, MoveList & moveList);
  void generateKnightMoves(uint index, uint sourceRow, uint sourceCol, PieceType pieceType, MoveList & moveList);
  void generateRookMoves(uint index, uint sourceRow, uint sourceCol, PieceType pieceType, MoveList & moveList);
  void generateWhitePawnMoves(uint index, uint sourceRow, uint sourceCol, MoveList & moveList);
	void initMoves();
  uint getIndex(uint row, uint col) const;
  void getRowCol(uint index, uint & row, uint & col) const;
  uint getRow(uint index) const;
  uint getCol(uint index) const;
	void initBoard();
  bool isLegal() const;

	void loadState();
	void saveState();

private:
	bool mWhiteToMove;                      //!< Determines whether it is white or black's turn to move
	bool mBlackChecked;                     //!< Determines if black is currently in check
	bool mWhiteChecked;                     //!< Determines if white is currently in check
  bool mWhiteMated;                       //!< Determines whether white is checkmated
  bool mBlackMated;                       //!< Determines whether white is checkmated
	bool mStalemate;                        //!< Determines whether the game has reached a stalemate condition
	bool mBlackCastleKingSide;              //!< Black's ability to castle king side
	bool mBlackCastleQueenSide;             //!< Black's ability to castle queen side
	bool mWhiteCastleKingSide;              //!< White's ability to castle king side
	bool mWhiteCastleQueenSide;             //!< White's ability to castle queen side
	bool mBlackEnPassant;                   //!< Black's ability to perform an en-passant pawn capture
  bool mWhiteEnPassant;                   //!< White's ability to perform an en-passant pawn capture
  uint mEnPassantCaptureCol;              //!< The en-passant capture column
  uint mHalfMoveClock;                    //!< The number of moves since a pawn or capture move
  uint mFullMoveCounter;                  //! The number of full moves counted
  uint mWhiteKingRow;                     //!< Row position of the white king
  uint mWhiteKingCol;                     //!< Column position of the white king
  uint mBlackKingRow;                     //!< Row position of the black king
  uint mBlackKingCol;                     //!< Column position of the black king
  uint mTotalWhitePawns;
  uint mTotalWhiteRooks;
  uint mTotalWhiteBishops;
  uint mTotalWhiteKnights;
  uint mTotalWhiteQueens;
  uint mTotalBlackPawns;
  uint mTotalBlackRooks;
  uint mTotalBlackBishops;
  uint mTotalBlackKnights;
  uint mTotalBlackQueens;
  StalemateCondition mStalemateCondition; //!< Current stalemate condition of game
  PieceType mPieces[180];
  std::list<uint> mKnightMoves[64];
  std::list<uint> mWhitePieces;
  std::list<uint> mBlackPieces;
  std::list<uint> mSavedStates;          //!< List of saved states generated by makeMove
	std::map<PieceType, std::vector<int> > mPieceAttacks;
};

#endif // #ifndef BOARD15x12_H
