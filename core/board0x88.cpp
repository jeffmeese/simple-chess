#include "board0x88.h"
#include "move.h"
#include "movelist.h"
#include "fenparser.h"

#include <iostream>

static const int NORTH = 16;
static const int SOUTH = -16;
static const int EAST = 1;
static const int WEST = -1;
static const int NW = 15;
static const int NE = 17;
static const int SW = -17;
static const int SE = -15;

static const int sliders[5] = {0, 1, 1, 1, 0};
static const int numDirections[5] = {8, 8, 4, 4, 8};
static const int dirVectors[5][8] =
{
  { SW, SOUTH, SE, WEST, EAST, NW, NORTH, NE },
  { SW, SOUTH, SE, WEST, EAST, NW, NORTH, NE },
  { SOUTH, WEST, EAST, NORTH                 },
  { SW, SE, NW, NE                           },
  { -33, -31, -18, -14, 14, 18, 31, 33       }
};

enum Square
{
  A1=0  , B1, C1, D1, E1, F1, G1, H1,
  A2=16 , B2, C2, D2, E2, F2, G2, H2,
  A3=32 , B3, C3, D3, E3, F3, G3, H3,
  A4=48 , B4, C4, D4, E4, F4, G4, H4,
  A5=64 , B5, C5, D5, E5, F5, G5, H5,
  A6=80 , B6, C6, D6, E6, F6, G6, H6,
  A7=96 , B7, C7, D7, E7, F7, G7, H7,
  A8=112, B8, C8, D8, E8, F8, G8, H8
};


Board0x88::Board0x88(void)
{
  initBoard();
  initPieceMap();
}

Board0x88::~Board0x88(void)
{
}

bool Board0x88::canBlackCastleQueenSide() const
{
  return (mCastlingRights & CastleBlackQueen);
}

bool Board0x88::canBlackCastleKingSide() const
{
  return (mCastlingRights & CastleBlackKing);
}

bool Board0x88::canWhiteCastleQueenSide() const
{
  return (mCastlingRights & CastleWhiteQueen);
}

bool Board0x88::canWhiteCastleKingSide() const
{
  return (mCastlingRights & CastleWhiteKing);
}

