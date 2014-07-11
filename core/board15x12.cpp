#include "board15x12.h"
//#include "md5.h"
#include "move.h"
#include "movelist.h"
#include "fenparser.h"

#include <map>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

static const uint WHITE_ROW = 0;
static const uint BLACK_ROW = 7;
static const uint WHITE_PAWN_START_ROW = 1;
static const uint BLACK_PAWN_START_ROW = 6;
static const uint WHITE_KING_START_ROW = 0;
static const uint BLACK_KING_START_ROW = 7;
static const uint WHITE_ENPASSANT_CHECK_ROW = 4;
static const uint BLACK_ENPASSANT_CHECK_ROW = 3;
static const uint WHITE_ENPASSANT_CAPTURE_ROW = 5;
static const uint BLACK_ENPASSANT_CAPTURE_ROW = 2;
static const uint KING_CASTLE_ROOK_COL = 5;
static const uint QUEEN_CASTLE_ROOK_COL = 3;
static const uint WHITE_PROMOTION_ROW = 7;
static const uint BLACK_PROMOTION_ROW = 0;

static const int rookIncr[]   = {1, 15, -1, -15};
static const int bishopIncr[] = {16, 14, -14, -16};
static const int knightIncr[] = {17, 31, 29, 13, -17, -31, -29, -13};
static const int kingIncr[]   = {1, 16, 15, 14, -1, -14, -15, -16};
static const int pawnIncr[]   = {14, 16};

//! Allows a loop through the indicies as though the board was a standard 64 square board
static const uint indexList[] =
{
	34,  35,  36,  37,  38,  39,  40,  41,
	49,  50,  51,  52,  53,  54,  55,  56,
	64,  65,  66,  67,  68,  69,  70,  71,
	79,  80,  81,  82,  83,  84,  85,  86,
	94,  95,  96,  97,  98,  99, 100, 101,
	109, 110, 111, 112, 113, 114, 115, 116,
	124, 125, 126, 127, 128, 129, 130, 131,
	139, 140, 141, 142, 143, 144, 145, 146
};

Board15x12::Board15x12(void)
{
	initBoard();
	initMoves();
}

Board15x12::~Board15x12(void)
{
}

bool Board15x12::canBlackCastleQueenSide() const
{
  return mBlackCastleQueenSide;
}

bool Board15x12::canBlackCastleKingSide() const
{
  return mBlackCastleKingSide;
}

bool Board15x12::canBlackEnPassant() const
{
  return mBlackEnPassant;
}

bool Board15x12::canWhiteCastleQueenSide() const
{
  return mWhiteCastleQueenSide;
}

bool Board15x12::canWhiteCastleKingSide() const
{
  return mWhiteCastleKingSide;
}

bool Board15x12::canWhiteEnPassant() const
{
  return mWhiteEnPassant;
}

uint Board15x12::enPassantCaptureCol() const
{
  return mEnPassantCaptureCol;
}

////! Computes a hash checksum for the board
//String Board15x12::ComputeChecksum() const
//{
//	MD5 md5;

//	unsigned char buffer[64] = {0};
//	for (int i = 0; i < 64; i++)
//	{
//		unsigned char sum = (unsigned char)i + (unsigned char)(mPieces[i]);
//		buffer[i] = sum;
//	}

//	md5.update(buffer, 64);
//	md5.finalize();

//	return md5.hexdigest();
//}

uint Board15x12::generateLegalMoves(uint row, uint col, MoveList & moveList)
{
	MoveList candidateMoves;

  uint index = 15 * row + col + 34;
	PieceType pieceType = mPieces[index];

	if (pieceType == NoPiece) return 0;
  if (isWhitePiece(pieceType) && !mWhiteToMove) return 0;
  if (isBlackPiece(pieceType) && mWhiteToMove) return 0;

	switch (pieceType)
	{
		case WhitePawn:
			generateWhitePawnMoves(index, row, col, candidateMoves);
			break;
		case WhiteKnight:
			generateKnightMoves(index, row, col, pieceType, candidateMoves);
			break;
		case WhiteBishop:
			generateBishopMoves(index, row, col, pieceType, candidateMoves);
			break;
		case WhiteRook:
			generateRookMoves(index, row, col, pieceType, candidateMoves);
			break;
		case WhiteQueen:
			generateBishopMoves(index, row, col, pieceType, candidateMoves);
			generateRookMoves(index, row, col, pieceType, candidateMoves);
			break;
		case WhiteKing:
			generateKingMoves(index, row, col, pieceType, candidateMoves);
			break;
		case BlackPawn:
			generateBlackPawnMoves(index, row, col, candidateMoves);
			break;
		case BlackKnight:
			generateKnightMoves(index, row, col, pieceType, candidateMoves);
			break;
		case BlackBishop:
			generateBishopMoves(index, row, col, pieceType, candidateMoves);
			break;
		case BlackRook:
			generateRookMoves(index, row, col, pieceType, candidateMoves);
			break;
		case BlackQueen:
			generateBishopMoves(index, row, col, pieceType, candidateMoves);
			generateRookMoves(index, row, col, pieceType, candidateMoves);
			break;
		case BlackKing:
			generateKingMoves(index, row, col, pieceType, candidateMoves);
			break;
    default:
      break;
	}

  for (uint i = 0; i < candidateMoves.totalMoves(); i++) {
    Move candidateMove(candidateMoves.moveAt(i));

		makeMove(candidateMove);
		if (isLegal())
      moveList.addMove(candidateMove);
		unmakeMove(candidateMove);
	}

  return moveList.totalMoves();
}

