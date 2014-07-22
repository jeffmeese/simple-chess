#include "board0x88.h"
#include "fen.h"
#include "move.h"
#include "movelist.h"

#include <iostream>

static const char NORTH = 16;
static const char SOUTH = -16;
static const char EAST = 1;
static const char WEST = -1;
static const char NW = 15;
static const char NE = 17;
static const char SW = -17;
static const char SE = -15;

static const char straightAttacks[] = {NORTH, SOUTH, EAST, WEST};
static const char diagAttacks[] = {NW, NE, SE, SW};
static const char knightAttacks[] = {-31, -33, -14, -18, 18, 14, 33, 31};
static const bool sliders[5] = {0, 1, 1, 1, 0};
static const char numDirections[5] = {8, 8, 4, 4, 8};
static const char dirVectors[5][8] =
{
  { SW, SOUTH, SE, WEST, EAST, NW, NORTH, NE },
  { SW, SOUTH, SE, WEST, EAST, NW, NORTH, NE },
  { SOUTH, WEST, EAST, NORTH                 },
  { SW, SE, NW, NE                           },
  { -31, -33, -14, -18, 18, 14, 33, 31       }
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
  initAttacks();
  //initMoves();
}

Board0x88::~Board0x88(void)
{
}

void Board0x88::generateCastlingMoves(MoveList & moveList)
{
  if (mSideToMove == White) {
    if (mCastlingRights & CastleWhiteKing) {
      if (mPieces[F1] == PieceEmpty &&
          mPieces[G1] == PieceEmpty &&
          !isCellAttacked(E1, Black) &&
          !isCellAttacked(F1, Black) &&
          !isCellAttacked(G1, Black))
      {
        pushMove(E1, G1, King, PieceEmpty, MoveCastle, moveList);
      }
    }
    if (mCastlingRights & CastleWhiteQueen) {
      if (mPieces[D1] == PieceEmpty &&
          mPieces[C1] == PieceEmpty &&
          mPieces[B1] == PieceEmpty &&
          !isCellAttacked(E1, Black) &&
          !isCellAttacked(D1, Black) &&
          !isCellAttacked(C1, Black))
      {
        pushMove(E1, C1, King, PieceEmpty, MoveCastle, moveList);
      }
    }
  }
  else {
    if (mCastlingRights & CastleBlackKing) {
      if (mPieces[F8] == PieceEmpty &&
          mPieces[G8] == PieceEmpty &&
          !isCellAttacked(E8, White) &&
          !isCellAttacked(F8, White) &&
          !isCellAttacked(G8, White))
      {
        pushMove(E8, G8, King, PieceEmpty, MoveCastle, moveList);
      }
    }
    if (mCastlingRights & CastleBlackQueen) {
      if (mPieces[D8] == PieceEmpty &&
          mPieces[C8] == PieceEmpty &&
          mPieces[B8] == PieceEmpty &&
          !isCellAttacked(E8, White) &&
          !isCellAttacked(D8, White) &&
          !isCellAttacked(C8, White))
      {
        pushMove(E8, C8, King, PieceEmpty, MoveCastle, moveList);
      }
    }
  }
}

uchar Board0x88::generateMoves(MoveList & moveList)
{
  generateCastlingMoves(moveList);

  for (uchar i = 0; i < 8; i++) {
    for (uchar col = 0; col < 8; col++) {

    //for (uchar index = 0; index < 120; index++) {
      uint index = getIndex(7-i, col);
      if (mColors[index] == mSideToMove) {
        if (mPieces[index] == Pawn) {
          generatePawnMoves(index, moveList);
          generatePawnCaptures(index, moveList);
        }
        else {
          uchar pieceType = mPieces[index];
          for (uchar num = 0; num < mNumDirections[pieceType]; num++) {
            for (uchar pos = index;;) {
              pos = pos + mDirectionVectors(pieceType, num);

              if (!isValidSquare(pos))
                break;

              if (mColors[pos] == ColorEmpty) {  // Standard move
                pushMove(index, pos, pieceType, PieceEmpty, MoveNormal, moveList);
              }
              else if (mColors[pos] != mSideToMove) { // Capture move
                pushMove(index, pos, pieceType, mPieces[pos], MoveCapture, moveList);
                break;
              }
              else {
                break;
              }

              // Break on non-sliding pieces (King, Knight)
              if (!mSliders[pieceType])
                break;
            }
          }
        }
      }
    }
  }
  return moveList.size();
}