uint Board0x88::generateLegalMoves(MoveList & moveList)
{
  //MoveList candidateMoves;

  //Color attackingColor = isWhiteToMove() ? ColorBlack : ColorWhite;
  //    // Don't check castling moves for legality si
  //    // they won't be generated unless they are legal
  //    if (mPieces[index] == King) {
  //      if (isWhiteToMove()) {
  //        if (canWhiteCastleKingSide()) {
  //          if (mPieces[F1] == Empty &&
  //              mPieces[G1] == Empty &&
  //              !isCellAttacked(E1, attackingColor) &&
  //              !isCellAttacked(F1, attackingColor) &&
  //              !isCellAttacked(G1, attackingColor))
  //          {
  //            Move newMove(getRow(index), getCol(index), getRow(G1), getCol(G1));
  //            newMove.setKingCastle();
  //            moveList.addMove(newMove);
  //          }
  //        }
  //        if (canWhiteCastleQueenSide()) {
  //          if (mPieces[D1] == Empty &&
  //              mPieces[C1] == Empty &&
  //              mPieces[B1] == Empty &&
  //              !isCellAttacked(E1, attackingColor) &&
  //              !isCellAttacked(D1, attackingColor) &&
  //              !isCellAttacked(C1, attackingColor))
  //          {
  //            Move newMove(getRow(index), getCol(index), getRow(C1), getCol(C1));
  //            newMove.setQueenCastle();
  //            moveList.addMove(newMove);
  //          }
  //        }
  //      }
  //      else {
  //        if (canBlackCastleKingSide()) {
  //          if (mPieces[F8] == Empty &&
  //              mPieces[G8] == Empty &&
  //              !isCellAttacked(E8, attackingColor) &&
  //              !isCellAttacked(F8, attackingColor) &&
  //              !isCellAttacked(G8, attackingColor))
  //          {
  //            Move newMove(getRow(index), getCol(index), getRow(G8), getCol(G8));
  //            newMove.setKingCastle();
  //            moveList.addMove(newMove);
  //          }
  //        }
  //        if (canBlackCastleQueenSide()) {
  //          if (mPieces[D8] == Empty &&
  //              mPieces[C8] == Empty &&
  //              mPieces[B8] == Empty &&
  //              !isCellAttacked(E8, attackingColor) &&
  //              !isCellAttacked(D8, attackingColor) &&
  //              !isCellAttacked(C8, attackingColor))
  //          {
  //            Move newMove(getRow(index), getCol(index), getRow(C8), getCol(C8));
  //            newMove.setQueenCastle();
  //            moveList.addMove(newMove);
  //          }
  //        }
  //      }
  //    }

  for (uint index = 0; index < 120; index++) {
    if (mColors[index] == mSideToMove) {
      if (mPieces[index] == Pawn) {
        generatePawnMoves(index, moveList);
        generatePawnCaptures(index, moveList);
      }
      else {
        Piece pieceType = mPieces[index];
        for (uint num = 0; num < numDirections[pieceType]; num++) {

          for (uint pos = index;;) {
            pos = pos + dirVectors[pieceType][num];

            if (!isValidSquare(pos))
              break;

            if (mColors[pos] == ColorEmpty) {
              // Add standard move
              Move newMove(getRow(index), getCol(index), getRow(pos), getCol(pos));
              moveList.addMove(newMove);
            }
            else if (mColors[pos] != mSideToMove) {
              // Add capture move
              Move newMove(getRow(index), getCol(index), getRow(pos), getCol(pos), mPieceTypes[pos]);
              moveList.addMove(newMove);
              break;
            }
            else {
              break;
            }

            // Break on non-sliding pieces (King, Knight)
            if (!sliders[pieceType])
              break;
          }
        }
      }
    }
  }

  // Determine the legal moves from the candidate moves
  uint kingIndex = (mSideToMove == ColorWhite) ? mWhiteKingIndex : mBlackKingIndex;
  for (uint i = 0; i < moveList.totalMoves(); i++) {
    makeMove(moveList[i]);
    bool legalMove = !isCellAttacked(kingIndex, static_cast<Color>(mSideToMove));
    unmakeMove(moveList[i]);
    if (!legalMove)
      moveList.deleteMove(i);
  }

  return moveList.totalMoves();
}

uint Board0x88::generateLegalMoves(uint row, uint col, MoveList & moveList)
{
  // This function is generally called as a one off or from a GUI
  // so speed is not a major concern here
  MoveList candidateMoves;
  generateLegalMoves(candidateMoves);

  for (uint i = 0; i < candidateMoves.totalMoves(); i++) {
    Move candidateMove = candidateMoves[i];
    if (candidateMove.sourceRow() == row && candidateMove.sourceColumn() == col)
      moveList.addMove(candidateMove);
  }
  return moveList.totalMoves();
}

void Board0x88::generatePawnCaptures(uint index, MoveList &moveList)
{
  if (mSideToMove == ColorWhite) {
    static const int dirs[] = {NE, NW};
    for (uint i = 0; i < 2; i++) {
      uint pos = index + dirs[i];
      if (isValidSquare(pos) && (mColors[pos] == ColorBlack || pos == mEnPassantIndex)) {
        PieceType capturePiece = mBlackPieceMap[mPieces[pos]];
        Move newMove(getRow(index), getCol(index), getRow(pos), getCol(pos), capturePiece);
        if (pos == mEnPassantIndex)
          newMove.setEnPassant();

        moveList.addMove(newMove);
      }
    }
  }
  else {
    static const int dirs[] = {SE, SW};
    for (uint i = 0; i < 2; i++) {
      uint pos = index + dirs[i];
      if (isValidSquare(pos) && (mColors[pos] == ColorWhite || pos == mEnPassantIndex)) {
        PieceType capturePiece = mWhitePieceMap[mPieces[pos]];
        Move newMove(getRow(index), getCol(index), getRow(pos), getCol(pos), capturePiece);
        if (pos == mEnPassantIndex)
          newMove.setEnPassant();

        moveList.addMove(newMove);
      }
    }
  }
}

