#include "board0x88.h"
#include "move.h"
#include "movelist.h"
#include "fenparser.h"

#include <iostream>
#include <sstream>
#include <map>

//#define isValidSquare(x)  ( (x) & 0x88 ) ? (0) : (1)

static const int NORTH = 16;
static const int SOUTH = -16;
static const int EAST = 1;
static const int WEST = -1;
static const int NW = 15;
static const int NE = 17;
static const int SW = -17;
static const int SE = -15;
static const int InvalidIndex = 0x7f;

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
  initMoves();
  initAttacks();
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

uint Board0x88::enPassantIndex() const
{
  return mEnPassantIndex;
}

uint Board0x88::whiteKingIndex() const
{
  return mWhiteKingIndex;
}

uint Board0x88::blackKingIndex() const
{
  return mBlackKingIndex;
}

uint Board0x88::generateLegalMoves(MoveList & moveList)
{
  //MoveList candidateMoves;

  // Castling
  if (mSideToMove == ColorWhite) {
    if (mCastlingRights & CastleWhiteKing) {
      if (mPieces[F1] == Empty &&
          mPieces[G1] == Empty &&
          !isCellAttacked(E1, ColorBlack) &&
          !isCellAttacked(F1, ColorBlack) &&
          !isCellAttacked(G1, ColorBlack))
      {
        Move newMove(getRow(E1), getCol(E1), getRow(G1), getCol(G1));
        newMove.setKingCastle();
        moveList.addMove(newMove);
      }
    }
    if (mCastlingRights & CastleWhiteQueen) {
      if (mPieces[D1] == Empty &&
          mPieces[C1] == Empty &&
          mPieces[B1] == Empty &&
          !isCellAttacked(E1, ColorBlack) &&
          !isCellAttacked(D1, ColorBlack) &&
          !isCellAttacked(C1, ColorBlack))
      {
        Move newMove(getRow(E1), getCol(E1), getRow(C1), getCol(C1));
        newMove.setQueenCastle();
        moveList.addMove(newMove);
      }
    }
  }
  else {
    if (mCastlingRights & CastleBlackKing) {
      if (mPieces[F8] == Empty &&
          mPieces[G8] == Empty &&
          !isCellAttacked(E8, ColorWhite) &&
          !isCellAttacked(F8, ColorWhite) &&
          !isCellAttacked(G8, ColorWhite))
      {
        Move newMove(getRow(E8), getCol(E8), getRow(G8), getCol(G8));
        newMove.setKingCastle();
        moveList.addMove(newMove);
      }
    }
    if (mCastlingRights & CastleBlackQueen) {
      if (mPieces[D8] == Empty &&
          mPieces[C8] == Empty &&
          mPieces[B8] == Empty &&
          !isCellAttacked(E8, ColorWhite) &&
          !isCellAttacked(D8, ColorWhite) &&
          !isCellAttacked(C8, ColorWhite))
      {
        Move newMove(getRow(E8), getCol(E8), getRow(C8), getCol(C8));
        newMove.setQueenCastle();
        moveList.addMove(newMove);
      }
    }
  }

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

            if (mColors[pos] == ColorEmpty) {  // Standard move
              Move newMove(getRow(index), getCol(index), getRow(pos), getCol(pos));
              moveList.addMove(newMove);
            }
            else if (mColors[pos] != mSideToMove) { // Capture move
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
//  for (uint i = 0; i < candidateMoves.totalMoves(); i++) {
//    moveList.addMove(candidateMoves[i]);
//    Move candidateMove(candidateMoves[i]);
//    makeMove(candidateMove);
//    if (!isCellAttacked(getKingIndex(!mSideToMove), mSideToMove))
//      moveList.addMove(candidateMove);
//    unmakeMove(candidateMove);
//  }

  return moveList.totalMoves();
}

uint Board0x88::generateLegalMoves(uint row, uint col, MoveList & moveList)
{
  // This function is generally called from a GUI
  // so speed is not a major concern here
  MoveList canidateMoves;
  generateLegalMoves(canidateMoves);

  for (uint i = 0; i < canidateMoves.totalMoves(); i++) {
    Move candidateMove = canidateMoves[i];
    if (candidateMove.sourceRow() == row && candidateMove.sourceColumn() == col) {
      makeMove(candidateMove);
      if (!isCellAttacked(getKingIndex(!mSideToMove), mSideToMove))
        moveList.addMove(candidateMove);
      unmakeMove(candidateMove);
    }
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

      // Generate double pawn push
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

std::string Board0x88::getFenString() const
{
  std::map<PieceType, std::string> pieceMap;
  pieceMap[WhitePawn] = std::string("P");
  pieceMap[WhiteRook] = std::string("R");
  pieceMap[WhiteKnight] = std::string("N");
  pieceMap[WhiteBishop] = std::string("B");
  pieceMap[WhiteQueen] = std::string("Q");
  pieceMap[WhiteKing] = std::string("K");
  pieceMap[BlackPawn] = std::string("p");
  pieceMap[BlackRook] = std::string("r");
  pieceMap[BlackKnight] = std::string("n");
  pieceMap[BlackBishop] = std::string("b");
  pieceMap[BlackQueen] = std::string("q");
  pieceMap[BlackKing] = std::string("k");

  std::ostringstream oss;
  for (uint i = 0; i < 8; i++) {
    uint row = 7-i;
    uint col = 0;
    while (col < 8) {
      PieceType pieceType = getPieceType(row,col);
      if (pieceType != NoPiece) {
        oss << pieceMap[pieceType];
        ++col;
      }
      else {
        uint totalBlanks = 0;
        while (pieceType == NoPiece && col < 8) {
          ++totalBlanks;
          ++col;
          pieceType = getPieceType(row,col);
        }
        oss << totalBlanks;
      }
    }
    if (row != 0)
      oss << "/";
  }

  oss << " " << (isWhiteToMove() ? "w" : "b") << " ";

  if (canWhiteCastleKingSide())
    oss << "K";
  if (canWhiteCastleQueenSide())
    oss << "Q";
  if (canBlackCastleKingSide())
    oss << "k";
  if (canBlackCastleQueenSide())
    oss << "q";

  if (mCastlingRights == 0)
    oss << "-";

  oss << " ";
  if (!isValidSquare(mEnPassantIndex))
    oss << "-";
  else {
    uint epRow = getRow(mEnPassantIndex);
    uint epCol = getCol(mEnPassantIndex);
    switch (epCol)
    {
    case 0:
      oss << "a";
      break;
    case 1:
      oss << "b";
      break;
    case 2:
      oss << "c";
      break;
    case 3:
      oss << "d";
      break;
    case 4:
      oss << "e";
      break;
    case 5:
      oss << "f";
      break;
    case 6:
      oss << "g";
      break;
    case 7:
      oss << "h";
      break;
    }
    oss << epRow + 1;
  }

  oss << " " << mHalfMoveClock;
  oss << " " << mFullMoveCounter;

  return oss.str();
}

uint Board0x88::getKingIndex(uchar kingColor) const
{
  return ( (kingColor == ColorWhite) ? mWhiteKingIndex : mBlackKingIndex);
}

inline uint Board0x88::getIndex(uint row, uint col) const
{
  //return (row * 16 + col);
  return ( (row << 4) + col);
}

PieceType Board0x88::getPieceType(uint row, uint col) const
{
  return mPieceTypes[getIndex(row, col)];
}

inline uint Board0x88::getRow(uint index) const
{
  return (index >> 4);
}

void Board0x88::initAttacks()
{
  for (uint index = 0; index < 128; index++) {

    // Initialize knight attacks
    mNumKnightAttacks[index] = 0;
    for (int direction = 0, attackIndex = 0; direction < numDirections[Knight]; direction++) {
      uint pos = index + dirVectors[Knight][direction];
      if (isValidSquare(pos)) {
        mNumKnightAttacks[index]++;
        mKnightAttacks[index][attackIndex] = pos;
        ++attackIndex;
      }
    }

    // Initialize king attacks
    mNumKingAttacks[index] = 0;
    for (int direction = 0, attackIndex = 0; direction < numDirections[King]; direction++) {
      uint pos = index + dirVectors[King][direction];
      if (isValidSquare(pos)) {
        mNumKingAttacks[index]++;
        mKingAttacks[index][attackIndex] = pos;
        attackIndex++;
      }
    }

    // Initialize straight attacks
    for (int direction = 0; direction < 4; direction++) {
      mNumStraightAttacks[index][direction] = 0;
      uint pos = index + dirVectors[Rook][direction];
      uint sq = 0;
      while (isValidSquare(pos)) {
        mNumStraightAttacks[index][direction]++;
        mStraightAttacks[index][direction][sq] = pos;
        pos += dirVectors[Rook][direction];
        sq++;
      }
    }

    // Initialize diagonal attacks
    for (int direction = 0; direction < 4; direction++) {
      mNumDiagAttacks[index][direction] = 0;
      uint pos = index + dirVectors[Bishop][direction];
      uint sq = 0;
      while (isValidSquare(pos)) {
        mNumDiagAttacks[index][direction]++;
        mDiagAttacks[index][direction][sq] = pos;
        pos += dirVectors[Bishop][direction];
        sq++;
      }
    }
  }

//  uint numAttacks = mNumKnightAttacks[F3];
//  std::cout << "num: " << numAttacks << "\n";
//  for (uint i = 0; i < numAttacks; i++)
//    std::cout << i << " " << mKnightAttacks[F3][i] << "\n";

//  for (uint dir = 0; dir < 4; dir++) {
//    uint numAttacks = mNumDiagAttacks[D2][dir];
//    std::cout << "num: " << numAttacks << "\n";
//    for (uint i = 0; i < numAttacks; i++)
//      std::cout << i << " " << mDiagAttacks[D2][dir][i] << "\n";
//  }
}

void Board0x88::initBoard()
{
  static const Piece pieces[] =
  { Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook };

  static const PieceType whitePieces[] =
  { WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook };

  static const PieceType blackPieces[] =
  { BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook };

  mWhiteChecked = false;
  mBlackChecked = false;
  mEnPassantIndex = InvalidIndex;
  mHalfMoveClock = 0;
  mFullMoveCounter = 1;
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

    mPieces[getIndex(0, col)] = pieces[col];
    mPieces[getIndex(1, col)] = Pawn;
    mPieces[getIndex(6, col)] = Pawn;
    mPieces[getIndex(7, col)] = pieces[col];
  }
}

void Board0x88::initMoves()
{

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
  return isCellAttacked(mBlackKingIndex, ColorWhite);
}

bool Board0x88::isBlackToMove() const
{
  return (mSideToMove == ColorBlack);
}

bool Board0x88::isCellAttacked(uint index, uchar colorAttacking) const
{
  // Pawn Attacks
  if (colorAttacking == ColorWhite) {
    static const int dirs[] = {SE, SW};
    for (uint i = 0; i < 2; i++) {
      uint pos = index + dirs[i];
      if (isValidSquare(pos) && mPieceTypes[pos] == WhitePawn)
        return true;
    }
  }
  else {
    static const int dirs[] = {NE, NW};
    for (uint i = 0; i < 2; i++) {
      uint pos = index + dirs[i];
      if (isValidSquare(pos) && mPieceTypes[pos] == BlackPawn)
        return true;
    }
  }

  // Knight Attacks
  uint numKnightAttacks = mNumKnightAttacks[index];
  const uint * knightAttacks = mKnightAttacks[index];
  for (uint i = 0; i < numKnightAttacks; i++) {
    uint pos = knightAttacks[i];
    if (mColors[pos] == colorAttacking && mPieces[pos] == Knight)
      return true;
  }

  // King Attacks
  uint numKingAttacks = mNumKingAttacks[index];
  const uint * kingAttacks = mKingAttacks[index];
  for (uint i = 0; i < numKingAttacks; i++) {
    uint pos = kingAttacks[i];
    if (mColors[pos] == colorAttacking && mPieces[pos] == King)
      return true;
  }

  // Rook and Queen straight attacks
  const uint * numStraightAttacks = mNumStraightAttacks[index];
  for (uint direction = 0; direction < 4; direction++) {
    const uint * straightAttacks = mStraightAttacks[index][direction];
    uint numAttacks = numStraightAttacks[direction];
    for (uint i = 0; i < numAttacks; i++) {
      //uint pos = mStraightAttacks[index][direction][i];
      uint pos = straightAttacks[i];

      if (mColors[pos] == colorAttacking && (mPieces[pos] == Rook || mPieces[pos] == Queen) )
        return true;

      if (mPieces[pos] != Empty)
        break;
    }
  }

  // Bishop and Queen diagonal attacks
  const uint * numDiagAttacks = mNumDiagAttacks[index];
  for (uint direction = 0; direction < 4; direction++) {
    const uint * diagAttacks = mDiagAttacks[index][direction];
    uint numAttacks = numDiagAttacks[direction];
    for (uint i = 0; i < numAttacks; i++) {
     // uint pos = mDiagAttacks[index][direction][i];
      uint pos = diagAttacks[i];

      if (mColors[pos] == colorAttacking && (mPieces[pos] == Bishop || mPieces[pos] == Queen) )
        return true;

      if (mPieces[pos] != Empty)
       break;
    }
  }

  return false;
}

bool Board0x88::isWhiteChecked() const
{
  return isCellAttacked(mWhiteKingIndex, ColorBlack);
}

bool Board0x88::isWhiteToMove() const
{
  return (mSideToMove == ColorWhite);
}

inline bool Board0x88::isValidSquare(uint index) const
{
  return !(index & 0x0088);
}

bool Board0x88::leapAttacks(uint index, uchar attackingColor) const
{
  for (uint i = 0; i < mNumKnightAttacks[index]; i++) {
    uint pos = mKnightAttacks[index][i];
    if (mColors[pos] == attackingColor && mPieces[pos] == Knight)
      return true;
  }

  for (uint i = 0; i < mNumKingAttacks[index]; i++) {
    uint pos = mKingAttacks[index][i];
    if (mColors[pos] == attackingColor && mPieces[pos] == King)
      return true;
  }
  return false;
}

void Board0x88::loadState()
{
  ulong savedState = mSavedStates.back();
  mSavedStates.pop_back();

  mCastlingRights |= (savedState & CastleBlackKing);
  mCastlingRights |= (savedState & CastleBlackQueen);
  mCastlingRights |= (savedState & CastleWhiteKing);
  mCastlingRights |= (savedState & CastleWhiteQueen);
  mEnPassantIndex = ((savedState >> 4) & 0x00ff);
  mHalfMoveClock = ((savedState >> 12) & 0x00ff);
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

  if (newMove.isEnPassant()) {
    uint captureIndex = destIndex + ( (mSideToMove == ColorWhite) ? SOUTH : NORTH);
    mPieces[captureIndex] = Empty;
    mColors[captureIndex] = ColorEmpty;
    mPieceTypes[captureIndex] = NoPiece;
  }

  mEnPassantIndex = InvalidIndex;
  if (newMove.isDoublePawnPush()) {
    mEnPassantIndex = destIndex + ( (mSideToMove == ColorWhite) ? SOUTH : NORTH);
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

  mSideToMove = !mSideToMove;

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
  ulong savedState = 0;

  savedState |= (CastleBlackKing | CastleBlackQueen | CastleWhiteKing | CastleWhiteQueen);
  savedState |= ( (mEnPassantIndex & 0x00ff) << 4);
  savedState |= ( (mHalfMoveClock & 0x00ff) << 12);

  mSavedStates.push_back(savedState);
}

void Board0x88::setPosition(const std::string & fenString)
{
  initBoard();

  FenData fenData;
  if (!readFenString(fenString, fenData))
    return;

  for (uint i = 0; i < 8; i++) {
    for (uint j = 0; j < 8; j++) {
      uint index = getIndex(i, j);
      mPieceTypes[index] = fenData.mPieceType[i][j];
      switch (fenData.mPieceType[i][j]) {
      case WhitePawn:
        mPieces[index] = Pawn;
        mColors[index] = ColorWhite;
        break;
      case WhiteRook:
        mPieces[index] = Rook;
        mColors[index] = ColorWhite;
        break;
      case WhiteBishop:
        mPieces[index] = Bishop;
        mColors[index] = ColorWhite;
        break;
      case WhiteKnight:
        mPieces[index] = Knight;
        mColors[index] = ColorWhite;
        break;
      case WhiteQueen:
        mPieces[index] = Queen;
        mColors[index] = ColorWhite;
        break;
      case WhiteKing:
        mPieces[index] = King;
        mColors[index] = ColorWhite;
        mWhiteKingIndex = index;
        break;
      case BlackPawn:
        mPieces[index] = Pawn;
        mColors[index] = ColorBlack;
        break;
      case BlackRook:
        mPieces[index] = Rook;
        mColors[index] = ColorBlack;
        break;
      case BlackBishop:
        mPieces[index] = Bishop;
        mColors[index] = ColorBlack;
        break;
      case BlackKnight:
        mPieces[index] = Knight;
        mColors[index] = ColorBlack;
        break;
      case BlackQueen:
        mPieces[index] = Queen;
        mColors[index] = ColorBlack;
        break;
      case BlackKing:
        mPieces[index] = King;
        mColors[index] = ColorBlack;
        mBlackKingIndex = index;
        break;
      case NoPiece:
        mPieces[index] = Empty;
        mColors[index] = ColorEmpty;
        break;
      }
    }
  }

  mSideToMove = ColorWhite;
  if (!fenData.mWhiteToMove)
    mSideToMove = ColorBlack;

  mCastlingRights = 0;
  if (fenData.mBlackCastleKingSide)
    mCastlingRights |= CastleBlackKing;
  if (fenData.mBlackCastleQueenSide)
    mCastlingRights |= CastleBlackQueen;
  if (fenData.mWhiteCastleKingSide)
    mCastlingRights |= CastleWhiteKing;
  if (fenData.mWhiteCastleQueenSide)
    mCastlingRights |= CastleWhiteQueen;


  mEnPassantIndex = 0;
  if (fenData.mEnPassantCol > 0 ) {
    uint row = (mSideToMove == ColorWhite) ? 5 : 2;
    mEnPassantIndex = fenData.mEnPassantCol + (16 * row);
  }

  mHalfMoveClock = fenData.mHalfMoveClock;
  mFullMoveCounter = fenData.mFullMoveCounter;
}

bool Board0x88::sliderAttack(uint index, uchar attackingColor, int attackDirection, bool diag) const
{
  Piece attackingSlider = (diag) ? Bishop : Rook;
  uint pos = index + attackDirection;

  while (isValidSquare(pos)) {
    Piece pieceType = mPieces[pos];
    if (mColors[pos] != ColorEmpty) {
      if (mColors[pos] == attackingColor && (pieceType == Queen || pieceType == attackingSlider) )
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
  Piece piece = mPieces[destIndex];

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
    uint captureIndex = destIndex + ( (mSideToMove == ColorWhite) ? NORTH : SOUTH);
    mPieces[captureIndex] = Pawn;
    mColors[captureIndex] = ( (mSideToMove == ColorWhite) ? ColorWhite : ColorBlack);
    mPieceTypes[captureIndex] = ( (mSideToMove == ColorWhite) ? WhitePawn : BlackPawn);
  }

  if (undoMove.isEnPassant()) {
  }

  mSideToMove = !mSideToMove;
  loadState();

  return true;
}