uint Board15x12::generateLegalMoves(MoveList & moveList)
{
	MoveList candidateMoves;
  if (mWhiteToMove) {
    for (uint i = 0; i < 64; i++) {
      uint index = indexList[i];
			PieceType pieceType = mPieces[index];
      if (isWhitePiece(pieceType)) {
        uint sourceRow, sourceCol;
				getRowCol(index, sourceRow, sourceCol);

        switch (pieceType) {
				case WhitePawn:
					generateWhitePawnMoves(index, sourceRow, sourceCol, candidateMoves);
					break;
				case WhiteKnight:
          for (std::list<uint>::iterator itr = mKnightMoves[i].begin(); itr != mKnightMoves[i].end(); ++itr) {
            uint destIndex(*itr);
            if (!isWhitePiece(mPieces[destIndex])) {
              uint destRow, destCol;
							getRowCol(destIndex, destRow, destCol);

							Move newMove(sourceRow, sourceCol, destRow, destCol, mPieces[destIndex]);
              candidateMoves.addMove(newMove);
						}
					}
					break;
				case WhiteBishop:
          for (uint i = 0; i < 4; i++) {
            uint destIndex, destRow, destCol;
            for (destIndex = index + bishopIncr[i]; mPieces[destIndex] == NoPiece; destIndex += bishopIncr[i]) {
							if (mPieces[destIndex] == InvalidSquare)
								break;

							getRowCol(destIndex, destRow, destCol);
							Move newMove(sourceRow, sourceCol, destRow, destCol);
              candidateMoves.addMove(newMove);
						}

            if (isBlackPiece(mPieces[destIndex])) {
							getRowCol(destIndex, destRow, destCol);
							Move newMove(sourceRow, sourceCol, destRow, destCol, mPieces[destIndex]);
              candidateMoves.addMove(newMove);
						}
					}
					//generateBishopMoves(index, sourceRow, sourceCol, pieceType, candidateMoves);
					break;
				case WhiteRook:
					//generateRookMoves(index, sourceRow, sourceCol, pieceType, candidateMoves);
          for (uint i = 0; i < 4; i++) {
            uint destIndex, destRow, destCol;

            for (destIndex = index + rookIncr[i]; mPieces[destIndex] == NoPiece; destIndex += rookIncr[i]) {
							if (mPieces[destIndex] == InvalidSquare)
								break;

							getRowCol(destIndex, destRow, destCol);
							Move newMove(sourceRow, sourceCol, destRow, destCol);
              candidateMoves.addMove(newMove);
						}

            if (isBlackPiece(mPieces[destIndex])) {
							getRowCol(destIndex, destRow, destCol);
							Move newMove(sourceRow, sourceCol, destRow, destCol, mPieces[destIndex]);
              candidateMoves.addMove(newMove);
						}
					}
					break;
				case WhiteQueen:
					generateBishopMoves(index, sourceRow, sourceCol, pieceType, candidateMoves);
					generateRookMoves(index, sourceRow, sourceCol, pieceType, candidateMoves);
					break;
				case WhiteKing:
					generateKingMoves(index, sourceRow, sourceCol, pieceType, candidateMoves);
					break;
        default:
          break;
				}

				/*if (pieceType == WhitePawn)
				{
					generateWhitePawnMoves(index, sourceRow, sourceCol, candidateMoves);
				}
				else if (pieceType == WhiteKnight || pieceType == WhiteKing)
				{
					std::vector<int> & attackIncr = mPieceAttacks[pieceType];
					for (std::size_t j = 0; j < attackIncr.size(); j++)
					{
            uint destIndex = index + attackIncr[j];
						PieceType otherPiece = mPieces[destIndex];
						if (otherPiece == NoPiece || IsEnemyPiece(pieceType, otherPiece))
						{
              uint destRow, destCol;
							getRowCol(destIndex, destRow, destCol);
							Move newMove(sourceRow, sourceCol, destRow, destCol, otherPiece);
              candidateMoves.addMove(newMove);
						}
					}
				}
				else
				{
					std::vector<int> & attackIncr = mPieceAttacks[pieceType];
					for (std::size_t j = 0; j < attackIncr.size(); j++)
					{
            uint destIndex, destRow, destCol;

						for (destIndex = index + attackIncr[j]; mPieces[destIndex] == NoPiece; destIndex += attackIncr[j])
						{
							if (mPieces[destIndex] == InvalidSquare) break;

							getRowCol(destIndex, destRow, destCol);
							Move newMove(sourceRow, sourceCol, destRow, destCol);
              candidateMoves.addMove(newMove);
						}

						getRowCol(destIndex, destRow, destCol);
						if (IsEnemyPiece(pieceType, mPieces[destIndex]))
						{
							Move newMove(sourceRow, sourceCol, destRow, destCol, mPieces[destIndex]);
              candidateMoves.addMove(newMove);
						}
					}
				}*/
			}
		}
	}
  else {
    for (uint i = 0; i < 64; i++) {
      uint index = indexList[i];
      uint sourceRow, sourceCol;
			getRowCol(index, sourceRow, sourceCol);

			PieceType pieceType = mPieces[index];
      switch (pieceType) {
			case BlackPawn:
				generateBlackPawnMoves(index, sourceRow, sourceCol, candidateMoves);
				break;
			case BlackKnight:
        for (std::list<uint>::iterator itr = mKnightMoves[i].begin(); itr != mKnightMoves[i].end(); ++itr) {
          uint destIndex(*itr);
          if (!isBlackPiece(mPieces[destIndex])) {
						Move newMove(sourceRow, sourceCol, getRow(destIndex), getCol(destIndex), mPieces[destIndex]);
            candidateMoves.addMove(newMove);
					}
				}
				break;
			case BlackBishop:
				//generateBishopMoves(index, sourceRow, sourceCol, pieceType, candidateMoves);
        for (uint i = 0; i < 4; i++) {
          uint destIndex, destRow, destCol;
          for (destIndex = index + bishopIncr[i]; mPieces[destIndex] == NoPiece; destIndex += bishopIncr[i]) {
						if (mPieces[destIndex] == InvalidSquare)
							break;

						getRowCol(destIndex, destRow, destCol);
						Move newMove(sourceRow, sourceCol, destRow, destCol);
            candidateMoves.addMove(newMove);
					}

          if (isWhitePiece(mPieces[destIndex])) {
						getRowCol(destIndex, destRow, destCol);
						Move newMove(sourceRow, sourceCol, destRow, destCol, mPieces[destIndex]);
            candidateMoves.addMove(newMove);
					}
				}
				break;
			case BlackRook:
				//generateRookMoves(index, sourceRow, sourceCol, pieceType, candidateMoves);
        for (uint i = 0; i < 4; i++) {
          uint destIndex, destRow, destCol;
          for (destIndex = index + rookIncr[i]; mPieces[destIndex] == NoPiece; destIndex += rookIncr[i]) {
						if (mPieces[destIndex] == InvalidSquare)
							break;

						getRowCol(destIndex, destRow, destCol);
						Move newMove(sourceRow, sourceCol, destRow, destCol);
            candidateMoves.addMove(newMove);
					}

          if (isWhitePiece(mPieces[destIndex])) {
						getRowCol(destIndex, destRow, destCol);
						Move newMove(sourceRow, sourceCol, destRow, destCol, mPieces[destIndex]);
            candidateMoves.addMove(newMove);
					}
				}
				break;
			case BlackQueen:
				generateBishopMoves(index, sourceRow, sourceCol, pieceType, candidateMoves);
				generateRookMoves(index, sourceRow, sourceCol, pieceType, candidateMoves);
				break;
			case BlackKing:
				generateKingMoves(index, sourceRow, sourceCol, pieceType, candidateMoves);
				break;
      default:
        break;
			}
		}
	}

  for (uint i = 0; i < candidateMoves.totalMoves(); i++) {
    Move candidateMove(candidateMoves.moveAt(i));

		makeMove(candidateMove);
    if (isLegal())
      moveList.addMove(candidateMove);
		unmakeMove(candidateMove);
	}

  return moveList.totalMoves();
}