void Board0x88::generatePawnMoves(uint index, MoveList &moveList)
{
  static const PieceType whitePromo[] = {WhiteQueen, WhiteRook, WhiteBishop, WhiteKnight};
  static const PieceType blackPromo[] = {BlackQueen, BlackRook, BlackBishop, BlackKnight};

  if (mSideToMove == ColorWhite) {
    uint pos = index + NORTH;
    if (mPieces[pos] == Empty) {

      // Generate standard move
      Move newMove(getRow(index), getCol(index), getRow(pos), getCol(pos));
      if (getRow(pos) == 7) {
        for (uint i = 0; i < 4; i++) {
          newMove.setPromotedPiece(whitePromo[i]);
          moveList.addMove(newMove);
        }
      }
      else {
        moveList.addMove(newMove);
      }

      // Generate down pawn push
      pos = index + 2*NORTH;
      if (getRow(index) == 1 && mPieces[pos] == Empty) {
        Move newMove(getRow(index), getCol(index), getRow(pos), getCol(pos));
        newMove.setDoublePawnPush();
        moveList.addMove(newMove);
      }
    }
  }
  else {
    uint pos = index + SOUTH;
    if (mPieces[pos] == Empty) {
      Move newMove(getRow(index), getCol(index), getRow(pos), getCol(pos));
      if (getRow(pos) == 0) {
        for (uint i = 0; i < 4; i++) {
          newMove.setPromotedPiece(blackPromo[i]);
          moveList.addMove(newMove);
        }
      }
      else {
        moveList.addMove(newMove);
      }

      // Generate down pawn push
      pos = index + 2*SOUTH;
      if (getRow(index) == 6 && mPieces[pos] == Empty) {
        Move newMove(getRow(index), getCol(index), getRow(pos), getCol(pos));
        newMove.setDoublePawnPush();
        moveList.addMove(newMove);
      }
    }
  }
}

inline uint Board0x88::getCol(uint index) const
{
  return (index & 7);
}

inline uint Board0x88::getIndex(uint row, uint col) const
{
  return (row * 16 + col);
}

PieceType Board0x88::getPieceType(uint row, uint col) const
{
  return mPieceTypes[getIndex(row, col)];
}

inline uint Board0x88::getRow(uint index) const
{
  return (index >> 4);
}

void Board0x88::initBoard()
{
  static const Piece pieces[] =
  { Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook };

  static const PieceType whitePieces[] =
  { WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook };

  static const PieceType blackPieces[] =
  { BlackRook,  BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook };

  mWhiteChecked = false;
  mBlackChecked = false;
  mEnPassantIndex = 0;
  mHalfMoveClock = 0;
  mBlackKingIndex = E8;
  mWhiteKingIndex = E1;
  mCastlingRights = CastleWhiteKing | CastleWhiteQueen | CastleBlackKing | CastleBlackQueen;
  mSideToMove = ColorWhite;

  for (uint index = 0; index < 128; index++) {
    mPieceTypes[index] = NoPiece;
    mPieces[index] = Empty;
    mColors[index] = ColorEmpty;
  }

  for (uint col = 0; col < 8; col++) {
    mColors[getIndex(0, col)] = ColorWhite;
    mColors[getIndex(1, col)] = ColorWhite;
    mColors[getIndex(6, col)] = ColorBlack;
    mColors[getIndex(7, col)] = ColorBlack;
    mPieceTypes[getIndex(1, col)] = WhitePawn;
    mPieceTypes[getIndex(6, col)] = BlackPawn;
    mPieceTypes[getIndex(0, col)] = whitePieces[col];
    mPieceTypes[getIndex(7, col)] = blackPieces[col];
    mPieces[getIndex(1, col)] = Pawn;
    mPieces[getIndex(6, col)] = Pawn;
    mPieces[getIndex(0, col)] = pieces[col];
    mPieces[getIndex(7, col)] = pieces[col];
  }
}

