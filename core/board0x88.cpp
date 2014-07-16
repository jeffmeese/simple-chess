#include "board0x88.h"
#include "fen.h"

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
  //initAttacks();
  //initPieceMap();
}

Board0x88::~Board0x88(void)
{
}

void Board0x88::generateCastlingMoves()
{
  // Castling
  if (mSideToMove == White) {
    if (mCastlingRights & CastleWhiteKing) {
      if (mPieces[F1] == PieceEmpty &&
          mPieces[G1] == PieceEmpty &&
          !isCellAttacked(E1, Black) &&
          !isCellAttacked(F1, Black) &&
          !isCellAttacked(G1, Black))
      {
        pushMove(E1, G1, King, PieceEmpty, MoveCastle);
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
        pushMove(E1, C1, King, PieceEmpty, MoveCastle);
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
        pushMove(E8, G8, King, PieceEmpty, MoveCastle);
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
        pushMove(E8, C8, King, PieceEmpty, MoveCastle);
      }
    }
  }
}

uchar Board0x88::generateMoves(sMove * moveList)
{
  mMoves = moveList;
  mMoveCount = 0;

  generateCastlingMoves();

  for (uchar i = 0; i < 8; i++) {
    for (uchar col = 0; col < 8; col++) {

    //for (uchar index = 0; index < 120; index++) {
      uint index = getIndex(7-i, col);
      if (mColors[index] == mSideToMove) {
        if (mPieces[index] == Pawn) {
          generatePawnMoves(index);
          generatePawnCaptures(index);
        }
        else {
          uchar pieceType = mPieces[index];
          for (uchar num = 0; num < mNumDirections[pieceType]; num++) {
            for (uchar pos = index;;) {
              pos = pos + mDirectionVectors(pieceType, num);

              if (!isValidSquare(pos))
                break;

              if (mColors[pos] == ColorEmpty) {  // Standard move
                pushMove(index, pos, pieceType, PieceEmpty, MoveNormal);
              }
              else if (mColors[pos] != mSideToMove) { // Capture move
                pushMove(index, pos, pieceType, mPieces[pos], MoveCapture);
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

  return mMoveCount;
}

// This function should not be called by engines
// It exists simply to find the legal moves for a particular
// square, such as is needed when running in a GUI
uchar Board0x88::generateMoves(uchar row, uchar col, sMove * moveList)
{
  sMove allMoves[256];
  uchar totalMoves = generateMoves(allMoves);

  uchar moveIndex = 0;
  for (uchar i = 0; i < totalMoves; i++) {
    sMove move = allMoves[i];
    if (getRow(move.fromSquare) == row && getCol(move.fromSquare) == col) {
      makeMove(move);
      if (!isCellAttacked(mKingIndex[!mSideToMove], mSideToMove) ) {
        moveList[moveIndex] = move;
        moveIndex++;
      }
      unmakeMove(move);
    }
  }
  return moveIndex;
}

void Board0x88::generatePawnCaptures(uchar index)
{
  if (mSideToMove == White) {
    if (isValidSquare(index+NW) && ( (mEpIndex == index+NW ) || mColors[index+NW] == Black ) )
      pushMove(index, index+NW, Pawn, mPieces[index+NW], MoveCapture);
    if (isValidSquare(index+NE) && ( (mEpIndex == index+NE ) || mColors[index+NE] == Black ) )
      pushMove(index, index+NE, Pawn, mPieces[index+NE], MoveCapture);
  }
  else {
    if (isValidSquare(index+SW) && ( (mEpIndex == index+SW ) || mColors[index+SW] == White ) )
      pushMove(index, index+SW, Pawn, mPieces[index+SW], MoveCapture);
    if (isValidSquare(index+SE) && ( (mEpIndex == index+SE ) || mColors[index+SE] == White ) )
      pushMove(index, index+SE, Pawn, mPieces[index+SE], MoveCapture);
  }
}

void Board0x88::generatePawnMoves(uchar index)
{
  char dir = NORTH;
  char epRow = 1;
  if (mSideToMove == Black) {
    dir = SOUTH;
    epRow = 6;
  }

  if (mPieces[index+dir] == PieceEmpty) {
    pushMove(index, index+dir, Pawn, PieceEmpty, MoveNormal);
    if (getRow(index) == epRow && mPieces[index+dir+dir] == PieceEmpty)
      pushMove(index, index+dir+dir, Pawn, PieceEmpty, MoveEp);
  }
}

std::string Board0x88::getSmithNotation(const sMove & move) const
{
  uchar sourceRow = getRow(move.fromSquare);
  uchar sourceCol = getCol(move.fromSquare);
  uchar destRow = getRow(move.toSquare);
  uchar destCol = getCol(move.toSquare);

  Move newMove(sourceRow, sourceCol, destRow, destCol);

  if (move.flags & MoveCapture) {
    uchar capturePiece = move.capturedPiece;
    uchar color = mColors[move.toSquare];

    switch (capturePiece) {
    case Pawn:
      newMove.setCapturedPiece( color==White ? WhitePawn : BlackPawn);
      break;
    case Rook:
      newMove.setCapturedPiece( color==White ? WhiteRook : BlackRook);
      break;
    case Knight:
      newMove.setCapturedPiece( color==White ? WhiteKnight : BlackKnight);
      break;
    case Bishop:
      newMove.setCapturedPiece( color==White ? WhiteBishop : BlackBishop);
      break;
    case Queen:
      newMove.setCapturedPiece( color==White ? WhiteQueen : BlackQueen);
      break;
    case King:
      newMove.setCapturedPiece( color==White ? WhiteKing : BlackKing);
      break;
    }
  }

  if (move.flags & MovePromotion) {
    uchar promoPiece = move.toPiece;
    uchar color = mColors[move.fromSquare];
    switch (promoPiece) {
    case Rook:
      newMove.setPromotedPiece( color==White ? WhiteRook : BlackRook);
      break;
    case Knight:
      newMove.setPromotedPiece( color==White ? WhiteKnight : BlackKnight);
      break;
    case Bishop:
      newMove.setPromotedPiece( color==White ? WhiteBishop : BlackBishop);
      break;
    case Queen:
      newMove.setPromotedPiece( color==White ? WhiteQueen : BlackQueen);
      break;
    }
  }

  return newMove.getSmithNotation();
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

  mEpIndex = 0x7f;
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

bool Board0x88::isCellAttacked(uchar index, uchar attackingColor) const
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

  return false;
}

void Board0x88::makeMove(const sMove &newMove)
{
  mPieces[newMove.fromSquare] = PieceEmpty;
  mColors[newMove.fromSquare] = ColorEmpty;
  mPieces[newMove.toSquare] = newMove.toPiece;
  mColors[newMove.toSquare] = mSideToMove;

  mHalfMoveClock++;
  if (newMove.fromPiece == Pawn || newMove.flags & MoveCapture)
    mHalfMoveClock = 0;

  if (newMove.fromPiece == King)
    mKingIndex[mSideToMove] = newMove.toSquare;

  // Update castling rights
  // If the square associated with a king or rook is
  // modified in any way remove the castling ability
  // for that piece
  switch (newMove.fromSquare) {
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

  switch (newMove.toSquare) {
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

  if (newMove.flags & MoveCastle) {
    switch (newMove.toSquare) {
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
  mEpIndex = 0x7f;
  if (newMove.flags & MoveEp) {
    char dir = (mSideToMove == White) ? SOUTH : NORTH;
    mEpIndex = newMove.toSquare + dir;
  }

  // Handle en-passant capture
  if (newMove.flags & MoveEpCapture) {
    char dir = (mSideToMove == White) ? SOUTH : NORTH;
    mPieces[newMove.toSquare+dir] = PieceEmpty;
    mColors[newMove.toSquare+dir] = ColorEmpty;
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

void Board0x88::pushMove(uchar fromSquare, uchar toSquare, uchar fromPiece, uchar capturePiece, uchar flags)
{
  mMoves[mMoveCount].fromSquare = fromSquare;
  mMoves[mMoveCount].toSquare = toSquare;
  mMoves[mMoveCount].fromPiece = fromPiece;
  mMoves[mMoveCount].toPiece = fromPiece;
  mMoves[mMoveCount].capturedPiece = capturePiece;
  mMoves[mMoveCount].flags = flags;
  mMoves[mMoveCount].castlingRights = mCastlingRights;
  mMoves[mMoveCount].halfMove = mHalfMoveClock;
  mMoves[mMoveCount].epIndex = mEpIndex;

  if ( fromPiece == Pawn && toSquare == mEpIndex )
    mMoves[mMoveCount].flags = MoveEpCapture;

  if ( fromPiece == Pawn && ( ( getRow(toSquare) == 0 ) || ( getRow(toSquare) == 7) ) ) {
    mMoves[mMoveCount].flags |= MovePromotion;
    for (uchar promoPiece = Queen; promoPiece <= Knight; promoPiece++) {
      mMoves[mMoveCount+promoPiece-1] = mMoves[mMoveCount];
      mMoves[mMoveCount+promoPiece-1].toPiece = promoPiece;
    }
    mMoveCount += 3;
  }
  mMoveCount++;
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

void Board0x88::unmakeMove(const sMove &undoMove)
{
  mPieces[undoMove.fromSquare] = undoMove.fromPiece;
  mColors[undoMove.fromSquare] = !mSideToMove;
  mPieces[undoMove.toSquare] = PieceEmpty;
  mColors[undoMove.toSquare] = ColorEmpty;

  mHalfMoveClock = undoMove.halfMove;
  mEpIndex = undoMove.epIndex;
  mCastlingRights = undoMove.castlingRights;

  if (undoMove.flags & MoveCapture) {
    mPieces[undoMove.toSquare] = undoMove.capturedPiece;
    mColors[undoMove.toSquare] = mSideToMove;
  }

  if (undoMove.fromPiece == King)
    mKingIndex[!mSideToMove] = undoMove.fromSquare;

  if (undoMove.flags & MoveEpCapture) {
    int dir = (mSideToMove == White) ? NORTH : SOUTH;
    mPieces[undoMove.toSquare+dir] = Pawn;
    mColors[undoMove.toSquare+dir] = mSideToMove;
  }

  if (undoMove.flags & MoveCastle) {
    switch (undoMove.toSquare) {
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