uint Board15x12::getBlackKingRow() const
{
  return mBlackKingRow;
}

uint Board15x12::getBlackKingCol() const
{
  return mBlackKingCol;
}

uint Board15x12::getCol(uint index) const
{
  return index - 34 -15*getRow(index);
}

void Board15x12::getCastlingRights(uint & castlingRights) const
{
  if (canWhiteCastleKingSide())
    castlingRights |= CastleWhiteKing;
  if (canWhiteCastleQueenSide())
    castlingRights |= CastleWhiteQueen;
  if (canBlackCastleKingSide())
    castlingRights |= CastleBlackKing;
  if (canBlackCastleQueenSide())
    castlingRights |= CastleBlackQueen;
}

uint Board15x12::getIndex(uint row, uint col) const
{
  return (15*row + col + 34);
}

//! Gets the piece type at the given row and column
PieceType Board15x12::getPieceType(uint row, uint col) const
{
  uint index = 15 * row + col + 34;
  return mPieces[index];
}

uint Board15x12::getRow(uint index) const
{
  return ((index-34)*9) >> 7;
}

void Board15x12::getRowCol(uint index, uint & row, uint & col) const
{
  row = ((index - 34) * 9) >> 7;
  col = index - 34  - 15 * row;
}

uint Board15x12::getWhiteKingRow() const
{
  return mWhiteKingRow;
}

uint Board15x12::getWhiteKingCol() const
{
  return mWhiteKingCol;
}

uint Board15x12::halfMoveClock() const
{
  return mHalfMoveClock;
}

bool Board15x12::isBlackChecked() const
{
  return mBlackChecked;
}

bool Board15x12::isBlackMated() const
{
  return mBlackMated;
}

bool Board15x12::isBlackToMove() const
{
  return !mWhiteToMove;
}

bool Board15x12::isStalemate() const
{
  return mStalemate;
}

bool Board15x12::isWhiteChecked() const
{
  return mWhiteChecked;
}

bool Board15x12::isWhiteMated() const
{
  return mWhiteMated;
}

bool Board15x12::isWhiteToMove() const
{
  return mWhiteToMove;
}

bool Board15x12::isCellAttacked(uint row, uint col, bool whitePiece) const
{
	int pawnOffset = whitePiece ? 1 : -1;
	PieceType kingPieceType = (whitePiece) ?  BlackKing : WhiteKing;
	PieceType pawnPieceType = (whitePiece) ? BlackPawn : WhitePawn;
	PieceType bishopPieceType = (whitePiece) ? BlackBishop : WhiteBishop;
	PieceType queenPieceType = (whitePiece) ? BlackQueen : WhiteQueen;
	PieceType rookPieceType = (whitePiece) ? BlackRook : WhiteRook;
	PieceType knightPieceType = (whitePiece) ? BlackKnight : WhiteKnight;
	
  uint sourceIndex = getIndex(row, col);

	// Check for rook/bishop attacks
  for (uint i = 0; i < 4; i++) {
    uint destIndex;

		for (destIndex = sourceIndex + rookIncr[i]; mPieces[destIndex] == NoPiece; destIndex += rookIncr[i]);

		if (mPieces[destIndex] == queenPieceType) 
			return true;
		if (mPieces[destIndex] == rookPieceType)
			return true;

		for (destIndex = sourceIndex + bishopIncr[i]; mPieces[destIndex] == NoPiece; destIndex += bishopIncr[i]);

		if (mPieces[destIndex] == queenPieceType) 
			return true;
		if (mPieces[destIndex] == bishopPieceType)
			return true;
	}

	// Check for knight/knight attacks
  for (uint i = 0; i < 8; i++) {
    uint destIndex = sourceIndex + knightIncr[i];
		if (mPieces[destIndex] == knightPieceType) 
			return true;

		destIndex = sourceIndex + kingIncr[i];
		if (mPieces[destIndex] == kingPieceType) 
			return true;
	}

	// Check for pawn attacks
  for (uint i = 0; i < 2; i++) {
    uint destIndex = sourceIndex + pawnOffset * pawnIncr[i];
		if (mPieces[destIndex] == pawnPieceType) 
			return true;
	}

	return false;
}

