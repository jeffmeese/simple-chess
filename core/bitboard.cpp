#include "bitboard.h"
#include "fenparser.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#define C64(constantU64) constantU64##ULL

#define ENPASSANT
#define CASTLE
#define PROMOS
#define CHECKS

enum Square
{
	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8
};

static const uint rot90Matrix[] =
{
	A1, A2, A3, A4, A5, A6, A7, A8,
	B1, B2, B3, B4, B5, B6, B7, B8,
	C1, C2, C3, C4, C5, C6, C7, C8,
	D1, D2, D3, D4, D5, D6, D7, D8,
	E1, E2, E3, E4, E5, E6, E7, E8,
	F1, F2, F3, F4, F5, F6, F7, F8,
	G1, G2, G3, G4, G5, G6, G7, G8,
	H1, H2, H3, H4, H5, H6, H7, H8
};

static const uint a1h8Matrix[] =
{
	A8,                                   // 0
	A7, B8,                               // 1
	A6, B7, C8,                           // 3
	A5, B6, C7, D8,                       // 6
	A4, B5, C6, D7, E8,                   // 10
	A3, B4, C5, D6, E7, F8,               // 15
	A2, B3, C4, D5, E6, F7, G8,           // 21
	A1, B2, C3, D4, E5, F6, G7, H8,       // 28
	B1, C2, D3, E4, F5, G6, H7,           // 36
	C1, D2, E3, F4, G5, H6,               // 43
	D1, E2, F3, G4, H5,                   // 49
	E1, F2, G3, H4,                       // 54
	F1, G2, H3,                           // 58
	G1, H2,                               // 61
	H1                                    // 63
};

static const uint a8h1Matrix[] =
{
	H8,
	G8, H7,
	F8, G7, H6, 
	E8, F7, G6, H5,
	D8, E7, F6, G5, H4,
	C8, D7, E6, F5, G4, H3,
	B8, C7, D6, E5, F4, G3, H2,
	A8, B7, C6, D5, E4, F3, G2, H1,
	A7, B6, C5, D4, E3, F2, G1,
	A6, B5, C4, D3, E2, F1,
	A5, B4, C3, D2, E1,
	A4, B3, C2, D1,
	A3, B2, C1,
	A2, B1,
	A1
};

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

// Constants used for bitScanForward
const int index64[64] = 
{
   63,  0, 58,  1, 59, 47, 53,  2,
   60, 39, 48, 27, 54, 33, 42,  3,
   61, 51, 37, 40, 49, 18, 28, 20,
   55, 30, 34, 11, 43, 14, 22,  4,
   62, 57, 46, 52, 38, 26, 32, 41,
   50, 36, 17, 19, 29, 10, 13, 21,
   56, 45, 25, 31, 35, 16,  9, 12,
   44, 24, 15,  8, 23,  7,  6,  5
};
 
//#include <intrin.h>
//#pragma intrinsic(_BitScanForward64)