void Board0x88::initPieceMap()
{
  mWhitePieceMap.resize(6);
  mWhitePieceMap[Pawn] = WhitePawn;
  mWhitePieceMap[Rook] = WhiteRook;
  mWhitePieceMap[Knight] = WhiteKnight;
  mWhitePieceMap[Queen] = WhiteQueen;
  mWhitePieceMap[Bishop] = WhiteBishop;
  mWhitePieceMap[King] = WhiteKing;

  mBlackPieceMap.resize(6);
  mBlackPieceMap[Pawn] = BlackPawn;
  mBlackPieceMap[Rook] = BlackRook;
  mBlackPieceMap[Knight] = BlackKnight;
  mBlackPieceMap[Queen] = BlackQueen;
  mBlackPieceMap[Bishop] = BlackBishop;
  mBlackPieceMap[King] = BlackKing;

  mPieceMap.resize(12);
  mPieceMap[WhitePawn] = Pawn;
  mPieceMap[WhiteKnight] = Knight;
  mPieceMap[WhiteBishop] = Bishop;
  mPieceMap[WhiteRook] = Rook;
  mPieceMap[WhiteQueen] = Queen;
  mPieceMap[WhiteKing] = King;
  mPieceMap[BlackPawn] = Pawn;
  mPieceMap[BlackKnight] = Knight;
  mPieceMap[BlackBishop] = Bishop;
  mPieceMap[BlackRook] = Rook;
  mPieceMap[BlackQueen] = Queen;
  mPieceMap[BlackKing] = King;
}

bool Board0x88::isBlackChecked() const
{
  return mBlackChecked;
}

bool Board0x88::isCellAttacked(uint index, Color colorAttacking) const
{
  // Pawn Attacks
  if (colorAttacking == ColorWhite) {
    static const int dirs[] = {NE, NW};
    for (uint i = 0; i < 2; i++) {
      uint pos = index + dirs[i];
      if (isValidSquare(pos) && mPieceTypes[pos] == WhitePawn)
        return true;
    }
  }
  else {
    static const int dirs[] = {SE, SW};
    for (uint i = 0; i < 2; i++) {
      uint pos = index + dirs[i];
      if (isValidSquare(pos) && mPieceTypes[pos] == BlackPawn)
        return true;
    }
  }

  // Knight and King Attacks
  if (leapAttack(index, colorAttacking, Knight) || leapAttack(index, colorAttacking, King))
    return true;

  // Slider Attacks
  static const int dirs[] = {NORTH, SOUTH, EAST, WEST, NW, NE, SW, SE};
  for (uint i = 0; i < 8; i++)
    if (sliderAttack(index, colorAttacking, dirs[i], (i>3) ) )
      return true;

  return false;
}

bool Board0x88::isWhiteChecked() const
{
  return mWhiteChecked;
}

bool Board0x88::isWhiteToMove() const
{
  return (mSideToMove == ColorWhite);
}

inline bool Board0x88::isValidSquare(uint index) const
{
  return !(index & 0x88);
}

bool Board0x88::leapAttack(uint index, Color attackingColor, Piece pieceType) const
{
  for (uint dir = 0; dir < 8; dir++) {
    uint pos = index + dirVectors[pieceType][dir];
    if (isValidSquare(pos) && mColors[pos] == attackingColor && mPieces[pos] == pieceType)
      return true;
  }
  return false;
}

void Board0x88::loadState()
{
  uint savedState = mSavedStates.back();
  mSavedStates.pop_back();

  mCastlingRights |= (savedState & CastleBlackKing);
  mCastlingRights |= (savedState & CastleBlackQueen);
  mCastlingRights |= (savedState & CastleWhiteKing);
  mCastlingRights |= (savedState & CastleWhiteQueen);
  mEnPassantIndex = ((savedState >> 6) & 0x0007);
  mHalfMoveClock = ((savedState >> 9) & 0x003f);
}