//! Sets the position of the board with the given fen string
void Board15x12::setPosition(const std::string & fenString)
{
  initBoard();

  FenData fenData;
  if (!readFenString(fenString, fenData))
    return;

  mWhiteToMove = fenData.mWhiteToMove;
  mBlackCastleKingSide = fenData.mBlackCastleKingSide;
  mBlackCastleQueenSide = fenData.mBlackCastleQueenSide;
  mWhiteCastleKingSide = fenData.mWhiteCastleKingSide;
  mWhiteCastleQueenSide = fenData.mWhiteCastleQueenSide;
  mEnPassantCaptureCol = fenData.mEnPassantCol;
  mHalfMoveClock = fenData.mHalfMoveClock;
  mFullMoveCounter = fenData.mFullMoveCounter;
  for (uint row = 0; row < 8; row++) {
    for (uint col = 0; col < 8; col++) {
      uint index = getIndex(row, col);
      mPieces[index] = fenData.mPieceType[row][col];
    }
  }

  // Update our internal piece counters and the rows and columns for each king
  for (uint index = 0; index < 180; index++) {
    PieceType pieceType = mPieces[index];
    if (isPiece(pieceType))
    {
      //++mTotalPieces;

      //if (isBlackPiece(pieceType))
      //  ++mTotalBlackPieces;

      //if (isWhitePiece(pieceType))
      //	++mTotalWhitePieces;

      uint row, col;
      getRowCol(index, row, col);

      switch (pieceType) {
      case WhitePawn:
        mTotalWhitePawns++;
        break;
      case WhiteKnight:
        mTotalWhiteKnights++;
        break;
      case WhiteBishop:
        mTotalWhiteBishops++;
        break;
      case WhiteRook:
        mTotalWhiteRooks++;
        break;
      case WhiteQueen:
        mTotalWhiteQueens++;
        break;
      case WhiteKing:
        mWhiteKingRow = row;
        mWhiteKingCol = col;
        break;
      case BlackPawn:
        mTotalBlackPawns++;
        break;
      case BlackKnight:
        mTotalBlackKnights++;
        break;
      case BlackBishop:
        mTotalBlackBishops++;
        break;
      case BlackRook:
        mTotalBlackRooks++;
        break;
      case BlackQueen:
        mTotalBlackQueens++;
        break;
      case BlackKing:
        mBlackKingRow = row;
        mBlackKingCol = col;
        break;
      default:
        break;
      }
    }
  }

  mBlackChecked = isCellAttacked(mBlackKingRow, mBlackKingCol, false);
  mWhiteChecked = isCellAttacked(mWhiteKingRow, mWhiteKingCol, true);
}

uint Board15x12::totalWhiteBishops() const
{
  return mTotalWhiteBishops;
}

uint Board15x12::totalWhiteQueens() const
{
  return mTotalWhiteQueens;
}

uint Board15x12::totalWhiteRooks() const
{
  return mTotalWhiteRooks;
}

uint Board15x12::totalWhitePawns() const
{
  return mTotalWhitePawns;
}

uint Board15x12::totalWhiteKnights() const
{
  return mTotalWhiteKnights;
}

uint Board15x12::totalBlackBishops() const
{
  return mTotalBlackBishops;
}

uint Board15x12::totalBlackQueens() const
{
  return mTotalBlackQueens;
}

uint Board15x12::totalBlackRooks() const
{
  return mTotalBlackRooks;
}

uint Board15x12::totalBlackPawns() const
{
  return mTotalBlackPawns;
}

uint Board15x12::totalBlackKnights() const
{
  return mTotalBlackKnights;
}

void Board15x12::generateBishopMoves(uint index, uint sourceRow, uint sourceCol, PieceType pieceType, MoveList & moveList)
{
  for (uint i = 0; i < 4; i++) {
    uint destIndex, destRow, destCol;

    for (destIndex = index + bishopIncr[i]; mPieces[destIndex] == NoPiece; destIndex += bishopIncr[i]) {
			if (mPieces[destIndex] == InvalidSquare)
				break;

			getRowCol(destIndex, destRow, destCol);
			Move newMove(sourceRow, sourceCol, destRow, destCol);
      moveList.addMove(newMove);
		}

    if (isEnemyPiece(pieceType, mPieces[destIndex])) {
			getRowCol(destIndex, destRow, destCol);
			Move newMove(sourceRow, sourceCol, destRow, destCol, mPieces[destIndex]);
      moveList.addMove(newMove);
		}
	}
}

void Board15x12::generateBlackPawnMoves(uint index, uint sourceRow, uint sourceCol, MoveList & moveList)
{
	static const PieceType promoPieces[] = {BlackQueen, BlackRook, BlackBishop, BlackKnight};

  uint destRow, destCol;
  uint destIndex = index - 15;

	bool singlePush = (mPieces[destIndex] == NoPiece);
  if (singlePush) {
		getRowCol(destIndex, destRow, destCol);
		if (sourceRow == 1)
			for (int i = 0; i < 4; i++)
        moveList.addMove(Move(sourceRow, sourceCol, destRow, destCol, NoPiece, promoPieces[i]));
		else
      moveList.addMove(Move(sourceRow, sourceCol, destRow, destCol));

		destIndex = index - 30;
    if (sourceRow == 6 && mPieces[destIndex] == NoPiece) {
			getRowCol(destIndex, destRow, destCol);

			Move newMove(sourceRow, sourceCol, destRow, destCol);
      newMove.setDoublePawnPush();
      moveList.addMove(newMove);
		}
	}

  for (uint i = 0; i < 2; i++)
	{
		destIndex = index - pawnIncr[i];
		PieceType capturePiece = mPieces[destIndex];

    if (mPieces[destIndex] != InvalidSquare && isWhitePiece(capturePiece)) {
			getRowCol(destIndex, destRow, destCol);

			if (sourceRow == 1)
				for (int i = 0; i < 4; i++)
          moveList.addMove(Move(sourceRow, sourceCol, destRow, destCol, capturePiece, promoPieces[i]));
			else
        moveList.addMove(Move(sourceRow, sourceCol, destRow, destCol, capturePiece));
		}
	}

	// Check en-passant
  if (mBlackEnPassant && sourceRow == 3) {
    if (sourceCol != 7) {
      PieceType pieceType = getPieceType(sourceRow, sourceCol+1);
      if (mEnPassantCaptureCol == sourceCol + 1 && pieceType == WhitePawn)  {
				Move newMove(sourceRow, sourceCol, sourceRow-1, sourceCol+1, WhitePawn);
        newMove.setEnPassant();
        moveList.addMove(newMove);
			}
		}

    if (sourceCol != 0) {
      PieceType pieceType = getPieceType(sourceRow, sourceCol-1);
      if (mEnPassantCaptureCol == sourceCol - 1 && pieceType == WhitePawn)  {
				Move newMove(sourceRow, sourceCol, sourceRow-1, sourceCol-1, WhitePawn);
        newMove.setEnPassant();
        moveList.addMove(newMove);
			}
		}
	}
}