// This function should not be called by engines, it would be greatly inefficient
// It exists simply to find the legal moves for a particular
// square, such as is needed when running in a GUI
uchar Board0x88::generateMoves(uchar row, uchar col, MoveList & moveList)
{
  MoveList allMoves;
  uchar totalMoves = generateMoves(allMoves);

  for (uchar i = 0; i < totalMoves; i++) {
    if (allMoves[i].mSourceRow == row && allMoves[i].mSourceCol == col) {
      makeMove(allMoves[i]);
      if (!isCellAttacked(mKingIndex[!mSideToMove], mSideToMove) )
        moveList.addMove(allMoves[i]);
      unmakeMove(allMoves[i]);
    }
  }
  return moveList.size();
}

uchar Board0x88::generateOpponentMoves(MoveList & moveList)
{
  uchar sideToMove = mSideToMove;
  mSideToMove = !mSideToMove;
  generateMoves(moveList);
  mSideToMove = sideToMove;
  return moveList.size();
}

void Board0x88::generatePawnCaptures(uchar index, MoveList & moveList)
{
  if (mSideToMove == White) {
    if (isValidSquare(index+NW) && ( (mEpIndex == index+NW ) || mColors[index+NW] == Black ) )
      pushMove(index, index+NW, Pawn, mPieces[index+NW], MoveCapture, moveList);
    if (isValidSquare(index+NE) && ( (mEpIndex == index+NE ) || mColors[index+NE] == Black ) )
      pushMove(index, index+NE, Pawn, mPieces[index+NE], MoveCapture, moveList);
  }
  else {
    if (isValidSquare(index+SW) && ( (mEpIndex == index+SW ) || mColors[index+SW] == White ) )
      pushMove(index, index+SW, Pawn, mPieces[index+SW], MoveCapture, moveList);
    if (isValidSquare(index+SE) && ( (mEpIndex == index+SE ) || mColors[index+SE] == White ) )
      pushMove(index, index+SE, Pawn, mPieces[index+SE], MoveCapture, moveList);
  }
}

void Board0x88::generatePawnMoves(uchar index, MoveList & moveList)
{
  char dir = (mSideToMove == White) ? NORTH : SOUTH;
  char epRow = (mSideToMove == White) ? 1 : 6;

  if (mPieces[index+dir] == PieceEmpty) {
    pushMove(index, index+dir, Pawn, PieceEmpty, MoveNormal, moveList);
    if (getRow(index) == epRow && mPieces[index+dir+dir] == PieceEmpty)
      pushMove(index, index+dir+dir, Pawn, PieceEmpty, MoveEp, moveList);
  }
}

void Board0x88::initAttacks()
{
  mNumDiagAttacks.resize(128, 4);
  mNumStraightAttacks.resize(128, 4);
  mKnightAttacks.resize(128, 8);
  mKingAttacks.resize(128, 8);
  for (int i = 0; i < 4; i++) {
    mDiagAttacks[i].resize(128, 8);
    mStraightAttacks[i].resize(128, 8);
  }

  for (uint index = 0; index < 128; index++) {

    // Initialize knight attacks
    mNumKnightAttacks[index] = 0;
    for (int direction = 0, attackIndex = 0; direction < numDirections[Knight]; direction++) {
      uchar pos = index + dirVectors[Knight][direction];
      if (isValidSquare(pos)) {
        mNumKnightAttacks[index]++;
        mKnightAttacks(index, attackIndex) = pos;
        ++attackIndex;
      }
    }

    // Initialize king attacks
    mNumKingAttacks[index] = 0;
    for (int direction = 0, attackIndex = 0; direction < numDirections[King]; direction++) {
      uchar pos = index + dirVectors[King][direction];
      if (isValidSquare(pos)) {
        mNumKingAttacks[index]++;
        mKingAttacks(index, attackIndex) = pos;
        attackIndex++;
      }
    }

    // Initialize straight attacks
    for (uchar i = 0; i < 4; i++) {
      Array2D<uchar> & attackVector = mStraightAttacks[i];
      uchar & numAttacks = mNumStraightAttacks(index, i);
      uchar pos = index + straightAttacks[i];
      uchar sq = 0;
      numAttacks = 0;
      while (isValidSquare(pos)) {
        numAttacks++;
        attackVector(index, sq) = pos;
        pos += straightAttacks[i];
        sq++;
      }
    }

    for (uchar i = 0; i < 4; i++) {
      Array2D<uchar> & attackVector = mDiagAttacks[i];
      uchar & numAttacks = mNumDiagAttacks(index, i);
      uchar pos = index + diagAttacks[i];
      uchar sq = 0;
      numAttacks = 0;
      while (isValidSquare(pos)) {
        numAttacks++;
        attackVector(index, sq) = pos;
        pos += diagAttacks[i];
        sq++;
      }
    }
  }
}