bool Board0x88::makeMove(const Move &newMove)
{
  saveState();

  uint sourceRow = newMove.sourceRow();
  uint sourceCol = newMove.sourceColumn();
  uint destRow = newMove.destinationRow();
  uint destCol = newMove.destinationColumn();
  uint sourceIndex = getIndex(sourceRow, sourceCol);
  uint destIndex = getIndex(destRow, destCol);
  PieceType pieceType = mPieceTypes[sourceIndex];
  PieceType capturePieceType = mPieceTypes[destIndex];
  Piece piece = mPieces[sourceIndex];
  //Piece capturePiece = mPieces[destIndex];

  mPieces[destIndex] = piece;
  mPieces[sourceIndex] = Empty;
  mColors[destIndex] = mSideToMove;
  mColors[sourceIndex] = ColorEmpty;
  mPieceTypes[destIndex] = pieceType;
  mPieceTypes[sourceIndex] = NoPiece;

  if (pieceType == WhiteKing)
    mWhiteKingIndex = destIndex;
  if (pieceType == BlackKing)
    mBlackKingIndex = destIndex;

  mHalfMoveClock++;
  if (mPieces[sourceIndex] == Pawn || newMove.isCapture())
    mHalfMoveClock = 0;

  if (newMove.isPromotion()) {
    mPieceTypes[destIndex] = newMove.promotedPiece();
    mPieces[destIndex] = mPieceMap[newMove.promotedPiece()];
  }

  if (newMove.isCapture()) {
    if (capturePieceType == WhiteRook) {
      if (destIndex == H1)
        mCastlingRights &= ~CastleWhiteKing;
      if (destIndex == A1)
        mCastlingRights &= ~CastleWhiteQueen;
    }
    if (capturePieceType == BlackRook) {
      if (destIndex == H8)
        mCastlingRights &= ~CastleBlackKing;
      if (destIndex == H1)
        mCastlingRights &= ~CastleBlackQueen;
    }
  }

  if (newMove.isDoublePawnPush()) {
    static const int col[] = {-1, 1};
    if (isWhiteToMove() && sourceRow == 1) {
      for (uint i = 0; i < 2; i++) {
        uint epIndex = getIndex(destRow, destCol+col[i]);
        if (isValidSquare(epIndex) && mPieceTypes[epIndex] == BlackPawn) {
          mEnPassantIndex = destIndex - SOUTH;
        }
      }
    }
    if (!isWhiteToMove() && sourceRow == 6) {
      for (uint i = 0; i < 2; i++) {
        uint epIndex = getIndex(destRow, destCol+col[i]);
        if (isValidSquare(epIndex) && mPieceTypes[epIndex] == WhitePawn) {
          mEnPassantIndex = destIndex - NORTH;
        }
      }
    }
  }

  switch (sourceIndex) {
  case A1:
    mCastlingRights &= ~CastleWhiteQueen;
    break;
  case E1:
    mCastlingRights &= ~(CastleWhiteKing|CastleWhiteQueen);
    break;
  case H1:
    mCastlingRights &= ~CastleWhiteKing;
    break;
  case A8:
    mCastlingRights &= ~CastleBlackQueen;
    break;
  case E8:
    mCastlingRights &= ~(CastleBlackKing|CastleBlackQueen);
    break;
  case H8:
    mCastlingRights &= ~CastleBlackKing;
    break;
  }

  if (!isWhiteToMove()) {
    mWhiteChecked = isCellAttacked(mWhiteKingIndex, ColorBlack);
  }
  else {
    mBlackChecked = isCellAttacked(mBlackKingIndex, ColorWhite);
  }

  mSideToMove = (mSideToMove == ColorWhite) ? ColorBlack : ColorWhite;

  return true;
}

void Board0x88::printColors()
{
  std::cout << "\n";
  for (int row = 7; row >= 0; row--) {
    std::cout << " +---+---+---+---+---+---+---+---+\n";
    std::cout << row + 1 << "|";

    for (int col = 0; col < 8; col++) {
      uint index = getIndex(row, col);
      //std::string colorString("   ");
      //if (mColors[index] == ColorWhite)
      //  colorString = " W ";
      //else if (mColors[index] == ColorBlack)
      //  colorString = " B ";
      switch ((uint)mPieces[index]) {
      case Pawn:
        std::cout << " P ";
        break;
      case Rook:
        std::cout << " R ";
        break;
      case Knight:
        std::cout << " N ";
        break;
      case Bishop:
        std::cout << " B ";
        break;
      case Queen:
        std::cout << " Q ";
        break;
      case King:
        std::cout << " K ";
        break;
      default:
        std::cout << "   ";
        break;
      }
      std::cout << "|";
    }
    std::cout << "\n";
  }
  std::cout << " +---+---+---+---+---+---+---+---+\n";
  std::cout << "   A   B   C   D   E   F   G   H  \n";
  std::cout << "\n";
}

