#include "board8x8.h"
#include "move.h"
#include "movelist.h"

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

Board8x8::Board8x8()
{
  initBoard();
  initMoves();
}

bool Board8x8::canBlackCastleKingSide() const
{
  return mBlackCastleKingSide;
}

bool Board8x8::canBlackCastleQueenSide() const
{
  return mBlackCastleQueenSide;
}

bool Board8x8::canWhiteCastleKingSide() const
{
  return mWhiteCastleKingSide;
}

bool Board8x8::canWhiteCastleQueenSide() const
{
  return mWhiteCastleQueenSide;
}

uint Board8x8::generateLegalMoves(MoveList & moveList)
{
  // Generate the list of candidate moves
  MoveList candidateMoves;
  generateCandidateMoves(candidateMoves);

  // Trim the candidate moves based on the legality of the candidate moves
  trimCandidateMoves(candidateMoves, moveList);

  return moveList.totalMoves();
}

uint Board8x8::generateLegalMoves(uint row, uint col, MoveList & moveList)
{
  // Generate the list of candidate moves
  MoveList candidateMoves;
  generateCandidateMoves(row, col, candidateMoves);

  // Trim the candidate moves based on the legality of the candidate moves
  trimCandidateMoves(candidateMoves, moveList);

  return moveList.totalMoves();
}

void Board8x8::generateBishopMoves(uint row, uint col, MoveList & moveList)
{
  uint index = getIndex(row, col);
  generateMoves(mBishopMovesNorthEast[index], row, col, moveList, true);
  generateMoves(mBishopMovesSouthWest[index], row, col, moveList, true);
  generateMoves(mBishopMovesNorthWest[index], row, col, moveList, true);
  generateMoves(mBishopMovesSouthEast[index], row, col, moveList, true);
}

void Board8x8::generateBlackPawnMoves(uint row, uint col, MoveList & moveList)
{
  static const PieceType promotedPieceTypes[] =
  { BlackQueen, BlackRook, BlackBishop, BlackKnight };

  uint sourceIndex = getIndex(row, col);
  std::list<uint> listMoves = mBlackPawnMoves[sourceIndex];
  for (std::list<uint>::iterator itr = listMoves.begin(); itr != listMoves.end(); ++itr) {
    uint destIndex(*itr);
    uint destRow = destIndex >> 3;
    uint destCol = destIndex & 7;

    if (row == 1) {
      if (col == destCol) {
        if (mPieceType[(row-1)*8+col] == NoPiece) {
          for (int i = 0; i < 4; i++) {
            Move promoMove(row, col, destRow, destCol, NoPiece, promotedPieceTypes[i]);
            moveList.addMove(promoMove);
          }
        }
      }
      else {
        PieceType otherPiece = mPieceType[destIndex];
        if (isPiece(otherPiece) && isWhitePiece(otherPiece)) {
          for (int i = 0; i < 4; i++) {
            Move promoMove(row, col, destRow, destCol, otherPiece, promotedPieceTypes[i]);
            moveList.addMove(promoMove);
          }
        }
      }
    }
    else {
      if (col == destCol) {
        bool doublePush = false;
        bool singlePush = mPieceType[(row-1)*8+col] == NoPiece;
        if (singlePush)
          doublePush = mPieceType[(row-2)*8+col] == NoPiece;

        if ( (row-destRow) == 1 && singlePush) {
          Move newMove(row, col, destRow, destCol);
          moveList.addMove(newMove);
        }

        if ( (row-destRow) == 2 && doublePush) {
          Move newMove(row, col, destRow, destCol);
          newMove.setDoublePawnPush();
          moveList.addMove(newMove);
        }
      }
      else {
        PieceType otherPiece = mPieceType[destIndex];
        if (isPiece(otherPiece) && isWhitePiece(otherPiece)) {
          Move newMove(row, col, destRow, destCol, otherPiece);
          moveList.addMove(newMove);
        }
      }
    }
  }

  // Check en-passant
  if (mBlackEnPassant && row == 3) {
    if (col != 7) {
      PieceType pieceType = getPieceType(row, col+1);
      if (mEnPassantCaptureCol == col + 1 && pieceType == WhitePawn) {
        Move newMove(row, col, row-1, col+1, WhitePawn);
        newMove.setEnPassant();
        moveList.addMove(newMove);
      }
    }

    if (col != 0) {
      PieceType pieceType = getPieceType(row, col-1);
      if (mEnPassantCaptureCol == col - 1 && pieceType == WhitePawn) {
        Move newMove(row, col, row-1, col-1, WhitePawn);
        newMove.setEnPassant();
        moveList.addMove(newMove);
      }
    }
  }
}

