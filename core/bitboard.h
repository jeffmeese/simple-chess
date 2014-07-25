#ifndef BITBOARD_H
#define BITBOARD_H

#include "types.h"

#include <string>

class Move;
class MoveList;

class BitBoard
{
public:
  BitBoard();
  ~BitBoard();

public:
  char castlingRights() const;
  int enPassantCol() const;
  int enPassantRow() const;
  uint fullMoveCounter() const;
  uint halfMoveClock() const;
  char sideToMove() const;

public:
  uchar generateMoves(MoveList & moveList);
  uchar generateOpponentMoves(MoveList & moveList);
  uchar generateMoves(uchar row, uchar col, MoveList & moveList);
  bool isKingAttacked(char kingColor) const;
  bool isCellAttacked(uchar row, uchar col, char attackingColor) const;
  uchar kingRow(char color) const;
  uchar kingCol(char color) const;
  void makeMove(const Move & newMove);
  PieceType pieceType(uchar row, uchar col) const;
  bool setPosition(const std::string & fenString);
  void unmakeMove(const Move & undoMove);

private:
  uchar getIndex(uchar row, uchar col) const;
  void initBoard();
  void pushMove(uchar fromSquare, uchar toSquare, uchar pieceFrom, uchar pieceCapture, char flags, MoveList & moveList);
  ulonglong rotate90(ulonglong bb);
  ulonglong rotate45(ulonglong bb, const uint * rotMatrix);
  ulonglong rotateNeg45(ulonglong bb, const uint * rotMatrix);
  void writeBitBoard(ulonglong bb) const;

private:
  uchar mSideToMove;
  uchar mEpIndex;
  uchar mCastlingRights;
  uint mFullMoveCounter;
  uint mHalfMoveClock;
  ulonglong mEmptyBB;
  ulonglong mAllPiecesBB;
  ulonglong mRotate90AllPieces;
  ulonglong mRotate45LeftPieces;
  ulonglong mRotate45RightPieces;
  uchar mKingIndex[2];
  uchar mPieces[64];
  uchar mColors[64];
  ulonglong mColorBB[2];
  ulonglong mPieceBB[14];
  ulonglong mRotate90BB[64];
  ulonglong mRotate45RightBB[64];
  ulonglong mRotate45LeftBB[64];
};

inline char BitBoard::castlingRights() const
{
  return mCastlingRights;
}

inline int BitBoard::enPassantCol() const
{
  return 0;
}

inline int BitBoard::enPassantRow() const
{
  return 0;
}

inline uint BitBoard::fullMoveCounter() const
{
  return mFullMoveCounter;
}

inline uint BitBoard::halfMoveClock() const
{
  return mHalfMoveClock;
}

inline char BitBoard::sideToMove() const
{
  return mSideToMove;
}

#endif // #ifndef BITBOARD_H

//#ifndef BITBOARD_H
//#define BITBOARD_H

//#include "types.h"
//#include "move.h"
//#include "movelist.h"
//#include "stalematecondition.h"

//#include <list>

//class BitBoard
//{
//public:
//	BitBoard(void);
//	~BitBoard(void);

//public:
//  bool isBlackChecked() const;
//  bool isBlackToMove() const;
//  bool isBlackMated() const;
//  bool canBlackCastleQueenSide() const;
//  bool canBlackCastleKingSide() const;
//  bool blackEnPassantAvailable() const;
//  bool blackKingRow() const;
//  bool blackKingCol() const;
//  bool isWhiteChecked() const;
//  bool isWhiteToMove() const;
//  bool isWhiteMated() const;
//  bool isStalemate() const;
//  bool canWhiteCastleQueenSide() const;
//  bool canWhiteCastleKingSide() const;
//  bool whiteEnPassantAvailable() const;
//  bool whiteKingRow() const;
//  bool whiteKingCol() const;
//  uint enPassantCaptureCol() const;
//  uint halfMoveClock() const;
//  uint totalWhiteBishops() const;
//  uint totalWhiteQueens() const;
//  uint totalWhiteRooks() const;
//  uint totalWhitePawns() const;
//  uint totalWhiteKnights() const;
//  uint totalBlackBishops() const;
//  uint totalBlackQueens() const;
//  uint totalBlackRooks() const;
//  uint totalBlackPawns() const;
//  uint totalBlackKnights() const;

//public:

//	//! Computes a hash checksum for the board
//  //String ComputeChecksum() const;

//  void getCastlingRights(uint & castlingRights) const;

//  //! Makes a move
//  bool makeMove(const Move & newMove);

//	//! Generates all legal moves on the board in the current board position
//  uint generateLegalMoves(MoveList & moveList);

//	//! Generates all legal moves for the piece on the given row and column in the current board position
//  uint generateLegalMoves(uint row, uint col, MoveList & moveList);