void Board0x88::initBoard()
{
  static const Piece pieces[] = { Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook };

  mDirectionVectors.resize(5, 8);
  for (int i = 0; i < 5; i++) {
    mSliders[i] = sliders[i];
    mNumDirections[i] = numDirections[i];
    for (int j = 0; j < 8; j++) {
      mDirectionVectors(i,j) = dirVectors[i][j];
    }
  }

  mEpIndex = -1;
  mSideToMove = White;
  mHalfMoveClock = 0;
  mFullMoveCounter = 0;
  mCastlingRights = CastleBlackKing | CastleBlackQueen | CastleWhiteKing | CastleWhiteQueen;

  mKingIndex[White] = E1;
  mKingIndex[Black] = E8;
  for (uint index = 0; index < 128; index++) {
    mPieces[index] = PieceEmpty;
    mColors[index] = ColorEmpty;
  }

  for (uint col = 0; col < 8; col++) {
    mColors[getIndex(0, col)] = White;
    mColors[getIndex(1, col)] = White;
    mColors[getIndex(6, col)] = Black;
    mColors[getIndex(7, col)] = Black;

    mPieces[getIndex(0, col)] = pieces[col];
    mPieces[getIndex(1, col)] = Pawn;
    mPieces[getIndex(6, col)] = Pawn;
    mPieces[getIndex(7, col)] = pieces[col];
  }
}

void Board0x88::initMoves()
{

}

bool Board0x88::isCellAttacked(uchar index, char attackingColor) const
{
  // Pawn attacks
  if (attackingColor == White) {
    if (mPieces[index+SE] == Pawn && mColors[index+SE] == White) return true;
    if (mPieces[index+SW] == Pawn && mColors[index+SW] == White) return true;
  }
  else {
    if (mPieces[index+NE] == Pawn && mColors[index+NE] == Black) return true;
    if (mPieces[index+NW] == Pawn && mColors[index+NW] == Black) return true;
  }

//#define PRELOADED

#ifndef PRELOADED
  // Rook, Queen, and King straight attacks
  for (uchar i = 0; i < 4; i++) {
    char dir = straightAttacks[i];
    for (uchar pos = index + dir;; pos += dir) {

      if (!isValidSquare(pos))
        break;

      if (mPieces[pos] != PieceEmpty) {

        if ( (pos == index + dir) && (mColors[pos] == attackingColor) && (mPieces[pos] == King) )
          return true;

        if ( (mColors[pos] == attackingColor) && (mPieces[pos] == Queen || mPieces[pos] == Rook) )
          return true;

        break;
      }
    }
  }

  // Bishop, Queen, and King diagonal attacks
  for (uchar i = 0; i < 4; i++) {

    char dir = diagAttacks[i];
    for (uchar pos = index + dir;; pos += dir) {

      if (!isValidSquare(pos))
        break;

      if (mPieces[pos] != PieceEmpty) {

        if ( (pos == index + dir) && (mColors[pos] == attackingColor) && (mPieces[pos] == King) )
          return true;

        if ( (mColors[pos] == attackingColor) && (mPieces[pos] == Queen || mPieces[pos] == Bishop) )
          return true;

        break;
      }
    }
  }

  // Knight Attacks
  for (uchar i = 0; i < 8; i++) {
    uchar pos = index + knightAttacks[i];
    if (isValidSquare(pos) && mColors[pos] == attackingColor && mPieces[pos] == Knight)
      return true;
  }

#else  // PRELOADED
  // Rook and Queen straight attacks
  for (uint direction = 0; direction < 4; direction++) {
    uchar numAttacks = mNumStraightAttacks(index, direction);
    const Array2D<uchar> & attackVector = mStraightAttacks[direction];
    for (uchar i = 0; i < numAttacks; i++) {
      uchar pos = attackVector(index, i);

      if (mColors[pos] == attackingColor && (mPieces[pos] == Rook || mPieces[pos] == Queen) )
        return true;

      if (mPieces[pos] != PieceEmpty)
        break;
    }
  }

  // Bishop and Queen diag attacks
  for (uint direction = 0; direction < 4; direction++) {
    uchar numAttacks = mNumDiagAttacks(index, direction);
    const Array2D<uchar> & attackVector = mDiagAttacks[direction];
    for (uchar i = 0; i < numAttacks; i++) {
      uchar pos = attackVector(index, i);

      if (mColors[pos] == attackingColor && (mPieces[pos] == Bishop || mPieces[pos] == Queen) )
        return true;

      if (mPieces[pos] != PieceEmpty)
        break;
    }
  }

  // Pawn attacks
  if (attackingColor == White) {
    if (mPieces[index+SE] == Pawn && mColors[index+SE] == White) return true;
    if (mPieces[index+SW] == Pawn && mColors[index+SW] == White) return true;
  }
  else {
    if (mPieces[index+NE] == Pawn && mColors[index+NE] == Black) return true;
    if (mPieces[index+NW] == Pawn && mColors[index+NW] == Black) return true;
  }

  // Knight Attacks
  uint numKnightAttacks = mNumKnightAttacks[index];
  for (uint i = 0; i < numKnightAttacks; i++) {
    uint pos = mKnightAttacks(index, i);
    if (mColors[pos] == attackingColor && mPieces[pos] == Knight)
      return true;
  }

  // King Attacks
  uint numKingAttacks = mNumKingAttacks[index];
  for (uint i = 0; i < numKingAttacks; i++) {
    uint pos = mKingAttacks(index, i);
    if (mColors[pos] == attackingColor && mPieces[pos] == King)
      return true;
  }
#endif // #ifndef PRELOADED

  return false;
}