void Board8x8::generateCandidateMoves(MoveList & moveList)
{
  for (uint index = 0; index < 64; index++) {
    uint row = index >> 3;
    uint col = index & 7;

    PieceType pieceType = mPieceType[index];
    if (mWhiteToMove) {
      switch (pieceType) {
      case WhitePawn:
        generateWhitePawnMoves(row, col, moveList);
        break;
      case WhiteKnight:
        generateKnightMoves(row, col, moveList);
        break;
      case WhiteBishop:
        generateBishopMoves(row, col, moveList);
        break;
      case WhiteRook:
        generateRookMoves(row, col, moveList);
        break;
      case WhiteQueen:
        generateBishopMoves(row, col, moveList);
        generateRookMoves(row, col, moveList);
        break;
      case WhiteKing:
        generateKingMoves(row, col, moveList);
        break;
      default:
        break;
      }
    }
    else {
      switch (pieceType) {
      case BlackPawn:
        generateBlackPawnMoves(row, col, moveList);
        break;
      case BlackKnight:
        generateKnightMoves(row, col, moveList);
        break;
      case BlackBishop:
        generateBishopMoves(row, col, moveList);
        break;
      case BlackRook:
        generateRookMoves(row, col, moveList);
        break;
      case BlackQueen:
        generateBishopMoves(row, col, moveList);
        generateRookMoves(row, col, moveList);
        break;
      case BlackKing:
        generateKingMoves(row, col, moveList);
        break;
      default:
        break;
      }
    }
  }
}

void Board8x8::generateCandidateMoves(uint row, uint col, MoveList & moveList)
{
  PieceType pieceType = getPieceType(row, col);
  if (pieceType == NoPiece)
    return;

  switch (pieceType) {
  case WhitePawn:
    generateWhitePawnMoves(row, col, moveList);
    break;
  case BlackPawn:
    generateBlackPawnMoves(row, col, moveList);
    break;
  case WhiteKnight:
  case BlackKnight:
    generateKnightMoves(row, col, moveList);
    break;
  case WhiteBishop:
  case BlackBishop:
    generateBishopMoves(row, col, moveList);
    break;
  case WhiteRook:
  case BlackRook:
    generateRookMoves(row, col, moveList);
    break;
  case WhiteQueen:
  case BlackQueen:
    generateBishopMoves(row, col, moveList);
    generateRookMoves(row, col, moveList);
    break;
  case WhiteKing:
  case BlackKing:
    generateKingMoves(row, col, moveList);
    break;
  default:
    break;
  }
}