//	//! Gets the piece type at the given row and column
//  PieceType getPieceType(uint row, uint col) const;
	
//  //! Determines if a piece exists at the given row and column
//  bool hasPiece(uint row, uint col) const;

//  //! Determines if the cell at the given row and column is under attack
//  bool isCellAttacked(uint row, uint col, bool whitePiece) const;

//	//! Sets the position of the board with the given fen string
//  void setPosition(const std::string & fenString);

//  //! Unmakes a move
//  bool unmakeMove(const Move & unmakeMove);

//private:

//	//! Retrieves the board index from the given row and column
//  unsigned int getIndex(uint row, uint col) const;

//	//! Retrieves the row and column of a given board index
//  void getRowCol(uint index, uint & row, uint & col) const;

//	//! Generates the pseudo-legal moves for all pieces based on the current board state
//  void generateCandidateMoves(MoveList & moveList);

//	//! Generates the pseudo-legal moves for the piece at the given row and column
//  void generateCandidateMoves(uint index, ulonglong pieceBB, MoveList & moveList);

//	//! Generates the moves for a white pawn on the given row and column
//  void generateWhitePawnMoves(uint index, MoveList & moveList);

//  void generateBlackPawnMoves(MoveList & moveList);
//  void generateWhitePawnMoves(MoveList & moveList);
//  void generateBlackPawnAttacks(uint index, MoveList & moveList);
//  void generateWhitePawnAttacks(uint index, MoveList & moveList);

//  //ulonglong generatePawnAttacks(uint index);
//  ulonglong generateBishopAttacks(uint index);
//  ulonglong generateRookAttacks(uint index);
//  ulonglong generateKingAttacks(uint index);
//  ulonglong generateKnightAttacks(uint index);

//	//! Generates the moves for a black pawn on the given row and column
//  void generateBlackPawnMoves(uint index, MoveList & moveList);

//	//! Generates the moves for a knight on the given row and column
//  void generateKnightMoves(uint index, ulonglong pieceBB, MoveList & moveList);

//  //! Generates the moves for a bishop on the given row and column
//  void generateBishopMoves(uint index, ulonglong pieceBB, MoveList & moveList);

//	//! Generates the moves for a rook on the given row and column
//  void generateRookMoves(uint index, uint row, uint col, ulonglong pieceBB, MoveList & moveList);

//	//! Generates the moves for a king on the given row and column
//  void generateKingMoves(uint index, ulonglong pieceBB, bool kingSide, bool queenSide, MoveList & moveList);

//	//! Initializes the postion of all pieces on the board as well as the board state
//	void initBoard();

//	//! Intializes the possible moves for all pieces
//	void initMoves();

//	//! Intializes the possible moves for pawns
//	void initPawnAttacks();

//	//! Intializes the possible moves for knights
//	void initKnightAttacks();

//	//! Intializes the possible moves for kings
//	void initKingAttacks();

//	//! Intializes the possible moves for rooks
//	void initRookAttacks();

//	//! Intializes the possible moves for queens
//  void initBishopAttacks();

//  //! Determines if the board state is legal during trimcandidateMoves
//  bool isLegal() const;

//  //! Loads the irrevisible state back in the object during an unmake move
//	void loadState();

//	//! Rotates a bitboard through 90 degrees
//  ulonglong rotate90(ulonglong bb);

//	//! Rotates a bitboard through 45 degrees
//  ulonglong rotate45(ulonglong bb, const uint * rotMatrix);

//	//! Rotates a bitboard through negative 45 degrees
//  ulonglong rotateNeg45(ulonglong bb, const uint * rotMatrix);

//  //! Saves the irrevisible state of the object during an make move
//	void saveState();

//  //! Creates moves from a bit board and adds them to supplied the move list
//  void serializeBitBoardMoves(ulonglong bitBoard, uint sourceIndex, MoveList & moveList);

//  void serializePawnMoves(ulonglong standardMoves, ulonglong promoMoves, uint sourceIndex, MoveList & moveList);

//	//! Determines if the given row and column reside in the board
//	bool squareInBoard(int row, int col);

//	//! Determines the set of legal moves from a set of pseudo-legal moves
//  void trimCandidateMoves(MoveList & candidateMoves, MoveList & legalMoves);

//	//! Updates the bit boards during a make/unmake function call
//  void updateBitBoards(uint sourceIndex, uint destIndex, PieceType pieceType, PieceType capturePiece = NoPiece);

//	void updatePieceEntriesMake(Move & newMove);
//	void updatePieceEntriesUnmake(Move & undoMove);

//	//! Debugging function to write out a bit board in a easy to read format
//  void writeBitBoard(ulonglong bitBoard) const;
	 
//	//! Debugging function to write out an occupancy in a easy to read format
//	void writeOccupancy(u_char occupancy) const;