void Board0x88::makeMove(const Move & newMove)
{
  uchar fromSquare = getIndex(newMove.mSourceRow, newMove.mSourceCol);
  uchar toSquare = getIndex(newMove.mDestRow, newMove.mDestCol);

  mPieces[fromSquare] = PieceEmpty;
  mColors[fromSquare] = ColorEmpty;
  mPieces[toSquare] = newMove.mToPiece;
  mColors[toSquare] = mSideToMove;

  mHalfMoveClock++;
  if (newMove.mFromPiece == Pawn || newMove.mFlags & MoveCapture)
    mHalfMoveClock = 0;

  if (newMove.mFromPiece == King)
    mKingIndex[mSideToMove] = toSquare;

  // Update castling rights
  // If the square associated with a king or rook is
  // modified in any way remove the castling ability
  // for that piece
  switch (fromSquare) {
  case H1:
    mCastlingRights &= ~CastleWhiteKing;
    break;
  case E1:
    mCastlingRights &= ~(CastleWhiteKing|CastleWhiteQueen);
    break;
  case A1:
    mCastlingRights &= ~CastleWhiteQueen;
    break;
  case H8:
    mCastlingRights &= ~CastleBlackKing;
    break;
  case E8:
    mCastlingRights &= ~(CastleBlackKing|CastleBlackQueen);
    break;
  case A8:
    mCastlingRights &= ~CastleBlackQueen;
    break;
  }

  switch (toSquare) {
  case H1:
    mCastlingRights &= ~CastleWhiteKing;
    break;
  case E1:
    mCastlingRights &= ~(CastleWhiteKing|CastleWhiteQueen);
    break;
  case A1:
    mCastlingRights &= ~CastleWhiteQueen;
    break;
  case H8:
    mCastlingRights &= ~CastleBlackKing;
    break;
  case E8:
    mCastlingRights &= ~(CastleBlackKing|CastleBlackQueen);
    break;
  case A8:
    mCastlingRights &= ~CastleBlackQueen;
    break;
  }

  if (newMove.mFlags & MoveCastle) {
    switch (toSquare) {
    case C1:
      mPieces[D1] = Rook;
      mColors[D1] = White;
      mPieces[A1] = PieceEmpty;
      mColors[A1] = ColorEmpty;
      break;
    case G1:
      mPieces[F1] = Rook;
      mColors[F1] = White;
      mPieces[H1] = PieceEmpty;
      mColors[H1] = ColorEmpty;
      break;
    case C8:
      mPieces[D8] = Rook;
      mColors[D8] = Black;
      mPieces[A8] = PieceEmpty;
      mColors[A8] = ColorEmpty;
      break;
    case G8:
      mPieces[F8] = Rook;
      mColors[F8] = Black;
      mPieces[H8] = PieceEmpty;
      mColors[H8] = ColorEmpty;
      break;
    }
  }

  // Update the en-passant index
  mEpIndex = -1;
  if (newMove.mFlags & MoveEp) {
    char dir = (mSideToMove == White) ? SOUTH : NORTH;
    mEpIndex = toSquare + dir;
  }

  // Handle en-passant capture
  if (newMove.mFlags & MoveEpCapture) {
    char dir = (mSideToMove == White) ? SOUTH : NORTH;
    mPieces[toSquare+dir] = PieceEmpty;
    mColors[toSquare+dir] = ColorEmpty;
  }

  mSideToMove = !mSideToMove;
}