void Board8x8::generateKingMoves(uint row, uint col, MoveList & moveList)
{
  uint index = getIndex(row, col);
  generateMoves(mKingMoves[index], row, col, moveList, false);

  bool queenSide = (mWhiteToMove) ? mWhiteCastleQueenSide : mBlackCastleQueenSide;
  bool kingSide = (mWhiteToMove) ? mWhiteCastleKingSide : mBlackCastleKingSide;
  uint rowCheck = (mWhiteToMove) ? WHITE_KING_START_ROW : BLACK_KING_START_ROW;

  if (kingSide && getPieceType(rowCheck, 5) == NoPiece && getPieceType(rowCheck, 6) == NoPiece) {
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

void Board8x8::generateKnightMoves(uint row, uint col, MoveList & moveList)
{
  uint index = getIndex(row, col);
  generateMoves(mKnightMoves[index], row, col, moveList, false);
}

void Board8x8::generateMoves(
    std::list<uint> & indexList,
    uint row,
    uint col,
    MoveList & moveList,
    bool isSlider)
{
  for (std::list<uint>::iterator itr = indexList.begin(); itr != indexList.end(); ++itr) {
    uint destIndex(*itr);
    uint destRow = destIndex >> 3;
    uint destCol = destIndex & 7;

    PieceType otherPiece = mPieceType[destIndex];

    if (otherPiece != NoPiece) {
      if ( (mWhiteToMove && isBlackPiece(otherPiece)) || (!mWhiteToMove && isWhitePiece(otherPiece)) ) {
        Move newMove(row, col, destRow, destCol, otherPiece);
        moveList.addMove(newMove);
      }

      if (isSlider)
        break;
    }
    else {
      Move newMove(row, col, destRow, destCol);
      moveList.addMove(newMove);
    }
  }
}

void Board8x8::generateQueenMoves(uint row, uint col, MoveList & moveList)
{
  generateBishopMoves(row, col, moveList);
  generateRookMoves(row, col, moveList);
}

void Board8x8::generateRookMoves(uint row, uint col, MoveList & moveList)
{
  uint index = getIndex(row, col);
  generateMoves(mRookMovesNorth[index], row, col, moveList, true);
  generateMoves(mRookMovesSouth[index], row, col, moveList, true);
  generateMoves(mRookMovesEast[index], row, col, moveList, true);
  generateMoves(mRookMovesWest[index], row, col, moveList, true);
}

void Board8x8::generateWhitePawnMoves(uint row, uint col, MoveList & moveList)
{
  static const PieceType promotedPieceTypes[] =
  { WhiteQueen, WhiteRook, WhiteBishop, WhiteKnight };

  uint sourceIndex = getIndex(row, col);
  std::list<uint> & listMoves = mWhitePawnMoves[sourceIndex];
  for (std::list<uint>::iterator itr = listMoves.begin(); itr != listMoves.end(); ++itr)
  {
    uint destIndex(*itr);
    uint destRow = destIndex >> 3;
    uint destCol = destIndex & 7;

    if (row == 6) {
      if (col == destCol) {
        if (mPieceType[(row+1)*8+col] == NoPiece) {
          for (int i = 0; i < 4; i++) {
            Move promoMove(row, col, destRow, destCol, NoPiece, promotedPieceTypes[i]);
            moveList.addMove(promoMove);
          }
        }
      }
      else {
        PieceType otherPiece = mPieceType[destIndex];
        if (isPiece(otherPiece) && isBlackPiece(otherPiece)) {
          for (int i = 0; i < 4; i++) {
            Move promoMove(row, col, destRow, destCol, otherPiece, promotedPieceTypes[i]);
            moveList.addMove(promoMove);
          }
        }
      }
    }
    else {
      if (col == destCol) {
        bool doublePush = false;
        bool singlePush = mPieceType[(row+1)*8+col] == NoPiece;
        if (singlePush)
          doublePush = mPieceType[(row+2)*8+col] == NoPiece;

        if ( (destRow-row) == 1 && singlePush) {
          Move newMove(row, col, destRow, destCol);
          moveList.addMove(newMove);
        }

        if ( (destRow-row) == 2 && doublePush) {
          Move newMove(row, col, destRow, destCol);
          newMove.setDoublePawnPush();
          moveList.addMove(newMove);
        }
      }
      else {
        PieceType otherPiece = mPieceType[destIndex];
        if (isPiece(otherPiece) && isBlackPiece(otherPiece)) {
          Move newMove(row, col, destRow, destCol, otherPiece);
          moveList.addMove(newMove);
        }
      }
    }
  }

  // Check en-passant
  if (mWhiteEnPassant && row == 4) {
    if (col != 7) {
      PieceType pieceType = getPieceType(row, col+1);
      if (mEnPassantCaptureCol == col + 1 && pieceType == BlackPawn) {
        Move newMove(row, col, row+1, col+1, BlackPawn);
        newMove.setEnPassant();
        moveList.addMove(newMove);
      }
    }

    if (col != 0) {
      PieceType pieceType = getPieceType(row, col-1);
      if (mEnPassantCaptureCol == col - 1 && pieceType == BlackPawn)  {
        Move newMove(row, col, row+1, col-1, BlackPawn);
        newMove.setEnPassant();
        moveList.addMove(newMove);
      }
    }
  }
}

uint Board8x8::getBlackKingRow() const
{
  return mBlackKingRow;
}

uint Board8x8::getBlackKingCol() const
{
  return mBlackKingCol;
}

void Board8x8::getCastlingRights(uint & castlingRights) const
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

uint Board8x8::getIndex(uint row, uint col) const
{
  return (row << 3) + col;
}

void Board8x8::getRowCol(uint index, uint & row, uint & col) const
{
  row = index >> 3;
  col = index & 7;
}

uint Board8x8::getWhiteKingRow() const
{
  return mWhiteKingRow;
}

uint Board8x8::getWhiteKingCol() const
{
  return mWhiteKingCol;
}

PieceType Board8x8::getPieceType(uint row, uint col) const
{
  return mPieceType[getIndex(row,col)];
}

void Board8x8::initBoard()
{
  mWhiteToMove = true;
  mWhiteCastleKingSide = mWhiteCastleQueenSide = mBlackCastleKingSide = mBlackCastleQueenSide = mWhiteEnPassant = mBlackEnPassant = true;
  mWhiteMated = mBlackMated = mStalemate = false;
  //mStalemateCondition = NoStalemate;
  mEnPassantCaptureCol = 8;
  mWhiteKingCol = mBlackKingCol = 4;
  mBlackKingRow = BLACK_KING_START_ROW;
  mWhiteKingRow = WHITE_KING_START_ROW;

  mHalfMoveClock = 0;

  mTotalPieces = 32;
  mTotalWhitePieces = mTotalBlackPieces = 16;
  mTotalWhiteQueens = mTotalBlackQueens = 1;
  mTotalWhiteRooks = mTotalBlackRooks = mTotalWhiteBishops = mTotalBlackBishops = mTotalWhiteKnights = mTotalBlackKnights = 2;
  mTotalWhitePawns = mTotalBlackPawns = 8;

  for (uint i = 0; i < 64; i++)
    mPieceType[i] = NoPiece;

  mPieceType[WHITE_ROW*8 + 0] = mPieceType[WHITE_ROW*8 + 7] = WhiteRook;
  mPieceType[WHITE_ROW*8 + 1] = mPieceType[WHITE_ROW*8 + 6] = WhiteKnight;
  mPieceType[WHITE_ROW*8 + 2] = mPieceType[WHITE_ROW*8 + 5] = WhiteBishop;
  mPieceType[WHITE_ROW*8 + 3] = WhiteQueen;
  mPieceType[WHITE_ROW*8 + 4] = WhiteKing;

  mPieceType[BLACK_ROW*8 + 0] = mPieceType[BLACK_ROW*8 + 7] = BlackRook;
  mPieceType[BLACK_ROW*8 + 1] = mPieceType[BLACK_ROW*8 + 6] = BlackKnight;
  mPieceType[BLACK_ROW*8 + 2] = mPieceType[BLACK_ROW*8 + 5] = BlackBishop;
  mPieceType[BLACK_ROW*8 + 3] = BlackQueen;
  mPieceType[BLACK_ROW*8 + 4] = BlackKing;

  for (uint col = 0; col < 8; col++) {
    mPieceType[WHITE_PAWN_START_ROW*8 +col] = WhitePawn;
    mPieceType[BLACK_PAWN_START_ROW*8 +col] = BlackPawn;
  }
}

void Board8x8::initMoves()
{
  static const int knightRowIncr [] = {2, 1, -1, -2,  -2,  -1,  1,  2};
  static const int knightColIncr [] = {1, 2,  2,  1,  -1,  -2, -2, -1};

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      uint index = getIndex(static_cast<uint>(row), static_cast<uint>(col));
      int destRow, destCol;

      // Rank and file moves
      destCol = col + 1;
      while (destCol <= 7) {
        uint moveIndex = getIndex((u_int)row, (u_int)destCol);
        mRookMovesEast[index].push_back(moveIndex);

        if ( (destCol - col) == 1)
          mKingMoves[index].push_back(moveIndex);

        destCol++;
      }

      destCol = col - 1;
      while (destCol >= 0) {
        uint moveIndex = getIndex((uint)row, (uint)destCol);
        mRookMovesWest[index].push_back(moveIndex);

        if ( (col - destCol) == 1)
          mKingMoves[index].push_back(moveIndex);

        destCol--;
      }

      destRow = row + 1;
      while (destRow < 8) {
        uint moveIndex = getIndex((uint)destRow, (uint)col);
        mRookMovesNorth[index].push_back(moveIndex);

        if ( (destRow - row) == 1) {
          mKingMoves[index].push_back(moveIndex);
          mWhitePawnMoves[index].push_back(moveIndex);
        }

        if (row == WHITE_PAWN_START_ROW && (destRow - row) == 2)
          mWhitePawnMoves[index].push_back(moveIndex);

        destRow++;
      }

      destRow = row - 1;
      while (destRow >= 0) {
        uint moveIndex = getIndex((uint)destRow, (uint)col);
        mRookMovesSouth[index].push_back(moveIndex);

        if ( (row - destRow) == 1) {
          mKingMoves[index].push_back(moveIndex);
          mBlackPawnMoves[index].push_back(moveIndex);
        }

        if (row == BLACK_PAWN_START_ROW && (row-destRow) == 2)
          mBlackPawnMoves[index].push_back(moveIndex);

        destRow--;
      }

      // Diagonal moves
      destCol = col + 1;
      destRow = row + 1;
      while (destCol <= 7 && destRow <= 7) {
        uint moveIndex = getIndex((uint)destRow, (uint)destCol);
        mBishopMovesNorthEast[index].push_back(moveIndex);

        if ( (destRow - row) == 1 && (destCol - col) == 1) {
          mKingMoves[index].push_back(moveIndex);
          mWhitePawnMoves[index].push_back(moveIndex);
        }

        destRow++;
        destCol++;
      }

      destCol = col - 1;
      destRow = row - 1;
      while (destCol >= 0 && destRow >= 0) {
        uint moveIndex = getIndex((uint)destRow, (uint)destCol);
        mBishopMovesSouthWest[index].push_back(moveIndex);

        if ( (row - destRow) == 1 && (col - destCol) == 1) {
          mKingMoves[index].push_back(moveIndex);
          mBlackPawnMoves[index].push_back(moveIndex);
        }

        destRow--;
        destCol--;
      }

      destRow = row + 1;
      destCol = col - 1;
      while (destRow <= 7 && destCol >= 0) {
        uint moveIndex = getIndex((uint)destRow, (uint)destCol);
        mBishopMovesNorthWest[index].push_back(moveIndex);

        if ( (destRow - row) == 1 && (col - destCol) == 1)
        {
          mKingMoves[index].push_back(moveIndex);
          mWhitePawnMoves[index].push_back(moveIndex);
        }

        destRow++;
        destCol--;
      }

      destRow = row - 1;
      destCol = col + 1;
      while (destRow >= 0 && destCol <= 7) {
        uint moveIndex = getIndex((uint)destRow, (uint)destCol);
        mBishopMovesSouthEast[index].push_back(moveIndex);

        if ( (row - destRow) == 1 && (destCol - col) == 1) {
          mKingMoves[index].push_back(moveIndex);
          mBlackPawnMoves[index].push_back(moveIndex);
        }

        destRow--;
        destCol++;
      }

      // Knight moves
      for (int i = 0; i < 8; i++) {
        int destRow = row + knightRowIncr[i];
        int destCol = col + knightColIncr[i];
        if (destRow >= 0 && destRow <= 7 && destCol >= 0 && destCol <= 7) {
          uint moveIndex = getIndex((uint)destRow, (uint)destCol);
          mKnightMoves[index].push_back(moveIndex);
        }
      }
    }
  }
}