void Board0x88::saveState()
{
  uint savedState = 0;

  savedState |= (CastleBlackKing | CastleBlackQueen | CastleWhiteKing | CastleWhiteQueen);
  savedState |= ( (mEnPassantIndex & 0x0007) << 6);
  savedState |= ( (mHalfMoveClock & 0x003f) << 9);

  mSavedStates.push_back(savedState);
}

void Board0x88::setPosition(const std::string & fenString)
{
  initBoard();

  FenData fenData;
  if (!readFenString(fenString, fenData))
    return;
}

bool Board0x88::sliderAttack(uint index, Color attackingColor, int attackDirection, bool diag) const
{
  Piece attackPiece = (diag) ? Bishop : Rook;
  uint pos = index + attackDirection;
  while (isValidSquare(pos)) {
    if (mColors[pos] != ColorEmpty) {
      Piece pieceType = mPieces[pos];
      if (mColors[pos] == attackingColor && (pieceType == Queen || pieceType == Bishop) )
        return true;

      return false;
    }
    pos += attackDirection;
  }
  return false;
}

bool Board0x88::unmakeMove(const Move &undoMove)
{
  uint sourceRow = undoMove.sourceRow();
  uint sourceCol = undoMove.sourceColumn();
  uint destRow = undoMove.destinationRow();
  uint destCol = undoMove.destinationColumn();
  uint sourceIndex = getIndex(sourceRow, sourceCol);
  uint destIndex = getIndex(destRow, destCol);
  PieceType pieceType = mPieceTypes[destIndex];
  //PieceType capturePieceType = mPieceTypes[destIndex];
  Piece piece = mPieces[destIndex];
  //Piece capturePiece = mPieces[destIndex];

  mPieces[sourceIndex] = piece;
  mPieces[destIndex] = Empty;
  mColors[sourceIndex] = !mSideToMove;
  mColors[destIndex] = ColorEmpty;
  mPieceTypes[sourceIndex] = pieceType;
  mPieceTypes[destIndex] = NoPiece;

  if (pieceType == WhiteKing)
    mWhiteKingIndex = sourceIndex;
  if (pieceType == BlackKing)
    mBlackKingIndex = sourceIndex;

  if (undoMove.isCapture() && !undoMove.isEnPassant()) {
    mPieceTypes[destIndex] = undoMove.capturedPiece();
    mPieces[destIndex] = mPieceMap[undoMove.capturedPiece()];
    mColors[destIndex] = mSideToMove;
  }

  if (undoMove.isPromotion()) {
    mPieces[sourceIndex] = Pawn;
    mPieceTypes[sourceIndex] = isWhiteToMove() ? BlackPawn : WhitePawn;
  }

  if (undoMove.isKingCastle() || undoMove.isQueenCastle()) {
    if (destIndex == G1) {
      mPieces[H1] = Rook;
      mPieceTypes[H1] = WhiteRook;
      mColors[H1] = ColorWhite;
    }
    if (destIndex == C1) {
      mPieces[A1] = Rook;
      mPieceTypes[A1] = WhiteRook;
      mColors[A1] = ColorWhite;
    }
    if (destIndex == G8) {
      mPieces[H8] = Rook;
      mPieceTypes[H8] = BlackRook;
      mColors[H8] = ColorBlack;
    }
    if (destIndex == C8) {
      mPieces[A8] = Rook;
      mPieceTypes[A8] = BlackRook;
      mColors[A8] = ColorBlack;
    }
  }

  if (undoMove.isEnPassant()) {

  }

  if (!isWhiteToMove()) {
    mBlackChecked = isCellAttacked(mBlackKingIndex, ColorWhite);
  }
  else {
    mWhiteChecked = isCellAttacked(mWhiteKingIndex, ColorBlack);
  }

  mSideToMove = (mSideToMove == ColorWhite) ? ColorBlack : ColorWhite;

  loadState();

  return true;
}