PieceType Board0x88::pieceType(uchar row, uchar col) const
{
  uchar index = getIndex(row, col);
  uchar piece = mPieces[index];
  uchar color = mColors[index];

  PieceType type = NoPiece;
  switch (piece) {
  case Pawn:
    type = (color == White) ? WhitePawn : BlackPawn;
    break;
  case Rook:
    type = (color == White) ? WhiteRook : BlackRook;
    break;
  case Knight:
    type = (color == White) ? WhiteKnight : BlackKnight;
    break;
  case Bishop:
    type = (color == White) ? WhiteBishop : BlackBishop;
    break;
  case Queen:
    type = (color == White) ? WhiteQueen : BlackQueen;
    break;
  case King:
    type = (color == White) ? WhiteKing : BlackKing;
    break;
  default:
    break;
  }

  return type;
}

void Board0x88::pushMove(uchar fromSquare, uchar toSquare, uchar fromPiece, uchar capturePiece, char flags, MoveList & moveList)
{
  uchar sourceRow = getRow(fromSquare);
  uchar sourceCol = getCol(fromSquare);
  uchar destRow = getRow(toSquare);
  uchar destCol = getCol(toSquare);
  char epCol = (mEpIndex != -1) ? getCol(mEpIndex) : -1;

  Move newMove;
  newMove.mSourceRow = sourceRow;
  newMove.mSourceCol = sourceCol;
  newMove.mDestRow = destRow;
  newMove.mDestCol = destCol;
  newMove.mFromPiece = fromPiece;
  newMove.mToPiece = fromPiece;
  newMove.mCastlingRights = mCastlingRights;
  newMove.mHalfMoveClock = mHalfMoveClock;
  newMove.mEnPassantCol = epCol;
  newMove.mFlags = flags;
  newMove.mCapturePiece = capturePiece;

  if ( fromPiece == Pawn && toSquare == mEpIndex )
    newMove.mFlags = MoveEpCapture;

  if ( fromPiece == Pawn && ( ( destRow == 0 ) || ( destRow == 7) ) ) {
    newMove.mFlags |= MovePromotion;
    for (uchar promoPiece = Queen; promoPiece <= Knight; promoPiece++) {
      newMove.mToPiece = promoPiece;
      moveList.addMove(newMove);
    }
  }
  else
    moveList.addMove(newMove);
}