bool Board8x8::isBlackChecked() const
{
  return mBlackChecked;
}

bool Board8x8::isBlackMated() const
{
  return mBlackMated;
}

bool Board8x8::isCellAttacked(uint row, uint col, bool whitePiece) const
{
  PieceType pieceType = getPieceType(row, col);

  int pawnRowOffset = whitePiece ? 1 : -1;
  u_int kingRow = (whitePiece) ? mBlackKingRow : mWhiteKingRow;
  u_int kingCol = (whitePiece) ? mBlackKingCol : mWhiteKingCol;
  //PieceType kingPieceType =(whitePiece) ?  BlackKing : WhiteKing;
  PieceType pawnPieceType = (whitePiece) ? BlackPawn : WhitePawn;
  PieceType bishopPieceType = (whitePiece) ? BlackBishop : WhiteBishop;
  PieceType queenPieceType = (whitePiece) ? BlackQueen : WhiteQueen;
  PieceType rookPieceType = (whitePiece) ? BlackRook : WhiteRook;
  PieceType knightPieceType = (whitePiece) ? BlackKnight : WhiteKnight;

  int destRow = 0, destCol = 0;

  // Up along file
  destRow = row; destCol = col;
  while (++destRow < 8){
    PieceType otherPiece = getPieceType(destRow, destCol);
    if (otherPiece == NoPiece) continue;
    if (isFriendlyPiece(pieceType, otherPiece)) break;
    if (otherPiece == queenPieceType || otherPiece == rookPieceType) return true;
    break;
  }

  // Down along file
  destRow = row; destCol = col;
  while (--destRow >= 0) {
    PieceType otherPiece = getPieceType(destRow, destCol);
    if (otherPiece == NoPiece) continue;
    if (isFriendlyPiece(pieceType, otherPiece)) break;
    if (otherPiece == queenPieceType || otherPiece == rookPieceType) return true;
    break;
  }

  // Forward along rank
  destRow = row; destCol = col;
  while (++destCol < 8) {
    PieceType otherPiece = getPieceType(destRow, destCol);
    if (otherPiece == NoPiece) continue;
    if (isFriendlyPiece(pieceType, otherPiece)) break;
    if (otherPiece == queenPieceType || otherPiece == rookPieceType) return true;
    break;
  }

  // Backward along rank
  destRow = row; destCol = col;
  while (--destCol >= 0) {
    PieceType otherPiece = getPieceType(destRow, destCol);
    if (otherPiece == NoPiece) continue;
    if (isFriendlyPiece(pieceType, otherPiece)) break;
    if (otherPiece == queenPieceType || otherPiece == rookPieceType) return true;
    break;
  }

  // Forward right along diagonal
  destRow = row; destCol = col;
  while (++destRow < 8 && ++destCol < 8) {
    PieceType otherPiece = getPieceType(destRow, destCol);
    if (otherPiece == NoPiece) continue;
    if (isFriendlyPiece(pieceType, otherPiece)) break;
    if (otherPiece == queenPieceType || otherPiece == bishopPieceType) return true;
    break;
  }

  // Forward left along diagonal
  destRow = row; destCol = col;
  while (++destRow < 8 && --destCol >= 0) {
    PieceType otherPiece = getPieceType(destRow, destCol);
    if (otherPiece == NoPiece) continue;
    if (isFriendlyPiece(pieceType, otherPiece)) break;
    if (otherPiece == queenPieceType || otherPiece == bishopPieceType) return true;
    break;
  }

  // Backward left along diagonal
  destRow = row; destCol = col;
  while (--destRow >= 0 && --destCol >= 0) {
    PieceType otherPiece = getPieceType(destRow, destCol);
    if (otherPiece == NoPiece) continue;
    if (isFriendlyPiece(pieceType, otherPiece)) break;
    if (otherPiece == queenPieceType || otherPiece == bishopPieceType) return true;
    break;
  }

  // Backward right along diagonal
  destRow = row; destCol = col;
  while (--destRow >= 0 && ++destCol < 8) {
    PieceType otherPiece = getPieceType(destRow, destCol);
    if (otherPiece == NoPiece) continue;
    if (isFriendlyPiece(pieceType, otherPiece)) break;
    if (otherPiece == queenPieceType || otherPiece == bishopPieceType) return true;
    break;
  }

  // Knight attack
  static const int rowIncr [] =   {-1, -2, -2, -1,  1,  2,  2,  1};
  static const int colIncr [] =   {-2, -1,  1,  2,  2,  1, -1, -2};
  for (uint i = 0; i < 8; i++) {
    int destRow = row + rowIncr[i];
    int destCol = col + colIncr[i];
    if (destRow >= 0 && destCol >= 0 && destRow < 8 && destCol < 8)
    {
      PieceType otherPieceType = getPieceType(destRow, destCol);
      if (otherPieceType == knightPieceType)
        return true;
    }
  }

  // Pawn attack
  if (row >= 0 && row < 8) {
    destRow = row + pawnRowOffset;
    if (col == 0) {
      if (getPieceType(destRow,1) == pawnPieceType)
        return true;
    }
    else if (col == 7) {
      if (getPieceType(destRow,6) == pawnPieceType)
        return true;
    }
    else {
      if (getPieceType(destRow,col+1) == pawnPieceType || getPieceType(destRow,col-1) == pawnPieceType)
        return true;
    }
  }

  // King attack
  if (abs((int)(row-kingRow)) <= 1 && abs((int)(col-kingCol)) <= 1)
    return true;

  return false;
}