void Board15x12::generateKingMoves(uint index, uint sourceRow, uint sourceCol, PieceType pieceType, MoveList & moveList)
{
  for (uint i = 0; i < 8; i++) {
    uint destRow, destCol;

    uint destIndex = index + kingIncr[i];
		getRowCol(destIndex, destRow, destCol);

		PieceType otherPiece = mPieces[destIndex];
    if (otherPiece == NoPiece || isEnemyPiece(pieceType, otherPiece)) {
			Move newMove(sourceRow, sourceCol, destRow, destCol, otherPiece);
      moveList.addMove(newMove);
		}
	}

	bool queenSide = (mWhiteToMove) ? mWhiteCastleQueenSide : mBlackCastleQueenSide;
	bool kingSide = (mWhiteToMove) ? mWhiteCastleKingSide : mBlackCastleKingSide;
  uint rowCheck = (mWhiteToMove) ? WHITE_KING_START_ROW : BLACK_KING_START_ROW;

	if (!kingSide && !queenSide)
		return;

  if (kingSide && getPieceType(rowCheck, 5) == NoPiece && getPieceType(rowCheck, 6) == NoPiece)  {
		bool canCastle = true;
    uint attackCol = 4;
    while (attackCol <= 6) {
      if (isCellAttacked(rowCheck, attackCol, mWhiteToMove)) {
				canCastle = false;
				break;
			}
			attackCol++;
		}

    if (canCastle) {
			Move newMove(rowCheck, 4, rowCheck, 6);
      newMove.setKingCastle();
      moveList.addMove(newMove);
		}
	}
				
  if (queenSide && getPieceType(rowCheck,3) == NoPiece && getPieceType(rowCheck,2) == NoPiece && getPieceType(rowCheck,1) == NoPiece) {
		bool canCastle = true;
    uint attackCol = 4;
    while (attackCol >= 2) {
      if (isCellAttacked(rowCheck, attackCol, mWhiteToMove)) {
				canCastle = false;
				break;
			}
			attackCol--;
		}

    if (canCastle) {
			Move newMove(rowCheck, 4, rowCheck, 2);
      newMove.setQueenCastle();
      moveList.addMove(newMove);
		}
	}
}

void Board15x12::generateKnightMoves(uint index, uint sourceRow, uint sourceCol, PieceType pieceType, MoveList & moveList)
{
  for (uint i = 0; i < 8; i++) {
    uint destRow, destCol;

    uint destIndex = index + knightIncr[i];
		getRowCol(destIndex, destRow, destCol);

		PieceType otherPiece = mPieces[destIndex];
    if (otherPiece == NoPiece || isEnemyPiece(pieceType, otherPiece)) {
			Move newMove(sourceRow, sourceCol, destRow, destCol, otherPiece);
      moveList.addMove(newMove);
		}
	}
}

void Board15x12::generateRookMoves(uint index, uint sourceRow, uint sourceCol, PieceType pieceType, MoveList & moveList)
{
  for (uint i = 0; i < 4; i++) {
    uint destIndex, destRow, destCol;
    for (destIndex = index + rookIncr[i]; mPieces[destIndex] == NoPiece; destIndex += rookIncr[i]) {
			if (mPieces[destIndex] == InvalidSquare) break;

			getRowCol(destIndex, destRow, destCol);
			Move newMove(sourceRow, sourceCol, destRow, destCol);
      moveList.addMove(newMove);
		}

		getRowCol(destIndex, destRow, destCol);
    if (isEnemyPiece(pieceType, mPieces[destIndex])) {
			Move newMove(sourceRow, sourceCol, destRow, destCol, mPieces[destIndex]);
      moveList.addMove(newMove);
		}
	}
}

void Board15x12::generateWhitePawnMoves(uint index, uint sourceRow, uint sourceCol, MoveList & moveList)
{
	static const PieceType promoPieces[] = {WhiteQueen, WhiteRook, WhiteBishop, WhiteKnight};

  uint destRow, destCol;

	// Generate push moves
  uint destIndex = index + 15;
	getRowCol(destIndex, destRow, destCol);
	bool singlePush = (mPieces[destIndex] == NoPiece);
  if (singlePush) {
		if (sourceRow == 6)
			for (int i = 0; i < 4; i++)
        moveList.addMove(Move(sourceRow, sourceCol, destRow, destCol, NoPiece, promoPieces[i]));
		else
      moveList.addMove(Move(sourceRow, sourceCol, destRow, destCol));

		destIndex = index + 30;
    if (sourceRow == 1 && mPieces[destIndex] == NoPiece) {
			getRowCol(destIndex, destRow, destCol);

			Move newMove(sourceRow, sourceCol, destRow, destCol);
      newMove.setDoublePawnPush();
      moveList.addMove(newMove);
		}
	}

	// Generate capture moves
  for (uint i = 0; i < 2; i++) {
		destIndex = index + pawnIncr[i];
		PieceType capturePiece = mPieces[destIndex];
    if (mPieces[destIndex] != InvalidSquare && isBlackPiece(capturePiece)) {
			getRowCol(destIndex, destRow, destCol);

			if (sourceRow == 6)
				for (int i = 0; i < 4; i++)
          moveList.addMove(Move(sourceRow, sourceCol, destRow, destCol, capturePiece, promoPieces[i]));
			else
        moveList.addMove(Move(sourceRow, sourceCol, destRow, destCol, capturePiece));
		}
	}

	// Check for en passant moves
  if (mWhiteEnPassant && sourceRow == 4) {
    if (sourceCol != 7) {
      PieceType pieceType = getPieceType(sourceRow, sourceCol+1);
			if (mEnPassantCaptureCol == sourceCol + 1 && pieceType == BlackPawn) 
			{
				Move newMove(sourceRow, sourceCol, sourceRow+1, sourceCol+1, BlackPawn);
        newMove.setEnPassant();
        moveList.addMove(newMove);
			}
		}

    if (sourceCol != 0) {
      PieceType pieceType = getPieceType(sourceRow, sourceCol-1);
      if (mEnPassantCaptureCol == sourceCol - 1 && pieceType == BlackPawn)  {
				Move newMove(sourceRow, sourceCol, sourceRow+1, sourceCol-1, BlackPawn);
        newMove.setEnPassant();
        moveList.addMove(newMove);
			}
		}
	}
}