bool Board0x88::setPosition(const std::string & fenString)
{
  FenData fenData;
  if (!readFenString(fenString, fenData))
    return false;

  for (uchar i = 0; i < 8; i++) {
    for (uchar j = 0; j < 8; j++) {
      uchar index = getIndex(i, j);
      switch (fenData.mPieceType[i][j]) {
      case WhitePawn:
        mPieces[index] = Pawn;
        mColors[index] = White;
        break;
      case WhiteRook:
        mPieces[index] = Rook;
        mColors[index] = White;
        break;
      case WhiteBishop:
        mPieces[index] = Bishop;
        mColors[index] = White;
        break;
      case WhiteKnight:
        mPieces[index] = Knight;
        mColors[index] = White;
        break;
      case WhiteQueen:
        mPieces[index] = Queen;
        mColors[index] = White;
        break;
      case WhiteKing:
        mPieces[index] = King;
        mColors[index] = White;
        mKingIndex[White] = index;
        break;
      case BlackPawn:
        mPieces[index] = Pawn;
        mColors[index] = Black;
        break;
      case BlackRook:
        mPieces[index] = Rook;
        mColors[index] = Black;
        break;
      case BlackBishop:
        mPieces[index] = Bishop;
        mColors[index] = Black;
        break;
      case BlackKnight:
        mPieces[index] = Knight;
        mColors[index] = Black;
        break;
      case BlackQueen:
        mPieces[index] = Queen;
        mColors[index] = Black;
        break;
      case BlackKing:
        mPieces[index] = King;
        mColors[index] = Black;
        mKingIndex[Black] = index;
        break;
      case NoPiece:
        mPieces[index] = PieceEmpty;
        mColors[index] = ColorEmpty;
        break;
      default:
        break;
      }
    }
  }

  mSideToMove = White;
  if (!fenData.mWhiteToMove)
    mSideToMove = Black;

  mCastlingRights = 0;
  if (fenData.mBlackCastleKingSide)
    mCastlingRights |= CastleBlackKing;
  if (fenData.mBlackCastleQueenSide)
    mCastlingRights |= CastleBlackQueen;
  if (fenData.mWhiteCastleKingSide)
    mCastlingRights |= CastleWhiteKing;
  if (fenData.mWhiteCastleQueenSide)
    mCastlingRights |= CastleWhiteQueen;

  mEpIndex = 0x7f;
  if (fenData.mEnPassantCol > 0 ) {
    uchar row = (mSideToMove == White) ? 5 : 2;
    mEpIndex = fenData.mEnPassantCol + (16 * row);
  }

  mHalfMoveClock = fenData.mHalfMoveClock;
  mFullMoveCounter = fenData.mFullMoveCounter;

  return true;
}

void Board0x88::unmakeMove(const Move & undoMove)
{
  static const uchar epRow[] = {2, 5};

  uchar fromSquare = getIndex(undoMove.mSourceRow, undoMove.mSourceCol);
  uchar toSquare = getIndex(undoMove.mDestRow, undoMove.mDestCol);
  uchar fromPiece = undoMove.mFromPiece;
  uchar flags = undoMove.mFlags;

  mPieces[fromSquare] = fromPiece;
  mColors[fromSquare] = !mSideToMove;
  mPieces[toSquare] = PieceEmpty;
  mColors[toSquare] = ColorEmpty;
  mHalfMoveClock = undoMove.mHalfMoveClock;
  mCastlingRights = undoMove.mCastlingRights;

  if (undoMove.mEnPassantCol >= 0) {
    //uchar epRow = (mSideToMove == White) ? 2 : 5;
    mEpIndex = getIndex(epRow[mSideToMove], undoMove.mEnPassantCol);
  }

  if (flags & MoveCapture) {
    mPieces[toSquare] = undoMove.mCapturePiece;
    mColors[toSquare] = mSideToMove;
  }

  if (fromPiece == King)
    mKingIndex[!mSideToMove] = fromSquare;

  if (flags & MoveEpCapture) {
    int dir = (mSideToMove == White) ? NORTH : SOUTH;
    mPieces[toSquare+dir] = Pawn;
    mColors[toSquare+dir] = mSideToMove;
  }

  if (flags & MoveCastle) {
    switch (toSquare) {
    case C1:
      mPieces[A1] = Rook;
      mColors[A1] = White;
      mPieces[D1] = PieceEmpty;
      mColors[D1] = ColorEmpty;
      break;
    case G1:
      mPieces[H1] = Rook;
      mColors[H1] = White;
      mPieces[F1] = PieceEmpty;
      mColors[F1] = ColorEmpty;
      break;
    case C8:
      mPieces[A8] = Rook;
      mColors[A8] = Black;
      mPieces[D8] = PieceEmpty;
      mColors[D8] = ColorEmpty;
      break;
    case G8:
      mPieces[H8] = Rook;
      mColors[H8] = Black;
      mPieces[F8] = PieceEmpty;
      mColors[F8] = ColorEmpty;
      break;
    }
  }

  mSideToMove = !mSideToMove;
}