bool Board8x8::isLegal() const
{
  if (!isWhiteToMove() && isCellAttacked(mWhiteKingRow, mWhiteKingCol, true))
    return false;

  if (isWhiteToMove() && isCellAttacked(mBlackKingRow, mBlackKingCol, false))
    return false;

  return true;
}

bool Board8x8::isWhiteChecked() const
{
  return mWhiteChecked;
}

bool Board8x8::isWhiteMated() const
{
  return mWhiteMated;
}

bool Board8x8::isWhiteToMove() const
{
  return mWhiteToMove;
}

void Board8x8::loadState()
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

bool Board8x8::makeMove(Move & newMove)
{
  saveState();

  // Get some information about the move
  uint sourceRow = newMove.sourceRow();
  uint sourceCol = newMove.sourceColumn();
  uint destRow = newMove.destinationRow();
  uint destCol = newMove.destinationColumn();
  uint sourceIndex = getIndex(sourceRow, sourceCol);
  uint destIndex = getIndex(destRow, destCol);

  // Store the source and destination piece types
  PieceType pieceType = mPieceType[sourceIndex];
  //PieceType destPiece = mPieceType[destIndex];

  // Perform a standard move
  mPieceType[sourceIndex] = NoPiece;
  mPieceType[destIndex] = pieceType;

  // Assume we won't have an en-passant move available
  mWhiteEnPassant = mBlackEnPassant = false;
  mEnPassantCaptureCol = 8;

  // Update the half move clock
  mHalfMoveClock++;

  switch (pieceType) {
  case WhitePawn:
    mHalfMoveClock = 0;
    if (newMove.isDoublePawnPush()) {
      if (destCol != 0) {
        PieceType otherPiece = mPieceType[BLACK_ENPASSANT_CHECK_ROW*8 + destCol-1];
        if (otherPiece == BlackPawn)
        {
          mBlackEnPassant = true;
          mEnPassantCaptureCol = destCol;
        }
      }

      if (destCol != 7) {
        PieceType otherPiece = mPieceType[BLACK_ENPASSANT_CHECK_ROW*8 + destCol+1];
        if (otherPiece == BlackPawn) {
          mBlackEnPassant = true;
          mEnPassantCaptureCol = destCol;
        }
      }
    }
    if (newMove.isPromotion()) {
      PieceType promotedPiece = newMove.promotedPiece();
      mPieceType[destIndex] = promotedPiece;

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
    if (sourceIndex == 0) mWhiteCastleQueenSide = false;
    if (sourceIndex == 7) mWhiteCastleKingSide = false;
    break;
  case WhiteQueen:
    break;
  case WhiteKing:
    mWhiteKingRow = destRow;
    mWhiteKingCol = destCol;
    mWhiteCastleKingSide = mWhiteCastleQueenSide = false;
    if (newMove.isKingCastle()) {
      mPieceType[getIndex(sourceRow, 7)] = NoPiece;
      mPieceType[getIndex(sourceRow, KING_CASTLE_ROOK_COL)] = WhiteRook;
    }
    if (newMove.isQueenCastle()) {
      mPieceType[getIndex(sourceRow, 0)] = NoPiece;
      mPieceType[getIndex(sourceRow, QUEEN_CASTLE_ROOK_COL)] = WhiteRook;
    }
    break;
  case BlackPawn:
    mHalfMoveClock = 0;
    if (newMove.isDoublePawnPush()) {
      if (destCol != 0) {
        PieceType otherPiece = mPieceType[WHITE_ENPASSANT_CHECK_ROW*8 + destCol-1];
        if (otherPiece == WhitePawn) {
          mWhiteEnPassant = true;
          mEnPassantCaptureCol = destCol;
        }
      }

      if (destCol != 7) {
        PieceType otherPiece = mPieceType[WHITE_ENPASSANT_CHECK_ROW*8 + destCol+1];
        if (otherPiece == WhitePawn) {
          mWhiteEnPassant = true;
          mEnPassantCaptureCol = destCol;
        }
      }
    }
    if (newMove.isPromotion()) {
      PieceType promotedPiece = newMove.promotedPiece();
      mPieceType[destIndex] = promotedPiece;

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
    if (sourceIndex == 56) mBlackCastleQueenSide = false;
    if (sourceIndex == 63) mBlackCastleKingSide = false;
    break;
  case BlackQueen:
    break;
  case BlackKing:
    mBlackKingRow = destRow;
    mBlackKingCol = destCol;
    mBlackCastleKingSide = mBlackCastleQueenSide = false;
    if (newMove.isKingCastle()) {
      mPieceType[sourceRow*8 + 7] = NoPiece;
      mPieceType[sourceRow*8 + KING_CASTLE_ROOK_COL] = BlackRook;
    }
    if (newMove.isQueenCastle()) {
      mPieceType[sourceRow*8] = NoPiece;
      mPieceType[sourceRow*8 + QUEEN_CASTLE_ROOK_COL] = BlackRook;
    }
    break;
  default:
    break;
  }

  // Perform a capture move (en-passant moves are handled later)
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
      if (destIndex == 0) mWhiteCastleQueenSide = false;
      if (destIndex == 7) mWhiteCastleKingSide = false;
      break;
    case WhiteQueen:
      mTotalWhiteQueens--;
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
      if (destIndex == 63) mBlackCastleKingSide = false;
      if (destIndex == 56) mBlackCastleQueenSide = false;
      break;
    case BlackQueen:
      mTotalBlackQueens--;
      break;
    default:
      break;
    }
  }

  // Handle en-passant capture
  if (newMove.isEnPassant()) {
    mHalfMoveClock = 0;

    PieceType capturePiece = getPieceType(sourceRow, destCol);
    mPieceType[getIndex(sourceRow, destCol)] = NoPiece;
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

  if (!mWhiteToMove)
    mWhiteChecked = isCellAttacked(mWhiteKingRow, mWhiteKingCol, true);
  else
    mBlackChecked = isCellAttacked(mBlackKingRow, mBlackKingCol, false);

  mWhiteToMove = !mWhiteToMove;

  return true;
}

void Board8x8::saveState()
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

void Board8x8::setPosition(const std::string &fenString)
{
  Q_UNUSED(fenString);
}

void Board8x8::trimCandidateMoves(MoveList & candidateMoves, MoveList & legalMoves)
{
  for (uint i = 0; i < candidateMoves.totalMoves(); ++i) {
    Move currentMove(candidateMoves[i]);
    makeMove(currentMove);
    if (isLegal())
      legalMoves.addMove(currentMove);
    unmakeMove(currentMove);
  }
}

bool Board8x8::unmakeMove(Move & oldMove)
{
  // Get some information about the move
  uint sourceRow = oldMove.sourceRow();
  uint sourceCol = oldMove.sourceColumn();
  uint destRow = oldMove.destinationRow();
  uint destCol = oldMove.destinationColumn();
  uint sourceIndex = getIndex(sourceRow, sourceCol);
  uint destIndex = getIndex(destRow, destCol);

  // Store the source and destination piece types
  PieceType pieceType = getPieceType(destRow, destCol);

  // Undo standard move
  mPieceType[destIndex] = NoPiece;
  mPieceType[sourceIndex] = pieceType;

  switch (pieceType)
  {
  case WhitePawn:
    break;
  case WhiteKnight:
    if (oldMove.isPromotion()) {
      mPieceType[sourceIndex] = WhitePawn;
      mTotalWhiteKnights--;
      mTotalWhitePawns++;
    }
    break;
  case WhiteBishop:
    if (oldMove.isPromotion()) {
      mPieceType[sourceIndex] = WhitePawn;
      mTotalWhiteBishops--;
      mTotalWhitePawns++;
    }
    break;
  case WhiteRook:
    if (oldMove.isPromotion()) {
      mPieceType[sourceIndex] = WhitePawn;
      mTotalWhiteRooks--;
      mTotalWhitePawns++;
    }
    break;
  case WhiteQueen:
    if (oldMove.isPromotion()) {
      mPieceType[sourceIndex] = WhitePawn;
      mTotalWhiteQueens--;
      mTotalWhitePawns++;
    }
    break;
  case WhiteKing:
    mWhiteKingRow = sourceRow;
    mWhiteKingCol = sourceCol;
    if (oldMove.isKingCastle()) {
      mPieceType[getIndex(sourceRow, 7)] = WhiteRook;
      mPieceType[getIndex(sourceRow, KING_CASTLE_ROOK_COL)] = NoPiece;
    }
    if (oldMove.isQueenCastle()) {
      mPieceType[getIndex(sourceRow, 0)] = WhiteRook;
      mPieceType[getIndex(sourceRow, QUEEN_CASTLE_ROOK_COL)] = NoPiece;
    }
    break;
  case BlackPawn:
    break;
  case BlackKnight:
    if (oldMove.isPromotion()) {
      mPieceType[sourceIndex] = BlackPawn;
      mTotalBlackKnights--;
      mTotalBlackPawns++;
    }
    break;
  case BlackBishop:
    if (oldMove.isPromotion()) {
      mPieceType[sourceIndex] = BlackPawn;
      mTotalBlackBishops--;
      mTotalBlackPawns++;
    }
    break;
  case BlackRook:
    if (oldMove.isPromotion()) {
      mPieceType[sourceIndex] = BlackPawn;
      mTotalBlackRooks--;
      mTotalBlackPawns++;
    }
    break;
  case BlackQueen:
    if (oldMove.isPromotion()) {
      mPieceType[sourceIndex] = BlackPawn;
      mTotalBlackQueens--;
      mTotalBlackPawns++;
    }
    break;
  case BlackKing:
    mBlackKingRow = sourceRow;
    mBlackKingCol = sourceCol;
    if (oldMove.isKingCastle()) {
      mPieceType[getIndex(sourceRow, 7)] = BlackRook;
      mPieceType[getIndex(sourceRow, KING_CASTLE_ROOK_COL)] = NoPiece;
    }
    if (oldMove.isQueenCastle()) {
      mPieceType[getIndex(sourceRow, 0)] = BlackRook;
      mPieceType[getIndex(sourceRow, QUEEN_CASTLE_ROOK_COL)] = NoPiece;
    }
    break;
  default:
    break;
  }

  if (oldMove.isCapture() && !oldMove.isEnPassant()) {
    PieceType capturePiece = oldMove.capturedPiece();
    mPieceType[destIndex] = capturePiece;

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
    default:
      break;
    }
  }

  if (oldMove.isEnPassant()) {
    PieceType capturePiece = oldMove.capturedPiece();
    mPieceType[getIndex(sourceRow, destCol)] = capturePiece;

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

  // Load back in the previous state for castling and en-passant abilites
  // as well as the half move clock
  loadState();

  return true;
}