//private:
//  struct PieceEntry
//  {
//    PieceType pieceType;
//    uint pieceIndex;
//  };

//private:
//  bool mWhiteToMove;                      //!< Determines whether it is white or black's turn to move
//  bool mBlackChecked;                     //!< Determines if black is currently in check
//  bool mWhiteChecked;                     //!< Determines if white is currently in check
//  bool mWhiteCheckmate;                   //!< Determines whether white is checkmated
//  bool mBlackCheckmate;                   //!< Determines whether white is checkmated
//	bool mStalemate;                        //!< Determines whether the game has reached a stalemate condition
//	bool mBlackCastleKingSide;              //!< Black's ability to castle king side
//	bool mBlackCastleQueenSide;             //!< Black's ability to castle queen side
//	bool mWhiteCastleKingSide;              //!< White's ability to castle king side
//	bool mWhiteCastleQueenSide;             //!< White's ability to castle queen side
//	bool mBlackEnPassant;                   //!< Black's ability to perform an en-passant pawn capture
//  bool mWhiteEnPassant;                   //!< White's ability to perform an en-passant pawn capture
//  uint mEnPassantCaptureCol;             //!< The en-passant capture column
//  uint mHalfMoveClock;                   //!< The number of moves since a pawn or capture move
//  uint mFullMoveCounter;
//  uint mWhiteKingRow;                    //!< Row position of the white king
//  uint mWhiteKingCol;                    //!< Column position of the white king
//  uint mBlackKingRow;                    //!< Row position of the black king
//  uint mBlackKingCol;                    //!< Column position of the black king
//  uint mTotalWhitePawns;
//  uint mTotalWhiteRooks;
//  uint mTotalWhiteBishops;
//  uint mTotalWhiteKnights;
//  uint mTotalWhiteQueens;
//  uint mTotalBlackPawns;
//  uint mTotalBlackRooks;
//  uint mTotalBlackBishops;
//  uint mTotalBlackKnights;
//  uint mTotalBlackQueens;
//	StalemateCondition mStalemateCondition; //!< Current stalemate condition of game
//	PieceEntry mWhitePieceEntries[16];
//	PieceEntry mBlackPieceEntries[16];
//	PieceType mPieces[64];                  //!< Redundant piece set for quick direct access to pieces at squares
//  std::list<uint> mSavedStates;           //!< List of saved states generated by makeMove

//	// Bit Boards for each piece type
//  ulonglong mWhitePawns;                  //!< White pawn positions
//  ulonglong mWhiteRooks;                  //!< White rook positions
//  ulonglong mWhiteBishops;                //!< White bishop positions
//  ulonglong mWhiteKnights;                //!< White knight positions
//  ulonglong mWhiteQueens;                 //!< White queen positions
//  ulonglong mWhiteKings;                  //!< White king positions
//  ulonglong mBlackPawns;                  //!< Black pawn positions
//  ulonglong mBlackRooks;                  //!< Black rook positions
//  ulonglong mBlackBishops;                //!< Black bishop positions
//  ulonglong mBlackKnights;                //!< Black knight positions
//  ulonglong mBlackQueens;                 //!< Black queen positions
//  ulonglong mBlackKings;                  //!< Black king positions

//	// Combination bit boards
//  ulonglong mWhitePieces;                 //!< Position of all white pieces
//  ulonglong mBlackPieces;                 //!< Black piece positions
//  ulonglong mAllPieces;                   //!< Positions of all pieces
//  ulonglong mEmptySquares;                //!< Position of all empty squares
//  ulonglong mRotate90AllPieces;
//  ulonglong mRotate45LeftPieces;
//  ulonglong mRotate45RightPieces;
//  ulonglong mRotate90BB[64];
//  ulonglong mRotate45RightBB[64];
//  ulonglong mRotate45LeftBB[64];

//	// Attack matrices
//  ulonglong mRankAttacks[8][256];         //!< Matrix of attacks on ranks for all squares and occupancies
//  ulonglong mFileAttacks[8][256];
//  ulonglong mBishopAttacksNE[64][256];    //!< Matrix of A1-H8 bishop attacks for all squares and occupancies
//  ulonglong mBishopAttacksNW[64][256];    //!< Matrix of A8-H1 bishop attacks for all squares and occupancies
//  ulonglong mKnightAttacks[64];           //!< Matrix of knight attacks for all squares
//  ulonglong mKingAttacks[64];             //!< Matrix of knight attacks for all squares
//  ulonglong mPawnAttacks[2][64];          //!< Matrix of knight attacks for each color for all squares
//  uint mA1H8Shifts[64];
//  uint mA1H8Mask[64];
//  uint mA8H1Shifts[64];
//  uint mA8H1Mask[64];
//};

//#endif // #ifndef BITBOARD_H