void Board15x12::initBoard()
{
	mWhiteToMove = true;
  mWhiteCastleKingSide = mWhiteCastleQueenSide = mWhiteEnPassant = true;
  mBlackCastleKingSide = mBlackCastleQueenSide = mBlackEnPassant = true;
  mWhiteMated = mBlackMated = mStalemate = false;
	mStalemateCondition = NoStalemate;
	mEnPassantCaptureCol = 8;
	mWhiteKingCol = mBlackKingCol = 4;
	mBlackKingRow = BLACK_KING_START_ROW;
	mWhiteKingRow = WHITE_KING_START_ROW;

  for (uint i = 0; i < 15*12; i++)
		mPieces[i] = InvalidSquare;

  for (uint i = 34; i <= 41; i++) {
    for (uint j = 0; j < 8; j++)
			mPieces[j*15+i] = NoPiece;
	}

	mPieces[34] = mPieces[41] = WhiteRook;
	mPieces[35] = mPieces[40] = WhiteKnight;
	mPieces[36] = mPieces[39] = WhiteBishop;
	mPieces[37] = WhiteQueen;
	mPieces[38] = WhiteKing;

	mPieces[34 + 15*7] = mPieces[41 + 15*7] = BlackRook;
	mPieces[35 + 15*7] = mPieces[40 + 15*7] = BlackKnight;
	mPieces[36 + 15*7] = mPieces[39 + 15*7] = BlackBishop;
	mPieces[37 + 15*7] = BlackQueen;
	mPieces[38 + 15*7] = BlackKing;

  for (uint col = 0; col < 8; col++) {
		mPieces[34+15+col] = WhitePawn;
		mPieces[34 + 15*6 + col] = BlackPawn;

		mWhitePieces.push_back(34+15+col);
		mBlackPieces.push_back(34+15*6+col);
	}

  for (int i = 34; i < 41; i++) {
		mWhitePieces.push_back(i);
		mBlackPieces.push_back(i+15*7);
	}

	static const int rookIncr[]   = {1, 15, -1, -15};
	static const int bishopIncr[] = {16, 14, -14, -16};
	static const int knightIncr[] = {17, 31, 29, 13, -17, -31, -29, -13};
	static const int kingIncr[]   = {1, 16, 15, 14, -1, -14, -15, -16};
	static const int pawnIncr[]   = {14, 16};

  for (int i = 0; i < 2; i++) {
		mPieceAttacks[WhitePawn].push_back(pawnIncr[i]);
		mPieceAttacks[BlackPawn].push_back(pawnIncr[i]);
	}

  for (int i = 0; i < 4; i++) {
		mPieceAttacks[WhiteRook].push_back(rookIncr[i]);
		mPieceAttacks[BlackRook].push_back(rookIncr[i]);
		mPieceAttacks[WhiteBishop].push_back(bishopIncr[i]);
		mPieceAttacks[BlackBishop].push_back(bishopIncr[i]);
		mPieceAttacks[WhiteQueen].push_back(rookIncr[i]);
		mPieceAttacks[WhiteQueen].push_back(bishopIncr[i]);
		mPieceAttacks[BlackQueen].push_back(rookIncr[i]);
		mPieceAttacks[BlackQueen].push_back(bishopIncr[i]);
	}

  for (int i = 0; i < 8; i++) {
		mPieceAttacks[WhiteKnight].push_back(knightIncr[i]);
		mPieceAttacks[BlackKnight].push_back(knightIncr[i]);
		mPieceAttacks[WhiteKing].push_back(kingIncr[i]);
		mPieceAttacks[BlackKing].push_back(kingIncr[i]);
	}
}

void Board15x12::initMoves()
{
  for (uint iSquare = 0; iSquare < 64; iSquare++) {
		// King Moves
    uint index = indexList[iSquare];
    for (uint i = 0; i < 8; i++) {
      uint destIndex = index + knightIncr[i];
			if (mPieces[destIndex] != InvalidSquare)
				mKnightMoves[iSquare].push_back(destIndex);
		}
	}
}

bool Board15x12::isLegal() const
{
  if (!mWhiteToMove && isCellAttacked(mWhiteKingRow, mWhiteKingCol, true))
		return false;

  if (mWhiteToMove && isCellAttacked(mBlackKingRow, mBlackKingCol, false))
		return false;

	return true;
}

void Board15x12::loadState()
{
  uint savedState = mSavedStates.back();
	mSavedStates.pop_back();

	mBlackCastleKingSide = ((savedState & 0x0001) == 0x0001);
	mBlackCastleQueenSide = ((savedState & 0x0002) == 0x0002);
	mWhiteCastleKingSide = ((savedState & 0x0004) == 0x0004);
	mWhiteCastleQueenSide = ((savedState & 0x0008) == 0x0008);
	mBlackEnPassant = ((savedState >> 4) & 0x0001) > 0;
	mWhiteEnPassant = ((savedState >> 4) & 0x0002) > 0;
	mEnPassantCaptureCol = ((savedState >> 6) & 0x0007);
	mHalfMoveClock = ((savedState >> 9) & 0x003f);
}