/**
 * bitScanForward
 * @author Martin Läuter (1997)
 *         Charles E. Leiserson
 *         Harald Prokop
 *         Keith H. Randall
 * "Using de Bruijn Sequences to Index a 1 in a Computer Word"
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
int bitScanForward(ulonglong bb)
{
  //unsigned long index = 0;
  //_BitScanForward64(&index, bb);
  //return index;
   static const ulonglong debruijn64 = ulonglong(0x07EDD5E59A4E28C2);
   assert (bb != 0);
   return index64[((bb & -bb) * debruijn64) >> 58];
}

int popCount (ulonglong x)
{
   int count = 0;
   while (x) 
   {
       count++;
       x &= x - 1; // reset LS1B
   }
   return count;
}

int bitScanForwardPopCount(ulonglong bb)
{
	assert (bb != 0);
	return popCount( (bb & -bb) - 1 );
}

BitBoard::BitBoard(void)
{
	initBoard();
	initMoves();
}

BitBoard::~BitBoard(void)
{
}

//String BitBoard::ComputeChecksum() const
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

//bool BitBoard::makeMove(Move & newMove)
//{
//	makeMove(newMove);
//	return true;
//}

//bool BitBoard::UndoMove(Move & undoMove)
//{
//	unmakeMove(undoMove);
//	//checkEndgameConditions();
//	return true;
//}

bool BitBoard::blackEnPassantAvailable() const
{ return mBlackEnPassant; }

bool BitBoard::blackKingRow() const
{ return mBlackKingRow; }

bool BitBoard::blackKingCol() const
{ return mBlackKingCol; }

bool BitBoard::canBlackCastleQueenSide() const
{ return mBlackCastleQueenSide; }

bool BitBoard::canBlackCastleKingSide() const
{ return mBlackCastleKingSide; }

bool BitBoard::canWhiteCastleQueenSide() const
{ return mWhiteCastleQueenSide; }

bool BitBoard::canWhiteCastleKingSide() const
{ return mWhiteCastleKingSide; }

uint BitBoard::enPassantCaptureCol() const
{ return mEnPassantCaptureCol; }

uint BitBoard::halfMoveClock() const
{ return mHalfMoveClock; }

bool BitBoard::isBlackChecked() const
{ return mBlackChecked; }

bool BitBoard::isBlackMated() const
{ return mBlackCheckmate; }

bool BitBoard::isBlackToMove() const
{ return !mWhiteToMove; }

bool BitBoard::isStalemate() const
{ return mStalemate; }

bool BitBoard::isWhiteChecked() const
{ return mWhiteChecked; }

bool BitBoard::isWhiteMated() const
{ return mWhiteCheckmate; }

bool BitBoard::isWhiteToMove() const
{ return mWhiteToMove; }

bool BitBoard::whiteEnPassantAvailable() const
{ return mWhiteEnPassant; }

bool BitBoard::whiteKingRow() const
{ return mWhiteKingRow; }

bool BitBoard::whiteKingCol() const
{ return mWhiteKingCol; }

uint BitBoard::totalWhiteBishops() const
{ return mTotalWhiteBishops; }

uint BitBoard::totalWhiteQueens() const
{ return mTotalWhiteQueens; }

uint BitBoard::totalWhiteRooks() const
{ return mTotalWhiteRooks; }

uint BitBoard::totalWhitePawns() const
{ return mTotalWhitePawns; }

uint BitBoard::totalWhiteKnights() const
{ return mTotalWhiteKnights; }

uint BitBoard::totalBlackBishops() const
{ return mTotalBlackBishops; }

uint BitBoard::totalBlackQueens() const
{ return mTotalBlackQueens; }

uint BitBoard::totalBlackRooks() const
{ return mTotalBlackRooks; }

uint BitBoard::totalBlackPawns() const
{ return mTotalBlackPawns; }

uint BitBoard::totalBlackKnights() const
{ return mTotalBlackKnights; }

uint BitBoard::generateLegalMoves(MoveList & moveList)
{
	MoveList candidateMoves;

	if (mWhiteToMove)
	{
    ulonglong moveBB = 0;
    ulonglong promoMask = C64(255) << 56;

		// Promotion moves
		moveBB = (mWhitePawns << 8) & promoMask & mEmptySquares;
		while (moveBB)
		{
			static const PieceType promoPieces[] = {WhiteQueen, WhiteRook, WhiteBishop, WhiteKnight};

			int destIndex = bitScanForward(moveBB); // square index from 0..63
			int sourceIndex = destIndex - 8;
			for (int i = 0; i < 4; i++)
			{
				Move newMove(sourceIndex >> 3, sourceIndex & 7, destIndex >> 3, destIndex & 7, NoPiece, promoPieces[i]);
        candidateMoves.addMove(newMove);
			}

			moveBB &= moveBB - 1;
		}

		// Generate the standard push moves
    ulonglong pawnPushes = (mWhitePawns << 8) & mEmptySquares;
		moveBB = pawnPushes & ~promoMask;
		while (moveBB)
		{
			int destIndex = bitScanForward(moveBB); // square index from 0..63
			int sourceIndex = destIndex - 8;

			Move newMove(sourceIndex >> 3, sourceIndex & 7, destIndex >> 3, destIndex & 7);
      candidateMoves.addMove(newMove);

			moveBB &= moveBB - 1;
		}

		// Generate the double push moves
    ulonglong unmovedPawns = (mWhitePawns & (255 << 8));
    ulonglong pawnDoublePushes = ( (unmovedPawns & (~(mAllPieces >> 8)) & (~(mAllPieces >> 16)) ) ) << 16;
		moveBB = pawnDoublePushes;
		while (moveBB)
		{
			int destIndex = bitScanForward(moveBB); // square index from 0..63
			int sourceIndex = destIndex - 16;

			Move newMove(sourceIndex >> 3, sourceIndex & 7, destIndex >> 3, destIndex & 7);
      newMove.setDoublePawnPush();
      candidateMoves.addMove(newMove);

			moveBB &= moveBB - 1;
		}

    ulonglong bb = mWhitePieces;
		while (bb)
		{
			int sourceIndex = bitScanForward(bb); // square index from 0..63
      uint row = sourceIndex >> 3;
      uint col = sourceIndex & 7;

      ulonglong moveBB = 0;
      //uint occupancy = 0;
			PieceType pieceType = mPieces[sourceIndex];
			switch (pieceType)
			{
			case WhitePawn:
				generateWhitePawnAttacks(sourceIndex, candidateMoves);
				break;
			case WhiteKnight:
				moveBB = mKnightAttacks[sourceIndex] & ~(mWhitePieces);
				while (moveBB)
				{
					int destIndex = bitScanForward(moveBB); // square index from 0..63
					Move newMove(row, col, destIndex >> 3, destIndex & 7, mPieces[destIndex]);
          candidateMoves.addMove(newMove);
					moveBB &= moveBB-1;
				}
				break;
			case WhiteBishop:
				{
          uint neOccupancy = (mRotate45RightPieces >> mA1H8Shifts[sourceIndex]) & mA1H8Mask[sourceIndex];
          uint nwOccupancy = (mRotate45LeftPieces >> mA8H1Shifts[sourceIndex]) & mA8H1Mask[sourceIndex];
					moveBB = (mBishopAttacksNE[sourceIndex][neOccupancy] | mBishopAttacksNW[sourceIndex][nwOccupancy]) & ~mWhitePieces;
					while (moveBB)
					{
						int destIndex = bitScanForward(moveBB); // square index from 0..63
						Move newMove(row, col, destIndex >> 3, destIndex & 7, mPieces[destIndex]);
            candidateMoves.addMove(newMove);
						moveBB &= moveBB-1;
					}
				}
				break;
			case WhiteRook:
				{
          uint rankOccupancy = (mAllPieces >> (row*8)) & 255;
          uint fileOccupancy = (mRotate90AllPieces >> (col*8)) & 255;
					moveBB = ((mRankAttacks[col][rankOccupancy] << (row*8) | mFileAttacks[row][fileOccupancy] << col) & ~mWhitePieces);
					while (moveBB)
					{
						int destIndex = bitScanForward(moveBB); // square index from 0..63
						Move newMove(row, col, destIndex >> 3, destIndex & 7, mPieces[destIndex]);
            candidateMoves.addMove(newMove);
						moveBB &= moveBB-1;
					}
				}
				break;
			case WhiteQueen:
				{
          uint neOccupancy = (mRotate45RightPieces >> mA1H8Shifts[sourceIndex]) & mA1H8Mask[sourceIndex];
          uint nwOccupancy = (mRotate45LeftPieces >> mA8H1Shifts[sourceIndex]) & mA8H1Mask[sourceIndex];
					moveBB = (mBishopAttacksNE[sourceIndex][neOccupancy] | mBishopAttacksNW[sourceIndex][nwOccupancy]) & ~mWhitePieces;

          uint rankOccupancy = (mAllPieces >> (row*8)) & 255;
          uint fileOccupancy = (mRotate90AllPieces >> (col*8)) & 255;
					moveBB |= ((mRankAttacks[col][rankOccupancy] << (row*8) | mFileAttacks[row][fileOccupancy] << col) & ~mWhitePieces);
					while (moveBB)
					{
						int destIndex = bitScanForward(moveBB); // square index from 0..63
						Move newMove(row, col, destIndex >> 3, destIndex & 7, mPieces[destIndex]);
            candidateMoves.addMove(newMove);
						moveBB &= moveBB-1;
					}
				}
				break;
			case WhiteKing:
				{
					generateKingMoves(sourceIndex, mWhitePieces, mWhiteCastleKingSide, mWhiteCastleQueenSide, candidateMoves);
					//moveBB = mKingAttacks[sourceIndex] & ~(mWhitePieces);
				}
				break;
      default:
        break;
			}
			bb &= bb-1;
		}
	}
	else
	{
    ulonglong bb = mBlackPieces;
		while (bb)
		{
			int sourceIndex = bitScanForward(bb); // square index from 0..63
      uint row = sourceIndex >> 3;
      uint col = sourceIndex & 7;

      ulonglong moveBB = 0;
      uint occupancy = 0;
			PieceType pieceType = mPieces[sourceIndex];
			switch (pieceType)
			{
			case BlackPawn:
				generateBlackPawnMoves(sourceIndex, candidateMoves);
				generateBlackPawnAttacks(sourceIndex, candidateMoves);
				break;
			case BlackKnight:
				moveBB = mKnightAttacks[sourceIndex] & ~(mBlackPieces);
				while (moveBB)
				{
					int destIndex = bitScanForward(moveBB); // square index from 0..63
					Move newMove(row, col, destIndex >> 3, destIndex & 7, mPieces[destIndex]);
          candidateMoves.addMove(newMove);
					moveBB &= moveBB-1;
				}
				break;
			case BlackBishop:
				{
					occupancy = (mRotate45RightPieces >> mA1H8Shifts[sourceIndex]) & mA1H8Mask[sourceIndex];
					moveBB |= mBishopAttacksNE[sourceIndex][occupancy] & ~mBlackPieces;
					occupancy = (mRotate45LeftPieces >> mA8H1Shifts[sourceIndex]) & mA8H1Mask[sourceIndex];
					moveBB |= mBishopAttacksNW[sourceIndex][occupancy] & ~mBlackPieces;
					while (moveBB)
					{
						int destIndex = bitScanForward(moveBB); // square index from 0..63
						Move newMove(row, col, destIndex >> 3, destIndex & 7, mPieces[destIndex]);
            candidateMoves.addMove(newMove);
						moveBB &= moveBB-1;
					}
				}
				break;
			case BlackRook:
				{
          uint rankOccupancy = (mAllPieces >> (row*8)) & 255;
          ulonglong rankAttacks = (mRankAttacks[col][rankOccupancy] << (row*8));
          uint fileOccupancy = (mRotate90AllPieces >> (col*8)) & 255;
          ulonglong fileAttacks = (mFileAttacks[row][fileOccupancy] << col);
					moveBB = ((rankAttacks | fileAttacks) & ~mBlackPieces);
					while (moveBB)
					{
						int destIndex = bitScanForward(moveBB); // square index from 0..63
						Move newMove(row, col, destIndex >> 3, destIndex & 7, mPieces[destIndex]);
            candidateMoves.addMove(newMove);
						moveBB &= moveBB-1;
					}
				}
				break;
			case BlackQueen:
				{
					occupancy = (mRotate45RightPieces >> mA1H8Shifts[sourceIndex]) & mA1H8Mask[sourceIndex];
					moveBB |= mBishopAttacksNE[sourceIndex][occupancy] & ~mBlackPieces;
					occupancy = (mRotate45LeftPieces >> mA8H1Shifts[sourceIndex]) & mA8H1Mask[sourceIndex];
					moveBB |= mBishopAttacksNW[sourceIndex][occupancy] & ~mBlackPieces;
          uint rankOccupancy = (mAllPieces >> (row*8)) & 255;
          ulonglong rankAttacks = (mRankAttacks[col][rankOccupancy] << (row*8));
          uint fileOccupancy = (mRotate90AllPieces >> (col*8)) & 255;
          ulonglong fileAttacks = (mFileAttacks[row][fileOccupancy] << col);
					moveBB |= ((rankAttacks | fileAttacks) & ~mBlackPieces);
					while (moveBB)
					{
						int destIndex = bitScanForward(moveBB); // square index from 0..63
						Move newMove(row, col, destIndex >> 3, destIndex & 7, mPieces[destIndex]);
            candidateMoves.addMove(newMove);
						moveBB &= moveBB-1;
					}
				}
				break;
			case BlackKing:
				{
					generateKingMoves(sourceIndex, mBlackPieces, mBlackCastleKingSide, mBlackCastleQueenSide, candidateMoves);
				}
				break;
      default:
        break;
			}
			bb &= bb-1;
		}
	}

	// Generate the list of candidate moves
	//MoveList candidateMoves;
	//generateCandidateMoves(candidateMoves);

	// Trim the candidate moves based on the legality of the candidate moves
	trimCandidateMoves(candidateMoves, moveList);
	
  return moveList.totalMoves();
}

uint BitBoard::generateLegalMoves(uint row, uint col, MoveList & moveList)
{
	// Check to make sure we have a piece at this square
  uint index = getIndex(row, col);
	PieceType pieceType = mPieces[index];
	if (pieceType == NoPiece)
		return 0;

	MoveList candidateMoves;

	// Generate the list of candidate moves
	switch (pieceType)
	{
	case WhitePawn:
		generateWhitePawnMoves(index, candidateMoves);
		generateWhitePawnAttacks(index, candidateMoves);
		break;
	case WhiteRook:
		generateRookMoves(index, row, col, mWhitePieces, candidateMoves);
		break;
	case WhiteBishop:
		generateBishopMoves(index, mWhitePieces, candidateMoves);
		break;
	case WhiteQueen:
		generateRookMoves(index, row, col, mWhitePieces, candidateMoves);
		generateBishopMoves(index, mWhitePieces, candidateMoves);
		break;
	case WhiteKnight:
		generateKnightMoves(index, mWhitePieces, candidateMoves);
		break;
	case WhiteKing:
		generateKingMoves(index, mWhitePieces, mWhiteCastleKingSide, mWhiteCastleQueenSide, candidateMoves);
		break;
	case BlackPawn:
		generateBlackPawnMoves(index, candidateMoves);
		generateBlackPawnAttacks(index, candidateMoves);
		break;
	case BlackRook:
		generateRookMoves(index, row, col, mBlackPieces, candidateMoves);
		break;
	case BlackBishop:
		generateBishopMoves(index, mBlackPieces, candidateMoves);
		break;
	case BlackKnight:
		generateKnightMoves(index, mBlackPieces, candidateMoves);
		break;
	case BlackQueen:
		generateRookMoves(index, row, col, mBlackPieces, candidateMoves);
		generateBishopMoves(index, mBlackPieces, candidateMoves);
		break;
	case BlackKing:
		generateKingMoves(index, mBlackPieces, mBlackCastleKingSide, mBlackCastleQueenSide, candidateMoves);
		break;
  default:
    break;
	}

	// Trim the candidate moves based on the legality of the candidate moves
	trimCandidateMoves(candidateMoves, moveList);

  return moveList.totalMoves();
}

void BitBoard::generateBlackPawnMoves(uint index, MoveList & moveList)
{
	// Add single push moves
  ulonglong pawnPushes = (mBlackPawns >> 8) & mEmptySquares;
	pawnPushes &= (C64(1) << (index-8));

	// Add double push moves
  ulonglong unmovedPawns = (mBlackPawns & (C64(255) << 48));
  ulonglong pawnDoublePushes = ( (unmovedPawns & (~(mAllPieces << 8)) & (~(mAllPieces << 16)) ) ) >> 16;
	pawnDoublePushes &= (C64(1) << (index-16));

	// Add the pawn moves
  ulonglong moveBB = pawnDoublePushes | pawnPushes;
	if (moveBB)
	{
		//StopwatchHolder holder(mSerializeMoves);
		do 
		{
			int destIndex = bitScanForward(moveBB); // square index from 0..63

      uint destRow = destIndex >> 3;
      uint destCol = destIndex & 7;
      uint sourceRow = destRow+1;

			if (destRow == 0)
			{
				Move queenPromo(sourceRow, destCol, destRow, destCol, NoPiece, BlackQueen);
				Move bishopPromo(sourceRow, destCol, destRow, destCol, NoPiece, BlackBishop);
				Move rookPromo(sourceRow, destCol, destRow, destCol, NoPiece, BlackRook);
				Move knightPromo(sourceRow, destCol, destRow, destCol, NoPiece, BlackKnight);
				
        moveList.addMove(queenPromo);
        moveList.addMove(bishopPromo);
        moveList.addMove(rookPromo);
        moveList.addMove(knightPromo);
			}
			else
			{
				bool doublePush = false;
        uint sourceIndex = sourceRow*8 + destCol;

				if (!(mBlackPawns & (C64(1) << sourceIndex)))
				{
					doublePush = true;
					sourceRow++;
				}

				Move newMove(sourceRow, destCol, destRow, destCol);
				if (doublePush)
          newMove.setDoublePawnPush();

        moveList.addMove(newMove);
			}

		} while (moveBB &= moveBB-1); // reset LS1B
	}
}

void BitBoard::generateWhitePawnMoves(uint index, MoveList & moveList)
{
	// Generate the standard push moves
  ulonglong pawnPushes = (mWhitePawns << 8) & mEmptySquares;
	pawnPushes &= (C64(1) << (index+8));
	//writeBitBoard((C64(1) << (index+8)));

	// Generate the double push moves
  ulonglong unmovedPawns = (mWhitePawns & (255 << 8));
  ulonglong pawnDoublePushes = ( (unmovedPawns & (~(mAllPieces >> 8)) & (~(mAllPieces >> 16)) ) ) << 16;
	pawnDoublePushes &= C64(1) << (index+16);

	// Serialize the bit board moves
  ulonglong moveBB = pawnPushes | pawnDoublePushes;
	if (moveBB)
	{
		//StopwatchHolder holder(mSerializeMoves);

		do {
			int destIndex = bitScanForward(moveBB); // square index from 0..63

      uint destRow = destIndex >> 3;
      uint destCol = destIndex & 7;
      uint sourceRow = destRow - 1;

			if (destRow == 7)
			{
				Move queenPromo(sourceRow, destCol, destRow, destCol, NoPiece, WhiteQueen);
				Move bishopPromo(sourceRow, destCol, destRow, destCol, NoPiece, WhiteBishop);
				Move rookPromo(sourceRow, destCol, destRow, destCol, NoPiece, WhiteBishop);
				Move knightPromo(sourceRow, destCol, destRow, destCol, NoPiece, WhiteKnight);
	
        moveList.addMove(queenPromo);
        moveList.addMove(bishopPromo);
        moveList.addMove(rookPromo);
        moveList.addMove(knightPromo);
			}
			else
			{
				bool doublePush = false;
        uint sourceIndex = sourceRow*8 + destCol;

				if (!(mWhitePawns & (C64(1) << sourceIndex)))
				{
					doublePush = true;
					sourceRow--;
				}

				Move newMove(sourceRow, destCol, destRow, destCol);
				if (doublePush)
          newMove.setDoublePawnPush();

        moveList.addMove(newMove);
			}

		} while (moveBB &= moveBB-1); // reset LS1B
	}
}

PieceType BitBoard::getPieceType(uint row, uint col) const
{ 
	return mPieces[getIndex(row, col)];
}

bool BitBoard::hasPiece(uint row, uint col) const
{
	return mPieces[getIndex(row,col)] != NoPiece;
}

bool BitBoard::isCellAttacked(uint row, uint col, bool whitePiece) const
{
	//StopwatchHolder holder(mCellAttackTimer);

  uint index = row*8 + col; // getIndex(row, col);

	// Check pawn attacks
  ulonglong pawnBB = (whitePiece) ? mBlackPawns : mWhitePawns;
  ulonglong pawnAttacks = (whitePiece) ? mPawnAttacks[0][index] : mPawnAttacks[1][index];
	if (pawnAttacks & pawnBB) 
		return true;

	// Check knight attacks
  ulonglong knightBB = (whitePiece) ? mBlackKnights : mWhiteKnights;
	if (mKnightAttacks[index] & knightBB) 
		return true; 

	// Check king attacks
  ulonglong kingBB = (whitePiece) ? mBlackKings : mWhiteKings;
	if (mKingAttacks[index] & kingBB) 
		return true;

	// Check bishop attacks
  ulonglong bishopQueenBB = (whitePiece) ? (mBlackQueens | mBlackBishops) : (mWhiteQueens | mWhiteBishops);
  uint occupancyNE = (mRotate45RightPieces >> mA1H8Shifts[index]) & mA1H8Mask[index];
  uint occupancyNW = (mRotate45LeftPieces >> mA8H1Shifts[index]) & mA8H1Mask[index];
  ulonglong bishopAttacks = mBishopAttacksNE[index][occupancyNE] | mBishopAttacksNW[index][occupancyNW];
	if (bishopAttacks & bishopQueenBB)
		return true;

	// Check rook attacks
  ulonglong rookQueenBB = (whitePiece) ? (mBlackQueens | mBlackRooks) : (mWhiteQueens | mWhiteRooks);
  uint rankOccupancy = (mAllPieces >> (row*8)) & 255;
  uint fileOccupancy = (mRotate90AllPieces >> (col*8)) & 255;
  ulonglong rookAttacks = (mRankAttacks[col][rankOccupancy] << (row*8)) | (mFileAttacks[row][fileOccupancy] << col);
	if (rookAttacks & rookQueenBB)
		return true;

	return false;
}

ulonglong BitBoard::rotate90(ulonglong bb)
{
  ulonglong symmBB = 0;
  for (uint index = 0; index < 64; index++)
		if (bb & (C64(1) << index))
			symmBB |= (C64(1) << rot90Matrix[index]);

	return symmBB;
}

ulonglong BitBoard::rotateNeg45(ulonglong bb, const uint * rotMatrix)
{	
  ulonglong symmBB = 0;
  for (uint index = 0; index < 64; index++)
		if (bb & (C64(1) << index))
			symmBB |= (C64(1) << rotMatrix[index]);

	return symmBB;
}

ulonglong BitBoard::rotate45(ulonglong bb, const uint * rotMatrix)
{	
  ulonglong symmBB = 0;
  for (uint index = 0; index < 64; index++)
		if (bb & (C64(1) << rotMatrix[index]))
			symmBB |= (C64(1) << index);

	return symmBB;
}

void BitBoard::setPosition(const std::string & fenString)
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
      ulonglong bb = C64(1) << index;
      switch (mPieces[index]) {
      case BlackPawn:
        mBlackPawns |= bb;
        mTotalBlackPawns++;
        break;
      case BlackRook:
        mBlackRooks |= bb;
        mTotalBlackRooks++;
        break;
      case BlackKnight:
        mBlackKnights |= bb;
        mTotalBlackKnights++;
        break;
      case BlackBishop:
        mBlackBishops |= bb;
        mTotalBlackBishops++;
        break;
      case BlackQueen:
        mBlackQueens |= bb;
        mTotalBlackQueens++;
        break;
      case BlackKing:
        mBlackKings |= bb;
        mBlackKingRow = row;
        mBlackKingCol = col;
        break;
      case WhitePawn:
        mWhitePawns |= bb;
        mTotalWhitePawns++;
        break;
      case WhiteRook:
        mWhiteRooks |= bb;
        mTotalWhiteRooks++;
        break;
      case WhiteKnight:
        mWhiteKnights |= bb;
        mTotalWhiteKnights++;
        break;
      case WhiteBishop:
        mWhiteBishops |= bb;
        mTotalWhiteBishops++;
        break;
      case WhiteQueen:
        mWhiteQueens |= bb;
        mTotalWhiteQueens++;
        break;
      case WhiteKing:
        mWhiteKings |= bb;
        mWhiteKingRow = row;
        mWhiteKingCol = col;
        break;
      default:
        break;
      }
    }
  }

	// Update bit boards
	mWhitePieces = mWhiteRooks | mWhiteKnights | mWhiteBishops | mWhiteQueens | mWhiteKings | mWhitePawns;
	mBlackPieces = mBlackRooks | mBlackKnights | mBlackBishops | mBlackQueens | mBlackKings | mBlackPawns;
	mAllPieces = mWhitePieces | mBlackPieces;
	mEmptySquares = ~mAllPieces;
	mRotate90AllPieces = rotate90(mAllPieces);
	mRotate45RightPieces = rotate45(mAllPieces, a1h8Matrix);
	mRotate45LeftPieces = rotate45(mAllPieces, a8h1Matrix);

  //	std::map<char, PieceType> pieceTypes;

  //	pieceTypes['p'] = BlackPawn;
  //	pieceTypes['r'] = BlackRook;
  //	pieceTypes['n'] = BlackKnight;
  //	pieceTypes['b'] = BlackBishop;
  //	pieceTypes['q'] = BlackQueen;
  //	pieceTypes['k'] = BlackKing;
  //	pieceTypes['P'] = WhitePawn;
  //	pieceTypes['R'] = WhiteRook;
  //	pieceTypes['N'] = WhiteKnight;
  //	pieceTypes['B'] = WhiteBishop;
  //	pieceTypes['Q'] = WhiteQueen;
  //	pieceTypes['K'] = WhiteKing;

  //	mWhitePawns = mWhiteRooks = mWhiteKnights = mWhiteBishops = mWhiteQueens = mWhiteKings = 0;
  //	mBlackPawns = mBlackRooks = mBlackKnights = mBlackBishops = mBlackQueens = mBlackKings = 0;
  //	mTotalWhitePawns = mTotalWhiteKnights = mTotalWhiteRooks = mTotalWhiteBishops = mTotalWhiteQueens = 0;
  //	mTotalBlackPawns = mTotalBlackKnights = mTotalBlackRooks = mTotalBlackBishops = mTotalBlackQueens = 0;
  //  uint row = 7, col = 0;
  //	int currentIndex = -1;
  //	while (++currentIndex < fenString.length())
  //	{
  //    uint index = getIndex(row, col);
  //    ulonglong bb = C64(1) << index;

  //		char c = fenString.at(currentIndex);
  //		if (c == ' ')
  //			break;

  //		switch (c)
  //		{
  //		case 'p':
  //			mBlackPawns |= bb;
  //			mPieces[index] = pieceTypes[c];
  //			col++;
  //			mTotalBlackPawns++;
  //			break;
  //		case 'r':
  //			mBlackRooks |= bb;
  //			mPieces[index] = pieceTypes[c];
  //			col++;
  //			mTotalBlackRooks++;
  //			break;
  //		case 'n':
  //			mBlackKnights |= bb;
  //			mPieces[index] = pieceTypes[c];
  //			col++;
  //			mTotalBlackKnights++;
  //			break;
  //		case 'b':
  //			mBlackBishops |= bb;
  //			mPieces[index] = pieceTypes[c];
  //			col++;
  //			break;
  //		case 'q':
  //			mBlackQueens |= bb;
  //			mPieces[index] = pieceTypes[c];
  //			col++;
  //			mTotalBlackQueens++;
  //			break;
  //		case 'k':
  //			mPieces[index] = pieceTypes[c];
  //			mBlackKings |= bb;
  //			mBlackKingRow = row;
  //			mBlackKingCol = col;
  //			col++;
  //			break;
  //		case 'P':
  //			mWhitePawns |= bb;
  //			mPieces[index] = pieceTypes[c];
  //			col++;
  //			mTotalWhitePawns++;
  //			break;
  //		case 'R':
  //			mWhiteRooks |= bb;
  //			mPieces[index] = pieceTypes[c];
  //			col++;
  //			mTotalWhiteRooks++;
  //			break;
  //		case 'N':
  //			mWhiteKnights |= bb;
  //			mPieces[index] = pieceTypes[c];
  //			col++;
  //			mTotalWhiteKnights++;
  //			break;
  //		case 'B':
  //			mWhiteBishops |= bb;
  //			mPieces[index] = pieceTypes[c];
  //			col++;
  //			mTotalWhiteBishops++;
  //			break;
  //		case 'Q':
  //			mWhiteQueens |= bb;
  //			mPieces[index] = pieceTypes[c];
  //			col++;
  //			mTotalWhiteQueens++;
  //			break;
  //		case 'K':
  //			mWhiteKings |= bb;
  //			mPieces[index] = pieceTypes[c];
  //			mWhiteKingRow = row;
  //			mWhiteKingCol = col;
  //			col++;
  //			break;
  //		case '1':
  //		case '2':
  //		case '3':
  //		case '4':
  //		case '5':
  //		case '6':
  //		case '7':
  //		case '8':
  //		{
  //			int number = c - '0';
  //			for (int i = 0; i < number; i++)
  //				mPieces[index++] = NoPiece;
  //			col += number;
  //			break;
  //		}
  //		case '/':
  //			row--;
  //			col = 0;
  //			break;
  //		}
  //	}

//  std::string stateString(fenString.substr(++currentIndex));

//	std::string moveString, castleString, enPassantString, halfMoveString, fullMoveString;
//	std::istringstream iss(stateString);
//	iss >> moveString >> castleString >> enPassantString >> halfMoveString >> fullMoveString;

//	mWhiteToMove = false;
//	if (moveString == "w")
//		mWhiteToMove = true;

//	mWhiteCastleKingSide = mWhiteCastleQueenSide = mBlackCastleKingSide = mBlackCastleQueenSide = false;
//	int castleIndex = -1;
//	while (++castleIndex < castleString.size())
//	{
//		char c = castleString.at(castleIndex);

//		if (c == 'K') mWhiteCastleKingSide = true;
//		if (c == 'Q') mWhiteCastleQueenSide = true;
//		if (c == 'k') mBlackCastleKingSide = true;
//		if (c == 'q') mBlackCastleQueenSide = true;
//	}

//	// TODO: Read the enpassant string
//	mWhiteEnPassant = mBlackEnPassant = false;
//	mEnPassantCaptureCol = 8;

//	// Read the half move clock
//	mHalfMoveClock = atoi(halfMoveString.c_str());
}

void BitBoard::generateCandidateMoves(MoveList & moveList)
{
	//StopwatchHolder holder(mCandidateMoveTimer);

	if (mWhiteToMove)
	{
		generateWhitePawnMoves(moveList);

    ulonglong bitBoard = mWhitePieces;
		do {
			int index = bitScanForward(bitBoard); // square index from 0..63
			PieceType pieceType = mPieces[index];
      uint row = index >> 3;
      uint col = index & 7;

			switch (pieceType)
			{
			case WhitePawn:
				generateWhitePawnAttacks(index, moveList);
				break;
			case WhiteKnight:
				generateKnightMoves(index, mWhitePieces, moveList);
				break;
			case WhiteBishop:
				generateBishopMoves(index, mWhitePieces, moveList);
				break;
			case WhiteRook:
				generateRookMoves(index, row, col, mWhitePieces, moveList);
				break;
			case WhiteQueen:
				generateRookMoves(index, row, col, mWhitePieces, moveList);
				generateBishopMoves(index, mWhitePieces,moveList);
				break;
			case WhiteKing:
				generateKingMoves(index, mWhitePieces, mWhiteCastleKingSide, mWhiteCastleQueenSide, moveList);
				break;
      default:
        break;
			}
		} while (bitBoard &= bitBoard-1); // reset LS1B
	}
	else
	{
		generateBlackPawnMoves(moveList);

    ulonglong bitBoard = mBlackPieces;
		do {
			int index = bitScanForward(bitBoard); // square index from 0..63
			PieceType pieceType = mPieces[index];
      uint row = index >> 3;
      uint col = index & 7;

			switch (pieceType)
			{
			case BlackPawn:
				generateBlackPawnAttacks(index, moveList);
				break;
			case BlackKnight:
				generateKnightMoves(index, mBlackPieces, moveList);
				break;
			case BlackBishop:
				generateBishopMoves(index, mBlackPieces, moveList);
				break;
				generateRookMoves(index, row, col, mWhitePieces, moveList);
				break;
			case BlackRook:
				generateRookMoves(index, row, col, mBlackPieces, moveList);
				break;
			case BlackQueen:
				generateRookMoves(index, row, col, mBlackPieces, moveList);
				generateBishopMoves(index, mBlackPieces, moveList);
				break;
			case BlackKing:
				generateKingMoves(index, mBlackPieces, mBlackCastleKingSide, mBlackCastleQueenSide, moveList);
				break;
      default:
        break;
			}
		} while (bitBoard &= bitBoard-1); // reset LS1B
	}
}

void BitBoard::generateWhitePawnMoves(MoveList & moveList)
{
	// Generate the standard push moves
  ulonglong pawnPushes = (mWhitePawns << 8) & mEmptySquares;

	// Generate the double push moves
  ulonglong unmovedPawns = (mWhitePawns & (255 << 8));
  ulonglong pawnDoublePushes = ( (unmovedPawns & (~(mAllPieces >> 8)) & (~(mAllPieces >> 16)) ) ) << 16;

	// Serialize the bit board moves
  ulonglong moveBB = pawnPushes | pawnDoublePushes;
	while (moveBB)
	{
		int destIndex = bitScanForward(moveBB); // square index from 0..63

    uint destRow = destIndex >> 3;
    uint destCol = destIndex & 7;
    uint sourceRow = destRow - 1;

		if (destRow == 7)
		{
			Move queenPromo(sourceRow, destCol, destRow, destCol, NoPiece, WhiteQueen);
			Move bishopPromo(sourceRow, destCol, destRow, destCol, NoPiece, WhiteBishop);
			Move rookPromo(sourceRow, destCol, destRow, destCol, NoPiece, WhiteRook);
			Move knightPromo(sourceRow, destCol, destRow, destCol, NoPiece, WhiteKnight);
		
      moveList.addMove(queenPromo);
      moveList.addMove(bishopPromo);
      moveList.addMove(rookPromo);
      moveList.addMove(knightPromo);
		}
		else
		{
			bool doublePush = false;
      uint sourceIndex = sourceRow*8 + destCol;

			if (!(mWhitePawns & (C64(1) << sourceIndex)))
			{
				doublePush = true;
				sourceRow--;
			}

			Move newMove(sourceRow, destCol, destRow, destCol);
			if (doublePush)
        newMove.setDoublePawnPush();

      moveList.addMove(newMove);
		}
		moveBB &= moveBB-1; // reset LS1B
	}
}

void BitBoard::generateCandidateMoves(uint index, ulonglong pieceBB, MoveList & moveList)
{
  uint row = index >> 3;
  uint col = index & 7;

	PieceType pieceType = mPieces[index];
	switch (pieceType)
	{
	case WhitePawn:
		generateWhitePawnMoves(index, moveList);
		break;
	case BlackPawn:
		generateBlackPawnMoves(index, moveList);
		break;
	case WhiteKnight:
		generateKnightMoves(index, mWhitePieces, moveList);
		break;
	case BlackKnight:
		generateKnightMoves(index, mBlackPieces, moveList);
		break;
	case WhiteBishop:
		generateBishopMoves(index, mWhitePieces, moveList);
		break;
	case BlackBishop:
		generateBishopMoves(index, mBlackPieces, moveList);
		break;
	case WhiteRook:
		generateRookMoves(index, row, col, mWhitePieces, moveList);
		break;
	case BlackRook:
		generateRookMoves(index, row, col, mBlackPieces, moveList);
		break;
	case WhiteQueen:
		generateRookMoves(index, row, col, mWhitePieces, moveList);
		generateBishopMoves(index, mWhitePieces, moveList);
		break;
	case BlackQueen:
		generateRookMoves(index, row, col, mBlackPieces, moveList);
		generateBishopMoves(index, mBlackPieces, moveList);
		break;
	case WhiteKing:
		generateKingMoves(index, mWhitePieces, mWhiteCastleKingSide, mWhiteCastleQueenSide, moveList);
		break;
	case BlackKing:
		generateKingMoves(index, mBlackPieces, mBlackCastleKingSide, mBlackCastleQueenSide, moveList);
		break;
  default:
    break;
	}
}

void BitBoard::generateBishopMoves(uint index, ulonglong pieceBB, MoveList & moveList)
{
	//StopwatchHolder holder(mBishopMoveTime);

  ulonglong moveBB = 0;
  uint occupancy = 0;

	occupancy = (mRotate45RightPieces >> mA1H8Shifts[index]) & mA1H8Mask[index];
	moveBB |= mBishopAttacksNE[index][occupancy] & ~pieceBB;

	occupancy = (mRotate45LeftPieces >> mA8H1Shifts[index]) & mA8H1Mask[index];
	moveBB |= mBishopAttacksNW[index][occupancy] & ~pieceBB;

	if (moveBB)
		serializeBitBoardMoves(moveBB, index, moveList);
}

void BitBoard::generateBlackPawnMoves(MoveList & moveList)
{
	//StopwatchHolder holder(mBlackPawnTime);

	// Add single push moves
  ulonglong pawnPushes = (mBlackPawns >> 8) & mEmptySquares;

	// Add double push moves
  ulonglong unmovedPawns = (mBlackPawns & (C64(255) << 48));
  ulonglong pawnDoublePushes = ( (unmovedPawns & (~(mAllPieces << 8)) & (~(mAllPieces << 16)) ) ) >> 16;

	// Add the pawn moves
  ulonglong moveBB = pawnDoublePushes | pawnPushes;
	if (moveBB)
	{
		//StopwatchHolder holder(mSerializeMoves);

		do {
			int destIndex = bitScanForward(moveBB); // square index from 0..63

      uint destRow = destIndex >> 3;
      uint destCol = destIndex & 7;
      uint sourceRow = destRow+1;

			if (destRow == 0)
			{
				Move queenPromo(sourceRow, destCol, destRow, destCol, NoPiece, BlackQueen);
				Move bishopPromo(sourceRow, destCol, destRow, destCol, NoPiece, BlackBishop);
				Move rookPromo(sourceRow, destCol, destRow, destCol, NoPiece, BlackRook);
				Move knightPromo(sourceRow, destCol, destRow, destCol, NoPiece, BlackKnight);

        moveList.addMove(queenPromo);
        moveList.addMove(bishopPromo);
        moveList.addMove(rookPromo);
        moveList.addMove(knightPromo);
			}
			else
			{
				bool doublePush = false;
        uint sourceIndex = sourceRow*8 + destCol;

				if (!(mBlackPawns & (C64(1) << sourceIndex)))
				{
					doublePush = true;
					sourceRow++;
				}

				Move newMove(sourceRow, destCol, destRow, destCol);
				if (doublePush)
          newMove.setDoublePawnPush();

        moveList.addMove(newMove);
			}

		} while (moveBB &= moveBB-1); // reset LS1B
	}
}

void BitBoard::generateBlackPawnAttacks(uint index, MoveList & moveList)
{
	//StopwatchHolder holder(mBlackPawnTime);

  uint sourceRow = index >> 3;
  uint sourceCol = index & 7;

	// Generate the attack moves
  ulonglong moveBB = mPawnAttacks[1][index] & mWhitePieces;
	if (moveBB)
	{
		//StopwatchHolder holder(mSerializeMoves);
		do 
		{
			int destIndex = bitScanForward(moveBB); // square index from 0..63

      uint destRow = destIndex >> 3;
      uint destCol = destIndex & 7;
			PieceType pieceType = mPieces[destIndex];

			if (destRow == 0)
			{
				Move queenPromo(sourceRow, sourceCol, destRow, destCol, pieceType, BlackQueen);
				Move bishopPromo(sourceRow, sourceCol, destRow, destCol, pieceType, BlackBishop);
				Move rookPromo(sourceRow, sourceCol, destRow, destCol, pieceType, BlackRook);
				Move knightPromo(sourceRow, sourceCol, destRow, destCol, pieceType, BlackKnight);

        moveList.addMove(queenPromo);
        moveList.addMove(bishopPromo);
        moveList.addMove(rookPromo);
        moveList.addMove(knightPromo);
			}
			else
			{
				Move newMove(sourceRow, sourceCol, destRow, destCol, pieceType);
        moveList.addMove(newMove);
			}

		} while (moveBB &= moveBB-1); // reset LS1B
	}

	if (mBlackEnPassant && sourceRow == 3)
	{
    uint epIndex = 24 + mEnPassantCaptureCol;

    bool addMove = false;
		if (mEnPassantCaptureCol == 0)
		{
			if (sourceCol == mEnPassantCaptureCol+1)
        addMove = true;
		}
		else if (mEnPassantCaptureCol == 7)
		{
			if (sourceCol == mEnPassantCaptureCol-1)
        addMove = true;
		}
		else
		{
			if (sourceCol == mEnPassantCaptureCol+1 || sourceCol == mEnPassantCaptureCol-1)
        addMove = true;
		}

    if (addMove)
		{
			Move newMove(sourceRow, sourceCol, sourceRow-1, mEnPassantCaptureCol, WhitePawn);
      newMove.setEnPassant();
      moveList.addMove(newMove);
		}
	}
}

void BitBoard::generateKingMoves(uint index, ulonglong pieceBB, bool kingSide, bool queenSide, MoveList & moveList)
{
	//StopwatchHolder holder(mKingMoveTime);

  ulonglong moveBB = mKingAttacks[index] & ~(pieceBB);
	if (moveBB)
		serializeBitBoardMoves(moveBB, index, moveList);

	if (!kingSide && !queenSide)
		return;

	PieceType rookType = (mWhiteToMove) ? WhiteRook : BlackRook;
  uint kingRow = index >> 3;
	if (kingSide)
	{
    uint kingCol = index & 7;
    uint index1 = getIndex(kingRow, 5);
    uint index2 = getIndex(kingRow, 6);
    ulonglong kingSideBB = (C64(1) << index1) | (C64(1) << index2);
		if (mPieces[index1] == NoPiece && mPieces[index2] == NoPiece && mPieces[index2+1] == rookType)
		{
      uint col = kingCol;
			bool cellAttacked = false;
			while (col <= 6)
			{
        if (isCellAttacked(kingRow, col, mWhiteToMove))
				{
					cellAttacked = true;
					break;
				}
				col++;
			}

			if (!cellAttacked)
			{
				Move newMove(kingRow, 4, kingRow, 6);
        newMove.setKingCastle();
        moveList.addMove(newMove);
			}
		}
	}
				
	if (queenSide)
	{
    uint kingCol = index & 7;
    uint index0 = getIndex(kingRow, 0);
    uint index1 = getIndex(kingRow, 1);
    uint index2 = getIndex(kingRow, 2);
    uint index3 = getIndex(kingRow, 3);
    ulonglong queenSideBB = (C64(1) << index1) | (C64(1) << index2) | (C64(1) << index3);
		if (mPieces[index1] == NoPiece && mPieces[index2] == NoPiece && mPieces[index3] == NoPiece && mPieces[index0] == rookType)
		{
      uint col = kingCol;
			bool cellAttacked = false;
			while (col >= 2)
			{
        if (isCellAttacked(kingRow, col, mWhiteToMove))
				{
					cellAttacked = true;
					break;
				}
				col--;
			}

			if (!cellAttacked)
			{
				Move newMove(kingRow, 4, kingRow, 2);
        newMove.setQueenCastle();
        moveList.addMove(newMove);
			}
		}
	}
}

void BitBoard::generateKnightMoves(uint index, ulonglong pieceBB, MoveList & moveList)
{
	//StopwatchHolder holder(mKnightMoveTime);

  ulonglong moveBB = mKnightAttacks[index] & ~(pieceBB);
	if (moveBB)
		serializeBitBoardMoves(moveBB, index, moveList);
}

void BitBoard::generateRookMoves(uint index, uint row, uint col, ulonglong pieceBB, MoveList & moveList)
{
	//StopwatchHolder holder(mRookMoveTime);

	// Generate the rank moves
  uint rankOccupancy = (mAllPieces >> (row*8)) & 255;
  ulonglong rankAttacks = (mRankAttacks[col][rankOccupancy] << (row*8));

	// Generate the file moves
  uint fileOccupancy = (mRotate90AllPieces >> (col*8)) & 255;
  ulonglong fileAttacks = (mFileAttacks[row][fileOccupancy] << col);

  ulonglong moveBB = ((rankAttacks | fileAttacks) & ~pieceBB);
	if (moveBB)
		serializeBitBoardMoves(moveBB, index, moveList);
}

void BitBoard::generateWhitePawnAttacks(uint index, MoveList & moveList)
{
	//StopwatchHolder holder(mWhitePawnTime);

  uint sourceRow = index >> 3;
  uint sourceCol = index & 7;

	// Generate the attack moves
  ulonglong moveBB = mPawnAttacks[0][index] & mBlackPieces;
	if (moveBB)
	{
		//StopwatchHolder holder(mSerializeMoves);
		do 
		{
			int destIndex = bitScanForward(moveBB); // square index from 0..63
      uint destRow = destIndex >> 3;
      uint destCol = destIndex & 7;
			PieceType capturePiece = mPieces[destIndex];

			if (destRow == 7)
			{
				Move queenPromo(sourceRow, sourceCol, destRow, destCol, capturePiece, WhiteQueen);
				Move bishopPromo(sourceRow, sourceCol, destRow, destCol, capturePiece, WhiteBishop);
				Move rookPromo(sourceRow, sourceCol, destRow, destCol, capturePiece, WhiteRook);
				Move knightPromo(sourceRow, sourceCol, destRow, destCol, capturePiece, WhiteKnight);

        moveList.addMove(queenPromo);
        moveList.addMove(bishopPromo);
        moveList.addMove(rookPromo);
        moveList.addMove(knightPromo);
			}
			else
			{
				Move newMove(sourceRow, sourceCol, destRow, destCol, capturePiece);
        moveList.addMove(newMove);
			}

		} while (moveBB &= moveBB-1); // reset LS1B
	}

	// Add any en passant moves
	if (mWhiteEnPassant && sourceRow == 4)
	{
    bool addMove = false;
		if (mEnPassantCaptureCol == 0)
		{
			if (sourceCol == mEnPassantCaptureCol+1)
        addMove = true;
		}
		else if (mEnPassantCaptureCol == 7)
		{
			if (sourceCol == mEnPassantCaptureCol-1)
        addMove = true;
		}
		else
		{
			if (sourceCol == mEnPassantCaptureCol+1 || sourceCol == mEnPassantCaptureCol-1)
        addMove = true;
		}

    if (addMove)
		{
			Move newMove(sourceRow, sourceCol, sourceRow+1, mEnPassantCaptureCol, BlackPawn);
      newMove.setEnPassant();
      moveList.addMove(newMove);
		}
	}
}

void BitBoard::getCastlingRights(uint & castlingRights) const
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

uint BitBoard::getIndex(uint row, uint col) const
{
	return (row << 3) + col;
}

void BitBoard::getRowCol(uint index, uint & row, uint & col) const
{
	row = index >> 3;
	col = index & 7;
}

void BitBoard::initBoard()
{
  mHalfMoveClock = 0;
  mFullMoveCounter = 0;
	mWhiteToMove = true;
  mWhiteCastleKingSide = mWhiteCastleQueenSide = mWhiteEnPassant = true;
  mBlackCastleKingSide = mBlackCastleQueenSide  = mBlackEnPassant = true;
	mWhiteCheckmate = mBlackCheckmate = mStalemate = false;
	mStalemateCondition = NoStalemate;
	mEnPassantCaptureCol = 8;
	mWhiteKingCol = mBlackKingCol = 4;
	mBlackKingRow = BLACK_KING_START_ROW;

  for (uint i = 0; i < 64; i++)
		mPieces[i] = NoPiece;

	mPieces[0] = mPieces[7] = WhiteRook;
	mPieces[1] = mPieces[6] = WhiteKnight;
	mPieces[2] = mPieces[5] = WhiteBishop;
	mPieces[3] = WhiteQueen;
	mPieces[4] = WhiteKing;

	mPieces[56] = mPieces[63] = BlackRook;
	mPieces[57] = mPieces[62] = BlackKnight;
	mPieces[58] = mPieces[61] = BlackBishop;
	mPieces[59] = BlackQueen;
	mPieces[60] = BlackKing;

	// Initialize the pawns
  for (uint col = 0; col < 8; col++)
	{
		mPieces[8 + col] = WhitePawn;
		mPieces[48 + col] = BlackPawn;
	}

	static const PieceType whitePieceTypes[] = { WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook};
  static const uint whitePieceIndex[] = {0, 1, 2, 3, 4, 5, 6, 7};
  for (uint i = 0; i < 8; i++)
	{
		mWhitePieceEntries[i].pieceType = whitePieceTypes[i];
		mWhitePieceEntries[i].pieceIndex = whitePieceIndex[i];
		mWhitePieceEntries[i+8].pieceType = WhitePawn;
		mWhitePieceEntries[i+8].pieceIndex = whitePieceIndex[i] + 8;
	}

	static const PieceType blackPieceTypes[] = { BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook };
  static const uint blackPieceIndex[] = {56, 57, 58, 59, 60, 61, 62, 63};
  for (uint i = 0; i < 8; i++)
	{
		mBlackPieceEntries[i].pieceType = blackPieceTypes[i];
		mBlackPieceEntries[i].pieceIndex = blackPieceIndex[i];
		mBlackPieceEntries[i+8].pieceType = BlackPawn;
		mBlackPieceEntries[i+8].pieceIndex = blackPieceIndex[i] - 8;
	}

	mTotalWhiteQueens = mTotalWhiteKnights = mTotalWhiteRooks = mTotalWhiteBishops = 2;
	mTotalBlackQueens = mTotalBlackKnights = mTotalBlackRooks = mTotalBlackBishops = 2;
	mTotalWhitePawns = mTotalBlackPawns = 8;

	// Initialize the white piece bit boards
	mWhiteKingRow = WHITE_KING_START_ROW;
	mWhiteRooks = 0x81ULL;
	mWhiteKnights = 0x42ULL;
	mWhiteBishops = 0x24ULL;
	mWhiteQueens = 0x08ULL;
	mWhiteKings = 0x10ULL;
	mWhitePawns = (0xffULL) << 8ULL;
	mWhitePieces = mWhiteRooks | mWhiteKnights | mWhiteBishops | mWhiteQueens | mWhiteKings | mWhitePawns;

	// Initialize the black piece bit boards
	mBlackRooks = (0x81ULL << C64(56));
	mBlackKnights = (0x42ULL << C64(56));
	mBlackBishops = (0x24ULL << C64(56));
	mBlackQueens = (0x08ULL << C64(56));
	mBlackKings = (0x10ULL << C64(56));
	mBlackPawns = (0xffULL << C64(48));
	mBlackPieces = mBlackRooks | mBlackKnights | mBlackBishops | mBlackQueens | mBlackKings | mBlackPawns;
	
	mAllPieces = mWhitePieces | mBlackPieces;
	mEmptySquares = ~mAllPieces;
	mRotate90AllPieces = rotate90(mAllPieces);
	mRotate45RightPieces = rotate45(mAllPieces, a1h8Matrix);
	mRotate45LeftPieces = rotate45(mAllPieces, a8h1Matrix);

  for (uint index = 0; index < 64; index++)
	{
		mRotate45RightBB[index] = rotate45((C64(1)<<index), a1h8Matrix);
		mRotate45LeftBB[index] = rotate45((C64(1)<<index), a8h1Matrix);
		mRotate90BB[index] = rotate90((C64(1)<<index));
	}
}

void BitBoard::initMoves()
{
	initKnightAttacks();
	initPawnAttacks();
	initKingAttacks();
	initRookAttacks();
	initBishopAttacks();
}

void BitBoard::initBishopAttacks()
{
	// Initialize the diagonal shifts
  static const uint numElements[] = {1, 2, 3, 4, 5, 6, 7, 8, 7, 6, 5, 4, 3, 2, 1};
  static const uint shift[] = {0, 1, 3, 6, 10, 15, 21, 28, 36, 43, 49, 54, 58, 61, 63};

  uint index = 0;
  for (uint i = 0; i < 15; i++)
	{
    for (uint j = 0; j < numElements[i]; j++)
		{
			mA1H8Shifts[a1h8Matrix[index]] = shift[i];
			mA8H1Shifts[a8h1Matrix[index]] = shift[i];

      uint mask = 0, k = 0;
			while (k < numElements[i])
			{
				mask |= (C64(1) << k);
				k++;
			}
			
			mA1H8Mask[a1h8Matrix[index]] = mask;
			mA8H1Mask[a8h1Matrix[index]] = mask;
			index++;
		}
	}

	memset(mBishopAttacksNE, 0, sizeof(mBishopAttacksNE));
	memset(mBishopAttacksNW, 0, sizeof(mBishopAttacksNW));

	for (int index = 0; index < 64; index++)
	{
		int row = index >> 3;
		int col = index & 7;
		
		int rankCol = (col > row) ? row : col;
    for (uint occupancy = 0; occupancy <= mA1H8Mask[index]; occupancy++)
		{
      uint maskedOccupancy = mRankAttacks[rankCol][occupancy] & mA1H8Mask[index];

      ulonglong bb = C64(0);
			for (int i = rankCol+1; i < 8; i++)
			{
				if (maskedOccupancy & (C64(1) << i))
					bb |= (C64(1) << i);
			}

			for (int i = rankCol-1; i >= 0; i--)
			{
				if (maskedOccupancy & (C64(1) << i))
					bb |= (C64(1) << i);
			}

			mBishopAttacksNE[index][occupancy] = (rotateNeg45(bb << mA1H8Shifts[index], a1h8Matrix));
		}

		rankCol = (col > (7-row)) ? (7-row) : col;
    for (uint occupancy = 0; occupancy <= mA8H1Mask[index]; occupancy++)
		{
      uint maskedOccupancy = mRankAttacks[rankCol][occupancy] & mA8H1Mask[index];

      ulonglong bb = C64(0);
			for (int i = rankCol+1; i < 8; i++)
			{
				if (maskedOccupancy & (C64(1) << i))
					bb |= (C64(1) << i);
			}

			for (int i = rankCol-1; i >= 0; i--)
			{
				if (maskedOccupancy & (C64(1) << i))
					bb |= (C64(1) << i);
			}

			mBishopAttacksNW[index][occupancy] = (rotateNeg45(bb << mA8H1Shifts[index], a8h1Matrix));
		}
	}
}

void BitBoard::initKingAttacks()
{
	static const int kingIncr[8][2] = { {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1} };

	for (int iCell = 0; iCell < 64; iCell++) 
	{
		mKingAttacks[iCell] = C64(0);

		int row = iCell >> 3;
		int col = iCell & 7;
		for (int i = 0; i < 8; i++) 
		{
			int destRow = row + kingIncr[i][0];
			int destCol = col + kingIncr[i][1];
			if (squareInBoard(destRow, destCol))
			{
        uint index = getIndex(destRow, destCol);
				mKingAttacks[iCell] |= (C64(1) << index);
			}
		}
	}
}

void BitBoard::initKnightAttacks()
{
	static const int knightIncr[8][2] = { {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1,-2}, {-2,-1}, {-2, 1}, {-1, 2} };

	for (int iCell = 0; iCell < 64; iCell++) 
	{
		mKnightAttacks[iCell] = C64(0);

		int row = iCell >> 3;
		int col = iCell & 7;
		for (int i = 0; i < 8; i++) 
		{
			int destRow = row + knightIncr[i][0];
			int destCol = col + knightIncr[i][1];
			if (squareInBoard(destRow, destCol))
			{
        uint index = getIndex(destRow, destCol);
				mKnightAttacks[iCell] |= (C64(1) << index);
			}
		}
	}
}

void BitBoard::initPawnAttacks()
{
	static const int whitePawnIncr[2][2] = { {1, -1}, {1, 1} };
	static const int blackPawnIncr[2][2] = { {-1, -1}, {-1, 1} };
	
	static const int pawnIncr[2][2][2] = { { {1, -1}, {1, 1} }, { {-1,-1}, {-1, 1} } }; 

	for (int iCell = 0; iCell < 64; iCell++)
	{
		int row = iCell >> 3;
		int col = iCell & 7;
		mPawnAttacks[0][iCell] = mPawnAttacks[1][iCell] = C64(0);

		for (int iRow = 0; iRow < 2; iRow++)
		{
			for (int iColor = 0; iColor < 2; iColor++)
			{
				int destRow = row + pawnIncr[iColor][iRow][0];
				int destCol = col + pawnIncr[iColor][iRow][1];
				if (squareInBoard(destRow, destCol))
				{
          uint index = getIndex(destRow, destCol);
					mPawnAttacks[iColor][iCell] |= (C64(1) << index);
				}
			}
		}
	}
}

void BitBoard::initRookAttacks()
{
	for (int col = 0; col < 8; col++)
	{
		for (int occupancy = 0; occupancy < 256; occupancy++)
		{
      ulonglong bb = 0;

			for (int file = col-1; file >= 0; file--)
			{
				bb |= (C64(1) << file);
				if (occupancy & (C64(1) << file))
					break;
			}

			for (int file = col + 1; file < 8; file++)
			{
				bb |= (C64(1) << file);
				if (occupancy & (C64(1) << file))
					break;
			}

			mRankAttacks[col][occupancy] = bb;
			mFileAttacks[col][occupancy] = rotate90(bb);
		}
	}
}

bool BitBoard::isLegal() const
{
  if (!mWhiteToMove && isCellAttacked(mWhiteKingRow, mWhiteKingCol, true))
		return false;

  if (mWhiteToMove && isCellAttacked(mBlackKingRow, mBlackKingCol, false))
		return false;

	return true;
}

void BitBoard::loadState()
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

bool BitBoard::makeMove(const Move & newMove)
{
	//StopwatchHolder holder(mMakeMoveTimer);

	// Save off the irreversible game state
	saveState();

	// Get some information about the move
  uint sourceRow = newMove.sourceRow();
  uint sourceCol = newMove.sourceColumn();
  uint destRow = newMove.destinationRow();
  uint destCol = newMove.destinationColumn();
  uint sourceIndex = sourceRow*8 + sourceCol;
  uint destIndex = destRow*8 + destCol;
  PieceType pieceType = getPieceType(sourceRow, sourceCol);

	// Get move related bit boards
  ulonglong fromBB = (C64(1) << sourceIndex);
  ulonglong toBB = (C64(1) << destIndex);
  ulonglong fromToBB = fromBB ^ toBB;

  ulonglong fromRot90 = mRotate90BB[sourceIndex];
  ulonglong toRot90 = mRotate90BB[destIndex];
  ulonglong fromToRot90 = fromRot90 ^ toRot90;

  ulonglong fromRot45Right = mRotate45RightBB[sourceIndex];
  ulonglong toRot45Right = mRotate45RightBB[destIndex];
  ulonglong fromToRot45Right = fromRot45Right ^ toRot45Right;

  ulonglong fromRot45Left = mRotate45LeftBB[sourceIndex];
  ulonglong toRot45Left = mRotate45LeftBB[destIndex];
  ulonglong fromToRot45Left = fromRot45Left ^ toRot45Left;

	// Update the half move clock
	mHalfMoveClock++;

	// Reset en passant state
	mWhiteEnPassant = mBlackEnPassant = false;
	mEnPassantCaptureCol = 8;

	// The moving piece always leaves an empty square
	mPieces[sourceIndex] = NoPiece;
	mPieces[destIndex] = pieceType;

	// Update the rotated piece bit boards
	mRotate90AllPieces ^= fromToRot90;
	mRotate45LeftPieces ^= fromToRot45Left;
	mRotate45RightPieces ^= fromToRot45Right;

	switch (pieceType)
	{
	case WhitePawn:
		mHalfMoveClock = 0;
		mWhitePawns ^= fromToBB;
		mWhitePieces ^= fromToBB;
    if (newMove.isDoublePawnPush())
		{
			if (destCol != 0) 
			{
        ulonglong bb = (C64(1) << (destIndex-1));
				if (mBlackPawns & bb)
				{
					mBlackEnPassant = true;
					mEnPassantCaptureCol = destCol;
				}
			}
			if (destCol != 7)
			{
        ulonglong bb = (C64(1) << (destIndex+1));
				if (mBlackPawns & bb) 
				{
					mBlackEnPassant = true;
					mEnPassantCaptureCol = destCol;
				}
			}
		}
    if (newMove.isPromotion())
		{
			mWhitePawns ^= toBB;
			mTotalWhitePawns--;
			
      PieceType promotedPiece = newMove.promotedPiece();
      mPieces[destIndex] = promotedPiece;

      switch (promotedPiece)
			{
			case WhiteQueen:
				mWhiteQueens ^= toBB;
				mTotalWhiteQueens++;
				break;
			case WhiteBishop:
				mWhiteBishops ^= toBB;
				mTotalWhiteBishops++;
				break;
			case WhiteRook:
				mWhiteRooks ^= toBB;
				mTotalWhiteRooks++;
				break;
			case WhiteKnight:
				mWhiteKnights ^= toBB;
				mTotalWhiteKnights++;
				break;
      default:
        break;
			}
		}
		break;
	case WhiteKnight:
		mWhiteKnights ^= fromToBB;
		mWhitePieces ^= fromToBB;
		break;
	case WhiteBishop:
		mWhiteBishops ^= fromToBB;
		mWhitePieces ^= fromToBB;
		break;
	case WhiteRook:
		mWhiteRooks ^= fromToBB;
		mWhitePieces ^= fromToBB;
		if (sourceIndex == 0) mWhiteCastleQueenSide = false;
		if (sourceIndex == 7) mWhiteCastleKingSide = false;
		break;
	case WhiteQueen:
		mWhiteQueens ^= fromToBB;
		mWhitePieces ^= fromToBB;
		break;
	case WhiteKing:
		mWhiteKings ^= fromToBB;
		mWhitePieces ^= fromToBB;
		mWhiteCastleKingSide = mWhiteCastleQueenSide = false;
		mWhiteKingRow = destRow;
		mWhiteKingCol = destCol;
    if (newMove.isKingCastle())
		{
      uint index1 = sourceRow*8 + 7;
      uint index2 = sourceRow*8 + KING_CASTLE_ROOK_COL;
      ulonglong moveBB = ((C64(1) << index1) ^ (C64(1) << index2));

			mWhiteRooks ^= moveBB;
			mWhitePieces ^= moveBB;
			mRotate90AllPieces ^= (mRotate90BB[index1] ^ mRotate90BB[index2]);
			mRotate45RightPieces ^= (mRotate45RightBB[index1] ^ mRotate45RightBB[index2]);
			mRotate45LeftPieces ^= (mRotate45LeftBB[index1] ^ mRotate45LeftBB[index2]);

			mPieces[index1] = NoPiece; // Rook leaves an empty square
			mPieces[index2] = WhiteRook;
		}
    if (newMove.isQueenCastle())
		{
      uint index1 = sourceRow*8;
      uint index2 = sourceRow*8 + QUEEN_CASTLE_ROOK_COL;
      ulonglong moveBB = ((C64(1) << index1) ^ (C64(1) << index2));

			mWhiteRooks ^= moveBB;
			mWhitePieces ^= moveBB;
			mRotate90AllPieces ^= (mRotate90BB[index1] ^ mRotate90BB[index2]);
			mRotate45RightPieces ^= (mRotate45RightBB[index1] ^ mRotate45RightBB[index2]);
			mRotate45LeftPieces ^= (mRotate45LeftBB[index1] ^ mRotate45LeftBB[index2]);

			mPieces[index1] = NoPiece; // Rook leaves an empty square
			mPieces[index2] = WhiteRook;
		}
		break;
	case BlackPawn:
		mHalfMoveClock = 0;
		mBlackPawns ^= fromToBB;
		mBlackPieces ^= fromToBB;
    if (newMove.isDoublePawnPush())
		{
			if (destCol != 0) 
			{
        ulonglong bb = (C64(1) << (destIndex-1));
				if (mWhitePawns & bb)
				{
					mWhiteEnPassant = true;
					mEnPassantCaptureCol = destCol;
				}
			}
			if (destCol != 7)
			{
        ulonglong bb = (C64(1) << (destIndex+1));
				if (mWhitePawns & bb) 
				{
					mWhiteEnPassant = true;
					mEnPassantCaptureCol = destCol;
				}
			}
		}
    if (newMove.isPromotion())
		{
			mBlackPawns ^= toBB;
			mTotalBlackPawns--;

      PieceType promotedPiece = newMove.promotedPiece();
      mPieces[destIndex] = promotedPiece;

      switch (promotedPiece)
			{
			case BlackQueen:
				mBlackQueens ^= toBB;
				mTotalBlackQueens++;
				break;
			case BlackBishop:
				mBlackBishops ^= toBB;
				mTotalBlackBishops++;
				break;
			case BlackRook:
				mBlackRooks ^= toBB;
				mTotalBlackRooks++;
				break;
			case BlackKnight:
				mBlackKnights ^= toBB;
				mTotalBlackKnights++;
				break;
      default:
        break;
			}
		}
		break;
	case BlackKnight:
		mBlackKnights ^= fromToBB;
		mBlackPieces ^= fromToBB;
		break;
	case BlackBishop:
		mBlackBishops ^= fromToBB;
		mBlackPieces ^= fromToBB;
		break;
	case BlackRook:
		mBlackRooks ^= fromToBB;
		mBlackPieces ^= fromToBB;
		if (sourceIndex == 56) mBlackCastleQueenSide = false;
		if (sourceIndex == 63) mBlackCastleKingSide = false;
		break;
	case BlackQueen:
		mBlackQueens ^= fromToBB;
		mBlackPieces ^= fromToBB;
		break;
	case BlackKing:
		mBlackKings ^= fromToBB;
		mBlackPieces ^= fromToBB;
		mBlackCastleKingSide = mBlackCastleQueenSide = false;
		mBlackKingRow = destRow;
		mBlackKingCol = destCol;
    if (newMove.isKingCastle())
		{
      uint index1 = sourceRow*8 + 7;
      uint index2 = sourceRow*8 + KING_CASTLE_ROOK_COL;
      ulonglong moveBB = ((C64(1) << index1) ^ (C64(1) << index2));

			mBlackRooks ^= moveBB;
			mBlackPieces ^= moveBB;
			mRotate90AllPieces ^= (mRotate90BB[index1] ^ mRotate90BB[index2]);
			mRotate45RightPieces ^= (mRotate45RightBB[index1] ^ mRotate45RightBB[index2]);
			mRotate45LeftPieces ^= (mRotate45LeftBB[index1] ^ mRotate45LeftBB[index2]);

			mPieces[index1] = NoPiece; // Rook leaves an empty square
			mPieces[index2] = BlackRook;
		}
    if (newMove.isQueenCastle())
		{
      uint index1 = sourceRow*8;
      uint index2 = sourceRow*8 + QUEEN_CASTLE_ROOK_COL;
      ulonglong moveBB = ((C64(1) << index1) ^ (C64(1) << index2));

			mBlackRooks ^= moveBB;
			mBlackPieces ^= moveBB;
			mRotate90AllPieces ^= (mRotate90BB[index1] ^ mRotate90BB[index2]);
			mRotate45RightPieces ^= (mRotate45RightBB[index1] ^ mRotate45RightBB[index2]);
			mRotate45LeftPieces ^= (mRotate45LeftBB[index1] ^ mRotate45LeftBB[index2]);

			mPieces[index1] = NoPiece; // Rook leaves an empty square
			mPieces[index2] = BlackRook;
		}
		break;
  default:
    break;
	}

	// Perform a capture move
  if (newMove.isCapture() && !newMove.isEnPassant())
	{
		mHalfMoveClock = 0;

		mRotate90AllPieces ^= toRot90;
		mRotate45RightPieces ^= toRot45Right;
		mRotate45LeftPieces ^= toRot45Left;

    PieceType capturePiece = newMove.capturedPiece();
		switch (capturePiece)
		{
		case WhitePawn:
			mWhitePawns ^= toBB;
			mWhitePieces ^= toBB;
			mTotalWhitePawns--;
			break;
		case WhiteKnight:
			mWhiteKnights ^= toBB;
			mWhitePieces ^= toBB;
			mTotalWhiteKnights--;
			break;
		case WhiteBishop:
			mWhiteBishops ^= toBB;
			mWhitePieces ^= toBB;
			mTotalWhiteBishops--;
			break;
		case WhiteRook:
			mWhiteRooks ^= toBB;
			mWhitePieces ^= toBB;
			mTotalWhiteRooks--;
			if (destIndex == 0) mWhiteCastleQueenSide = false;
			if (destIndex == 7) mWhiteCastleKingSide = false;
			break;
		case WhiteQueen:
			mWhiteQueens ^= toBB;
			mWhitePieces ^= toBB;
			mTotalWhiteQueens--;
			break;
		case BlackPawn:
			mBlackPawns ^= toBB;
			mBlackPieces ^= toBB;
			mTotalBlackPawns--;
			break;
		case BlackKnight:
			mBlackKnights ^= toBB;
			mBlackPieces ^= toBB;
			mTotalBlackKnights--;
			break;
		case BlackBishop:
			mBlackBishops ^= toBB;
			mBlackPieces ^= toBB;
			mTotalBlackBishops--;
			break;
		case BlackRook:
			mBlackRooks ^= toBB;
			mBlackPieces ^= toBB;
			mTotalBlackRooks--;
			if (destIndex == 56) mBlackCastleQueenSide = false;
			if (destIndex == 63) mBlackCastleKingSide = false;
			break;
		case BlackQueen:
			mBlackQueens ^= toBB;
			mBlackPieces ^= toBB;
			mTotalBlackQueens--;
			break;
    default:
      break;
		}
	}

	// Handle en-passant moves
  if (newMove.isEnPassant())
	{
    uint pawnIndex = sourceRow*8 + destCol;
    ulonglong pawnBB = (C64(1) << pawnIndex);

		mRotate90AllPieces ^= mRotate90BB[pawnIndex];
		mRotate45LeftPieces ^= mRotate45LeftBB[pawnIndex];
		mRotate45RightPieces ^= mRotate45RightBB[pawnIndex];
		mPieces[pawnIndex] = NoPiece; // Remove the captured piece

		switch (pieceType)
		{
		case WhitePawn:
			mBlackPawns ^= pawnBB;
			mBlackPieces ^= pawnBB;
		    mTotalBlackPawns--;
			break;
		case BlackPawn:
			mWhitePawns ^= pawnBB;
			mWhitePieces ^= pawnBB;
			mTotalWhitePawns--;
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

	// Update the combination bit boards
	mAllPieces = mWhitePieces | mBlackPieces;
	mEmptySquares = ~mAllPieces;

	// Flip move
	mWhiteToMove = !mWhiteToMove;

  return true;
}

void BitBoard::updatePieceEntriesUnmake(Move & undoMove)
{
}

void BitBoard::saveState()
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

void BitBoard::serializeBitBoardMoves(ulonglong bitBoard, uint sourceIndex, MoveList & moveList)
{
	assert(bitBoard);

	//StopwatchHolder holder(mSerializeMoves);

  uint sourceRow = sourceIndex >> 3;
  uint sourceCol = sourceIndex & 7;
	do 
	{
		int destIndex = bitScanForward(bitBoard); // square index from 0..63
    uint destRow = destIndex >> 3;
    uint destCol = destIndex & 7;
		PieceType otherPiece = mPieces[destIndex];

		Move newMove(sourceRow, sourceCol, destRow, destCol);
		if ( (C64(1) << destIndex) & mAllPieces)
		{
      newMove.setCapture();
      newMove.setCapturedPiece(otherPiece);
		}
    moveList.addMove(newMove);

	} while (bitBoard &= bitBoard-1); // reset LS1B
}

bool BitBoard::squareInBoard(int row, int col)
{
	return (row >= 0 && row <= 7 && col >= 0 && col <= 7);
}

void BitBoard::trimCandidateMoves(MoveList & candidateMoves, MoveList & legalMoves)
{
	//StopwatchHolder holder(mTrimMoveTimer);

	// Loop through the candidate moves and determine which ones are legal
  for (uint i = 0; i < candidateMoves.totalMoves(); i++) {
    Move currentMove(candidateMoves[i]);
		makeMove(currentMove);
		if (isLegal())
      legalMoves.addMove(currentMove);
		unmakeMove(currentMove);
	}
}

bool BitBoard::unmakeMove(const Move & undoMove)
{
	//StopwatchHolder holder(mUnmakeMoveTimer);

	// Get some information about the move
  uint sourceRow = undoMove.sourceRow();
  uint sourceCol = undoMove.sourceColumn();
  uint destRow = undoMove.destinationRow();
  uint destCol = undoMove.destinationColumn();
  uint sourceIndex = getIndex(sourceRow, sourceCol);
  uint destIndex = getIndex(destRow, destCol);

	PieceType pieceType = mPieces[destIndex];

	// Update the piece type array
	mPieces[destIndex] = NoPiece;
	mPieces[sourceIndex] = pieceType;

	// Get move related bit boards
  ulonglong fromBB = (C64(1) << sourceIndex);
  ulonglong toBB = (C64(1) << destIndex);
  ulonglong fromToBB = fromBB ^ toBB;

  ulonglong fromRot90 = mRotate90BB[sourceIndex];
  ulonglong toRot90 = mRotate90BB[destIndex];
  ulonglong fromToRot90 = fromRot90 ^ toRot90;

  ulonglong fromRot45Right = mRotate45RightBB[sourceIndex];
  ulonglong toRot45Right = mRotate45RightBB[destIndex];
  ulonglong fromToRot45Right = fromRot45Right ^ toRot45Right;

  ulonglong fromRot45Left = mRotate45LeftBB[sourceIndex];
  ulonglong toRot45Left = mRotate45LeftBB[destIndex];
  ulonglong fromToRot45Left = fromRot45Left ^ toRot45Left;

	// Update the rotated pieces
	mRotate90AllPieces ^= fromToRot90;
	mRotate45LeftPieces ^= fromToRot45Left;
	mRotate45RightPieces ^= fromToRot45Right;

	// Do piece specific updates
	switch (pieceType)
	{
	case WhitePawn:
		mWhitePawns ^= fromToBB;
		mWhitePieces ^= fromToBB;
		break;
	case WhiteRook:
		mWhiteRooks ^= fromToBB;
		mWhitePieces ^= fromToBB;
    if (undoMove.isPromotion())
		{
			mWhiteRooks ^= fromBB;
			mWhitePawns ^= fromBB;
			mTotalWhitePawns++;
			mTotalWhiteRooks--;
			mPieces[sourceIndex] = WhitePawn;
		}
		break;
	case WhiteKnight:
		mWhiteKnights ^= fromToBB;
		mWhitePieces ^= fromToBB;
    if (undoMove.isPromotion())
		{
			mWhiteKnights ^= fromBB;
			mWhitePawns ^= fromBB;
			mTotalWhitePawns++;
			mTotalWhiteKnights--;
			mPieces[sourceIndex] = WhitePawn;
		}
		break;
	case WhiteBishop:
		mWhiteBishops ^= fromToBB;
		mWhitePieces ^= fromToBB;
    if (undoMove.isPromotion())
		{
			mWhiteBishops ^= fromBB;
			mWhitePawns ^= fromBB;
			mTotalWhitePawns++;
			mTotalWhiteBishops--;
			mPieces[sourceIndex] = WhitePawn;
		}
		break;
	case WhiteQueen:
		mWhiteQueens ^= fromToBB;
		mWhitePieces ^= fromToBB;
    if (undoMove.isPromotion())
		{
			mWhiteQueens ^= fromBB;
			mWhitePawns ^= fromBB;
			mTotalWhitePawns++;
			mTotalWhiteQueens--;
			mPieces[sourceIndex] = WhitePawn;
		}
		break;
	case WhiteKing:
		mWhiteKings ^= fromToBB;
		mWhitePieces ^= fromToBB;
		mWhiteKingRow = sourceRow;
		mWhiteKingCol = sourceCol;
    if (undoMove.isKingCastle())
		{
      uint index1 = sourceRow*8 + 7;
      uint index2 = sourceRow*8 + KING_CASTLE_ROOK_COL;

			mPieces[index1] = WhiteRook;
			mPieces[index2] = NoPiece;

      ulonglong moveBB = ( (C64(1) << index1) ^ (C64(1) << index2) );
			mWhiteRooks ^= moveBB;
			mWhitePieces ^= moveBB;
			mRotate90AllPieces ^= (mRotate90BB[index1] ^ mRotate90BB[index2]);
			mRotate45RightPieces ^= (mRotate45RightBB[index1] ^ mRotate45RightBB[index2]);
			mRotate45LeftPieces ^= (mRotate45LeftBB[index1] ^ mRotate45LeftBB[index2]);
		}
    if (undoMove.isQueenCastle())
		{
      uint index1 = sourceRow*8;
      uint index2 = sourceRow*8 + QUEEN_CASTLE_ROOK_COL;

			mPieces[index1] = WhiteRook;
			mPieces[index2] = NoPiece;

      ulonglong moveBB = ( (C64(1) << index1) ^ (C64(1) << index2) );
			mWhiteRooks ^= moveBB;
			mWhitePieces ^= moveBB;
			mRotate90AllPieces ^= (mRotate90BB[index1] ^ mRotate90BB[index2]);
			mRotate45RightPieces ^= (mRotate45RightBB[index1] ^ mRotate45RightBB[index2]);
			mRotate45LeftPieces ^= (mRotate45LeftBB[index1] ^ mRotate45LeftBB[index2]);
		}
		break;
	case BlackPawn:
		mBlackPawns ^= fromToBB;
		mBlackPieces ^= fromToBB;
		break;
	case BlackRook:
		mBlackRooks ^= fromToBB;
		mBlackPieces ^= fromToBB;
    if (undoMove.isPromotion())
		{
			mBlackRooks ^= fromBB;
			mBlackPawns ^= fromBB;
			mTotalBlackPawns++;
			mTotalBlackRooks--;
			mPieces[sourceIndex] = BlackPawn;
		}
		break;
	case BlackKnight:
		mBlackKnights ^= fromToBB;
		mBlackPieces ^= fromToBB;
    if (undoMove.isPromotion())
		{
			mBlackKnights ^= fromBB;
			mBlackPawns ^= fromBB;
			mTotalBlackPawns++;
			mTotalBlackKnights--;
			mPieces[sourceIndex] = BlackPawn;
		}
		break;
	case BlackBishop:
		mBlackBishops ^= fromToBB;
		mBlackPieces ^= fromToBB;
    if (undoMove.isPromotion())
		{
			mBlackBishops ^= fromBB;
			mBlackPawns ^= fromBB;
			mTotalBlackPawns++;
			mTotalBlackBishops--;
			mPieces[sourceIndex] = BlackPawn;
		}
		break;
	case BlackQueen:
		mBlackQueens ^= fromToBB;
		mBlackPieces ^= fromToBB;
    if (undoMove.isPromotion())
		{
			mBlackQueens ^= fromBB;
			mBlackPawns ^= fromBB;
			mTotalBlackPawns++;
			mTotalBlackQueens--;
			mPieces[sourceIndex] = BlackPawn;
		}
		break;
	case BlackKing:
		mBlackKings ^= fromToBB;
		mBlackPieces ^= fromToBB;
		mBlackKingRow = sourceRow;
		mBlackKingCol = sourceCol;
    if (undoMove.isKingCastle())
		{
      uint index1 = sourceRow*8 + 7;
      uint index2 = sourceRow*8 + KING_CASTLE_ROOK_COL;

			mPieces[index1] = BlackRook;
			mPieces[index2] = NoPiece;

      ulonglong moveBB = ( (C64(1) << index1) ^ (C64(1) << index2) );
			mBlackRooks ^= moveBB;
			mBlackPieces ^= moveBB;
			mRotate90AllPieces ^= (mRotate90BB[index1] ^ mRotate90BB[index2]);
			mRotate45RightPieces ^= (mRotate45RightBB[index1] ^ mRotate45RightBB[index2]);
			mRotate45LeftPieces ^= (mRotate45LeftBB[index1] ^ mRotate45LeftBB[index2]);
		}
    if (undoMove.isQueenCastle())
		{
      uint index1 = sourceRow*8;
      uint index2 = sourceRow*8 + QUEEN_CASTLE_ROOK_COL;

			mPieces[index1] = BlackRook;
			mPieces[index2] = NoPiece;

      ulonglong moveBB = ( (C64(1) << index1) ^ (C64(1) << index2) );
			mBlackRooks ^= moveBB;
			mBlackPieces ^= moveBB;
			mRotate90AllPieces ^= (mRotate90BB[index1] ^ mRotate90BB[index2]);
			mRotate45RightPieces ^= (mRotate45RightBB[index1] ^ mRotate45RightBB[index2]);
			mRotate45LeftPieces ^= (mRotate45LeftBB[index1] ^ mRotate45LeftBB[index2]);
		}
		break;
  default:
    break;
	}

	// Perform a capture move
  if (undoMove.isCapture() && !undoMove.isEnPassant())
	{
    PieceType capturePiece = undoMove.capturedPiece();
		mPieces[destIndex] = capturePiece;

		mRotate90AllPieces ^= toRot90;
		mRotate45RightPieces ^= toRot45Right;
		mRotate45LeftPieces ^= toRot45Left;

		switch (capturePiece)
		{
		case WhitePawn:
			mWhitePawns ^= toBB;
			mWhitePieces ^= toBB;
			mTotalWhitePawns++;
			break;
		case WhiteRook:
			mWhiteRooks ^= toBB;
			mWhitePieces ^= toBB;
			mTotalWhiteRooks++;
			break;
		case WhiteKnight:
			mWhiteKnights ^= toBB;
			mWhitePieces ^= toBB;
			mTotalWhiteKnights++;
			break;
		case WhiteBishop:
			mWhiteBishops ^= toBB;
			mWhitePieces ^= toBB;
			mTotalWhiteBishops++;
			break;
		case WhiteQueen:
			mWhiteQueens ^= toBB;
			mWhitePieces ^= toBB;
			mTotalWhiteQueens++;
			break;
		case BlackPawn:
			mBlackPawns ^= toBB;
			mBlackPieces ^= toBB;
			mTotalBlackPawns++;
			break;
		case BlackRook:
			mBlackRooks ^= toBB;
			mBlackPieces ^= toBB;
			mTotalBlackRooks++;
			break;
		case BlackKnight:
			mBlackKnights ^= toBB;
			mBlackPieces ^= toBB;
			mTotalBlackKnights++;
			break;
		case BlackBishop:
			mBlackBishops ^= toBB;
			mBlackPieces ^= toBB;
			mTotalBlackBishops++;
			break;
		case BlackQueen:
			mBlackQueens ^= toBB;
			mBlackPieces ^= toBB;
			mTotalBlackQueens++;
			break;
    default:
      break;
		}
	}

  if (undoMove.isEnPassant())
	{
    uint pawnIndex = sourceRow*8 + destCol;
    ulonglong pawnBB = C64(1) << pawnIndex;

		mRotate90AllPieces ^= mRotate90BB[pawnIndex];
		mRotate45RightPieces ^= mRotate45RightBB[pawnIndex];
		mRotate45LeftPieces ^= mRotate45LeftBB[pawnIndex];

    PieceType capturePiece = undoMove.capturedPiece();
		switch (capturePiece)
		{
		case WhitePawn:
			mWhitePawns ^= pawnBB;
			mWhitePieces ^= pawnBB;
			mPieces[pawnIndex] = WhitePawn;
			mTotalWhitePawns++;
			break;
		case BlackPawn:
			mBlackPawns ^= pawnBB;
			mBlackPieces ^= pawnBB;
			mPieces[pawnIndex] = BlackPawn;
			mTotalBlackPawns++;
    default:
      break;
		}
	}

	// Update the combination bit boards
	mAllPieces = mWhitePieces | mBlackPieces;
	mEmptySquares = ~mAllPieces;

	// Update check state
  if (!mWhiteToMove)
    mBlackChecked = isCellAttacked(mBlackKingRow, mBlackKingCol, false);
  else
    mWhiteChecked = isCellAttacked(mWhiteKingRow, mWhiteKingCol, true);

	// Flip move
	mWhiteToMove = !mWhiteToMove;

	// Load back in the previous state for castling and en-passant abilites 
	// as well as the half move clock
  loadState();

  return true;
}

//#include <windows.h>
void BitBoard::writeBitBoard(ulonglong bitBoard) const
{
  std::ostringstream oss;

	for (int i = 7; i >= 0; i--)
	{
		for (int j = 0; j < 8; j++)
		{
			int shift = 8*i + j ;
			if ( (bitBoard >> shift) & 0x1)
				oss << 'X';
			else 
				oss << '-';
		}
		oss << "\n";
	}

	std::cout << oss.str() << "\n\n";
  //OutputDebugStringA(oss.str().c_str());
}

void BitBoard::writeOccupancy(u_char occupancy) const
{
  std::ostringstream oss;
	for (int i = 0; i < 8; i++)
	{
		if ( (occupancy >> i) & 0x01)
			oss << 'X';
		else 
			oss << '-';
	}

	std::cout << oss.str() << "\n\n";
}