bool Board15x12::makeMove(const Move & newMove)
{
	saveState();

  uint sourceRow = newMove.sourceRow();
  uint sourceCol = newMove.sourceColumn();
  uint destRow = newMove.destinationRow();
  uint destCol = newMove.destinationColumn();
  uint sourceIndex = 15 * sourceRow + sourceCol + 34;
  uint destIndex = 15 * destRow + destCol + 34;
	PieceType pieceType = mPieces[sourceIndex];

	mPieces[destIndex] = pieceType;
	mPieces[sourceIndex] = NoPiece;

	mHalfMoveClock++;

	mWhiteEnPassant = mBlackEnPassant = false;
	mEnPassantCaptureCol = 8;

  switch (pieceType) {
	case WhitePawn:
		mHalfMoveClock = 0;
    if (newMove.isDoublePawnPush()) {
      if (destCol != 0) {
        uint epIndex = getIndex(BLACK_ENPASSANT_CHECK_ROW, destCol-1);
				PieceType otherPiece = mPieces[epIndex];
        if (otherPiece == BlackPawn)  {
					mBlackEnPassant = true;
					mEnPassantCaptureCol = destCol;
				}
			}

      if (destCol != 7) {
        uint epIndex = getIndex(BLACK_ENPASSANT_CHECK_ROW, destCol+1);
				PieceType otherPiece = mPieces[epIndex];
        if (otherPiece == BlackPawn)  {
					mBlackEnPassant = true;
					mEnPassantCaptureCol = destCol;
				}
			}
		}
    if (newMove.isPromotion()) {
      PieceType promotedPiece = newMove.promotedPiece();
			mPieces[destIndex] = promotedPiece;

			mTotalWhitePawns--;
      switch (promotedPiece) {
			case WhiteQueen:
				mTotalWhiteQueens++;
				break;
			case WhiteRook:
				mTotalWhiteRooks++;
				break;
			case WhiteBishop:
				mTotalWhiteBishops++;
				break;
			case WhiteKnight:
				mTotalWhiteKnights++;
				break;
      default:
        break;
			}
		}
		break;
	case WhiteKnight:
		break;
	case WhiteBishop:
		break;
	case WhiteRook:
		if (sourceCol == 0) mWhiteCastleQueenSide = false;
		if (sourceCol == 7) mWhiteCastleKingSide = false;
		break;
	case WhiteQueen:
		break;
	case WhiteKing:
		mWhiteKingRow = destRow;
		mWhiteKingCol = destCol;
		mWhiteCastleKingSide = mWhiteCastleQueenSide = false;
    if (newMove.isKingCastle()) {
			mPieces[getIndex(sourceRow, 7)] = NoPiece;
			mPieces[getIndex(sourceRow, KING_CASTLE_ROOK_COL)] = WhiteRook;
		}
    if (newMove.isQueenCastle()) {
			mPieces[getIndex(sourceRow, 0)] = NoPiece;
			mPieces[getIndex(sourceRow, QUEEN_CASTLE_ROOK_COL)] = WhiteRook;
		}
		break;
	case BlackPawn:
		mHalfMoveClock = 0;
    if (newMove.isDoublePawnPush()) {
      if (destCol != 0) {
        uint epIndex = getIndex(WHITE_ENPASSANT_CHECK_ROW, destCol-1);
				PieceType otherPiece = mPieces[epIndex];
        if (otherPiece == WhitePawn)  {
					mWhiteEnPassant = true;
					mEnPassantCaptureCol = destCol;
				}
			}

      if (destCol != 7) {
        uint epIndex = getIndex(WHITE_ENPASSANT_CHECK_ROW, destCol+1);
				PieceType otherPiece = mPieces[epIndex];
        if (otherPiece == WhitePawn) {
					mWhiteEnPassant = true;
					mEnPassantCaptureCol = destCol;
				}
			}
		}
    if (newMove.isPromotion()) {
      PieceType promotedPiece = newMove.promotedPiece();
			mPieces[destIndex] = promotedPiece;

			mTotalBlackPawns--;
      switch (promotedPiece) {
			case BlackQueen:
				mTotalBlackQueens++;
				break;
			case BlackRook:
				mTotalBlackRooks++;
				break;
			case BlackBishop:
				mTotalBlackBishops++;
				break;
			case BlackKnight:
				mTotalBlackKnights++;
				break;
      default:
        break;
			}
		}
		break;
	case BlackKnight:
		break;
  case BlackBishop:
		break;
	case BlackRook:
		if (sourceCol == 0) mBlackCastleQueenSide = false;
		if (sourceCol == 7) mBlackCastleKingSide = false;
		break;
	case BlackQueen:
		break;
	case BlackKing:
		mBlackKingRow = destRow;
		mBlackKingCol = destCol;
		mBlackCastleKingSide = mBlackCastleQueenSide = false;
    if (newMove.isKingCastle()) {
			mPieces[getIndex(sourceRow, 7)] = NoPiece;
			mPieces[getIndex(sourceRow, KING_CASTLE_ROOK_COL)] = BlackRook;
		}
    if (newMove.isQueenCastle())
		{
			mPieces[getIndex(sourceRow, 0)] = NoPiece;
			mPieces[getIndex(sourceRow, QUEEN_CASTLE_ROOK_COL)] = BlackRook;
		}
		break;
  default:
    break;
	}

  if (newMove.isCapture() && !newMove.isEnPassant()) {
		mHalfMoveClock = 0;

    PieceType capturePiece = newMove.capturedPiece();
    switch (capturePiece) {
		case WhitePawn:
			mTotalWhitePawns--;
			break;
		case WhiteKnight:
			mTotalWhiteKnights--;
			break;
		case WhiteBishop:
			mTotalWhiteBishops--;
			break;
		case WhiteRook:
			mTotalWhiteRooks--;
			if (destCol == 0) mWhiteCastleQueenSide = false;
			if (destCol == 7) mWhiteCastleKingSide = false;
			break;
		case WhiteQueen:
			mTotalWhiteQueens--;
			break;
		case WhiteKing:
			break;
		case BlackPawn:
			mTotalBlackPawns--;
			break;
		case BlackKnight:
			mTotalBlackKnights--;
			break;
		case BlackBishop:
			mTotalBlackBishops--;
			break;
		case BlackRook:
			mTotalBlackRooks--;
			if (destCol == 0) mBlackCastleQueenSide = false;
			if (destCol == 7) mBlackCastleKingSide = false;
			break;
		case BlackQueen:
			mTotalBlackQueens--;
			break;
		case BlackKing:
			break;
    default:
      break;
		}
	}

	// Handle en-passant capture 
  if (newMove.isEnPassant()) {
		mHalfMoveClock = 0;

    uint epIndex = getIndex(sourceRow, destCol);
		PieceType capturePiece = mPieces[epIndex];

		mPieces[epIndex] = NoPiece;
    switch (capturePiece) {
		case WhitePawn:
			mTotalWhitePawns--;
			break;
		case BlackPawn:
			mTotalBlackPawns--;
			break;
    default:
      break;
		}
	}

  if (!mWhiteToMove) {
    mWhiteChecked = isCellAttacked(mWhiteKingRow, mWhiteKingCol, true);
  }
  else {
    mBlackChecked = isCellAttacked(mBlackKingRow, mBlackKingCol, false);
  }

	mWhiteToMove = !mWhiteToMove;

  return true;
}

void Board15x12::saveState()
{
  uint savedState = 0;

	if (mBlackCastleKingSide) savedState |= 0x0001;
	if (mBlackCastleQueenSide) savedState |= 0x0002;
	if (mWhiteCastleKingSide) savedState |= 0x0004;
	if (mWhiteCastleQueenSide) savedState |= 0x0008;
	if (mBlackEnPassant) savedState |= 0x0010;
	if (mWhiteEnPassant) savedState |= 0x0020;
	savedState |= ( (mEnPassantCaptureCol & 0x0007) << 6);
	savedState |= ( (mHalfMoveClock & 0x003f) << 9);

	mSavedStates.push_back(savedState);
}

bool Board15x12::unmakeMove(const Move & undoMove)
{
  uint sourceRow = undoMove.sourceRow();
  uint sourceCol = undoMove.sourceColumn();
  uint destRow = undoMove.destinationRow();
  uint destCol = undoMove.destinationColumn();
  uint sourceIndex = 15*sourceRow + sourceCol + 34;
  uint destIndex = 15*destRow + destCol + 34;
	PieceType pieceType = mPieces[destIndex];

	mPieces[sourceIndex] = pieceType;
	mPieces[destIndex] = NoPiece;

  switch (pieceType) {
	case WhitePawn:
		break;
	case WhiteKnight:
    if (undoMove.isPromotion()) {
			mPieces[sourceIndex] = WhitePawn;
			mTotalWhiteKnights--;
			mTotalWhitePawns++;
		}
		break;
	case WhiteBishop:
    if (undoMove.isPromotion()) {
			mPieces[sourceIndex] = WhitePawn;
			mTotalWhiteBishops--;
			mTotalWhitePawns++;
		}
		break;
	case WhiteRook:
    if (undoMove.isPromotion()) {
			mPieces[sourceIndex] = WhitePawn;
			mTotalWhiteRooks--;
			mTotalWhitePawns++;
		}
		break;
	case WhiteQueen:
    if (undoMove.isPromotion()) {
			mPieces[sourceIndex] = WhitePawn;
			mTotalWhiteQueens--;
			mTotalWhitePawns++;
		}
		break;
	case WhiteKing:
		mWhiteKingRow = sourceRow;
		mWhiteKingCol = sourceCol;
    if (undoMove.isKingCastle()) {
			mPieces[getIndex(sourceRow, 7)] = WhiteRook;
			mPieces[getIndex(sourceRow, KING_CASTLE_ROOK_COL)] = NoPiece;
		}
    if (undoMove.isQueenCastle()) {
			mPieces[getIndex(sourceRow, 0)] = WhiteRook;
			mPieces[getIndex(sourceRow, QUEEN_CASTLE_ROOK_COL)] = NoPiece;
		}
		break;
  case BlackPawn:
		break;
	case BlackKnight:
    if (undoMove.isPromotion()) {
			mPieces[sourceIndex] = BlackPawn;
			mTotalBlackKnights--;
			mTotalBlackPawns++;
		}
		break;
	case BlackBishop:
    if (undoMove.isPromotion()) {
			mPieces[sourceIndex] = BlackPawn;
			mTotalBlackBishops--;
			mTotalBlackPawns++;
		}
		break;
	case BlackRook:
    if (undoMove.isPromotion()) {
			mPieces[sourceIndex] = BlackPawn;
			mTotalBlackRooks--;
			mTotalBlackPawns++;
		}
		break;
	case BlackQueen:
    if (undoMove.isPromotion()) {
			mPieces[sourceIndex] = BlackPawn;
			mTotalBlackQueens--;
			mTotalBlackPawns++;
		}
		break;
	case BlackKing:
		mBlackKingRow = sourceRow;
		mBlackKingCol = sourceCol;
    if (undoMove.isKingCastle()) {
			mPieces[getIndex(sourceRow, 7)] = BlackRook;
			mPieces[getIndex(sourceRow, KING_CASTLE_ROOK_COL)] = NoPiece;
		}
    if (undoMove.isQueenCastle()) {
			mPieces[getIndex(sourceRow, 0)] = BlackRook;
			mPieces[getIndex(sourceRow, QUEEN_CASTLE_ROOK_COL)] = NoPiece;
		}
		break;
  default:
    break;
	}

  if (undoMove.isCapture() && !undoMove.isEnPassant()) {
    PieceType capturePiece = undoMove.capturedPiece();
		mPieces[destIndex] = capturePiece;

    switch (capturePiece) {
		case WhitePawn:
			mTotalWhitePawns++;
			break;
		case WhiteKnight:
			mTotalWhiteKnights++;
			break;
		case WhiteBishop:
			mTotalWhiteBishops++;
			break;
		case WhiteRook:
			mTotalWhiteRooks++;
			break;
		case WhiteQueen:
			mTotalWhiteQueens++;
			break;
		case WhiteKing:
			break;
		case BlackPawn:
			mTotalBlackPawns++;
			break;
		case BlackKnight:
			mTotalBlackKnights++;
			break;
		case BlackBishop:
			mTotalBlackBishops++;
			break;
		case BlackRook:
			mTotalBlackRooks++;
			break;
		case BlackQueen:
			mTotalBlackQueens++;
			break;
		case BlackKing:
			break;
    default:
      break;
		}
	}

  if (undoMove.isEnPassant())
	{
    uint epIndex = getIndex(sourceRow, destCol);

    PieceType capturePiece = undoMove.capturedPiece();
		mPieces[epIndex] = capturePiece;

    switch (capturePiece) {
		case WhitePawn:
			mTotalWhitePawns++;
			break;
		case BlackPawn:
			mTotalBlackPawns++;
			break;
    default:
      break;
		}
	}

  if (!mWhiteToMove)
    mBlackChecked = isCellAttacked(mBlackKingRow, mBlackKingCol, false);
  else
    mWhiteChecked = isCellAttacked(mWhiteKingRow, mWhiteKingCol, true);

	mWhiteToMove = !mWhiteToMove;

	loadState();

  return true;
}
