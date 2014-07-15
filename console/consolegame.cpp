#include "consolegame.h"
#include "perft.h"

#include <core/stopwatch.h>

#include <iostream>
#include <algorithm>
#include <sstream>
#include <memory>
#include <iomanip>

ConsoleGame::ConsoleGame()
{
  mGameOver = false;
}

ConsoleGame::~ConsoleGame()
{
}

void ConsoleGame::handleHelp() const
{
  std::cout << "quit.................Quits the program\n";
  std::cout << "help.................Displays this help menu\n";
  std::cout << "print................Prints the current board position\n";
  std::cout << "disp.................Same as print\n";
  std::cout << "new..................Start a new game\n";
  std::cout << "move <smith>.........Performs a move\n";
  std::cout << "perft <level>........Counts the total number of nodes to depth <level>\n";
  std::cout << "divide <level>.......Displays the number of child moves\n";
  std::cout << "table <level>........Displays a table of all perft results from 1 to <level>\n";
  std::cout << "setboard <fen>.......Sets the board position to <fen>\n";
  std::cout << "undo.................Undoes the last move\n";
}

void ConsoleGame::handleMove(std::istringstream & iss)
{
  static std::map<char, int> columnMap, rowMap;

  if (columnMap.size() == 0) {
    columnMap['a'] = 0;
    columnMap['b'] = 1;
    columnMap['c'] = 2;
    columnMap['d'] = 3;
    columnMap['e'] = 4;
    columnMap['f'] = 5;
    columnMap['g'] = 6;
    columnMap['h'] = 7;

    rowMap['1'] = 0;
    rowMap['2'] = 1;
    rowMap['3'] = 2;
    rowMap['4'] = 3;
    rowMap['5'] = 4;
    rowMap['6'] = 5;
    rowMap['7'] = 6;
    rowMap['8'] = 7;

  }
  if (mGameOver) {
    std::cout << "The game is over. Type new to start a new game\n";
    return;
  }

  std::string moveString;
  iss >> moveString;
  if (moveString.empty()) {
    std::cout << "A move string must be provided\n";
    return;
  }

  if (moveString.find_first_not_of("123456789abcdefghpnbrqkEcCNBRQ") != std::string::npos) {
    std::cout << "Invalid move string " << moveString << "\n";
    return;
  }

  if (moveString.length() < 4) {
    std::cout << "Invalid move string " << moveString << "\n";
    return;
  }

  int srcCol = columnMap[moveString.at(0)];
  int srcRow = rowMap[moveString.at(1)];
  int dstCol = columnMap[moveString.at(2)];
  int dstRow = rowMap[moveString.at(3)];

  uchar moveIndex = 0;
  bool moveFound = false;
  sMove moveList[256];
  uchar totalMoves = mBoard->generateMoves(moveList);
  for (uchar i = 0; i < totalMoves; i++) {
    uchar sourceRow = getRow(moveList[i].fromSquare);
    uchar sourceCol = getCol(moveList[i].fromSquare);
    uchar destRow = getRow(moveList[i].toSquare);
    uchar destCol = getCol(moveList[i].toSquare);

    if (srcRow == sourceRow && dstRow == destRow && srcCol == sourceCol && dstCol == destCol) {
      moveIndex = i;
      moveFound = true;
      break;
    }
  }

  if (!moveFound) {
    std::cout << "Sorry " << moveString << " is not a valid move\n";
    for (uint i = 0; i < totalMoves; i++) {
      std::cout << mBoard->getSmithNotation(moveList[i]) << "\n";
    }
    return;
  }

  mBoard->makeMove(moveList[moveIndex]);
  mCompletedMoves[mTotalMoves] = moveList[moveIndex];
  ++mTotalMoves;
  //mGameOver = isGameOver();

  //std::cout << srcRow << " " << srcCol << "\n";
  //std::cout << destRow << " " << destCol << "\n";

//  Move newMove = Move::fromSmithNotation(moveString, isWhiteToMove());
//  uint sourceRow = newMove.sourceRow();
//  uint sourceCol = newMove.sourceColumn();
//  uint destRow = newMove.destinationRow();
//  PieceType pieceType = getPieceType(sourceRow, sourceCol);
//  if (pieceType == WhitePawn && (destRow - sourceRow) > 1)
//    newMove.setDoublePawnPush();

//  if (pieceType == BlackPawn && (sourceRow - destRow) > 1)
//    newMove.setDoublePawnPush();

//  MoveList moveList;
//  mBoard->generateLegalMoves(newMove.sourceRow(), newMove.sourceColumn(), moveList);

//  bool moveFound = false;
//  for (uint i = 0; i < moveList.totalMoves(); i++) {
//    Move legalMove(moveList[i]);
//    if (legalMove == newMove) {
//      moveFound = true;
//      break;
//    }
//  }

//  if (!moveFound) {
//    std::cout << "Sorry " << moveString << " is not a valid move\n";
//    for (uint i = 0; i < moveList.totalMoves(); i++) {
//      std::cout << moveList[i].getSmithNotation() << "\n";
//    }
//    return;
//  }

//  doMove(newMove);
//  mGameOver = isGameOver();
//  if (mGameOver)
//    return;

  //executeEngineMove();
  //mGameOver = isGameOver();
}

template <typename T>
T readValue(std::istream & iss)
{
  T value = T();
  iss >> value;
  return value;
}

uint readLevel(std::istream & iss)
{
  uint perftLevel = readValue<uint>(iss);
  if (iss.fail()) {
    std::cout << "Invalid perft level\n";
    return 0;
  }
  return perftLevel;
}

void ConsoleGame::handleDivide(std::istringstream & iss) const
{
  uint perftLevel = readLevel(iss);
  if (perftLevel == 0)
    return;

  Perft perft(mBoard.get());
  perft.divide(perftLevel);
}

void ConsoleGame::handleFen() const
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
      PieceType pieceType = mBoard->pieceType(row,col);
      if (pieceType != NoPiece) {
        oss << pieceMap[pieceType];
        ++col;
      }
      else {
        uint totalBlanks = 0;
        while (pieceType == NoPiece && col < 8) {
          ++totalBlanks;
          ++col;
          pieceType = mBoard->pieceType(row,col);
        }
        oss << totalBlanks;
      }
    }
    if (row != 0)
      oss << "/";
  }

  oss << " " << (mBoard->sideToMove() == White ? "w" : "b");

  std::string castleString;
  if (mBoard->canWhiteCastleKingSide())
    castleString += "K";
  if (mBoard->canWhiteCastleQueenSide())
    castleString += "Q";
  if (mBoard->canBlackCastleKingSide())
    castleString += "k";
  if (mBoard->canBlackCastleQueenSide())
    castleString += "q";
  if (castleString.empty())
    castleString += "-";

  oss << " " << castleString;

  std::string epString;
  int epRow = mBoard->enPassantRow();
  int epCol = mBoard->enPassantCol();
  if (epRow >= 0 && epCol >= 0) {
    switch (epCol)
    {
    case 0:
      epString += "a";
      break;
    case 1:
      epString += "b";
      break;
    case 2:
      epString +="c";
      break;
    case 3:
      epString += "d";
      break;
    case 4:
      epString += "e";
      break;
    case 5:
      epString += "f";
      break;
    case 6:
      epString += "g";
      break;
    case 7:
      epString += "h";
      break;
    }
    epString += epRow + 1;
  }

  if (epString.empty())
    epString += "-";

  oss << " " << epString;
  oss << " " << mBoard->halfMoveClock();
  oss << " " << mBoard->fullMoveCounter();

  std::cout << oss.str() << "\n";
}

void ConsoleGame::handleNewGame()
{
  mGameOver = false;
  startNewGame();
}

void ConsoleGame::handlePerft(std::istringstream & iss) const
{
  uint perftLevel = readLevel(iss);
  if (perftLevel == 0)
    return;

  Perft perft(mBoard.get());

  Stopwatch stopWatch(true);
  ulonglong totalNodes = perft.execute(perftLevel);
  std::string timeString = stopWatch.timeElapsedString();
  std::cout << "Total Nodes: " << totalNodes << " Time: " << timeString << "\n";
}

void ConsoleGame::handleShow() const
{
  sMove moveList[256];
  uchar totalMoves = mBoard->generateMoves(moveList);
  for (uchar i = 0; i < totalMoves; i++)
    std::cout << mBoard->getSmithNotation(moveList[i]) << "\n";

//  std::vector<std::string> moves;
//  MoveList moveList;
//  mBoard->generateLegalMoves(moveList);
//  for (uint i = 0; i < moveList.totalMoves(); i++)
//    moves.push_back(moveList[i].getSmithNotation());

//  std::sort(moves.begin(), moves.end());
//  for (uint i = 0; i < moves.size(); i++)
//    std::cout << moves[i] << "\n";
}

void ConsoleGame::handleTable(std::istringstream & iss) const
{
//  uint perftLevel = readLevel(iss);
//  if (perftLevel == 0)
//    return;

//  std::cout << std::setw(15) << std::left << "Depth";
//  std::cout << std::setw(15) << std::left << "Nodes";
//  std::cout << std::setw(15) << std::left << "Captures";
//  std::cout << std::setw(15) << std::left << "EnPassants";
//  std::cout << std::setw(15) << std::left << "Castles";
//  std::cout << std::setw(15) << std::left << "Promotions";
//  std::cout << std::setw(15) << std::left << "Checks";
//  std::cout << std::setw(15) << std::left << "Checkmates";
//  std::cout << std::endl;

//  uint totalCaptures = 0, totalEnPassants = 0, totalCastles = 0;
//  uint totalPromotions = 0, totalChecks = 0, totalCheckmates = 0;

//  Stopwatch stopWatch(true);
//  for (uint i = 1; i <= perftLevel; i++) {
//    Perft perft(mBoard.get());
//    perft.setSaveTableResults(true);
//    ulonglong totalNodes = perft.execute(i);

//    std::cout << std::setw(15) << std::left << i;
//    std::cout << std::setw(15) << std::left << totalNodes;
//    std::cout << std::setw(15) << std::left << perft.totalCaptures() - totalCaptures;
//    std::cout << std::setw(15) << std::left << perft.totalEnpassants() - totalEnPassants;
//    std::cout << std::setw(15) << std::left << perft.totalCastles() - totalCastles;
//    std::cout << std::setw(15) << std::left << perft.totalPromotions() - totalPromotions;
//    std::cout << std::setw(15) << std::left << perft.totalChecks() - totalChecks;
//    std::cout << std::setw(15) << std::left << perft.totalCheckmates() - totalCheckmates;
//    std::cout << std::endl;

//    totalCaptures = perft.totalCaptures();
//    totalEnPassants = perft.totalEnpassants();
//    totalCastles = perft.totalCastles();
//    totalPromotions = perft.totalPromotions();
//    totalChecks = perft.totalChecks() + perft.totalCheckmates();
//    totalCheckmates = perft.totalCheckmates();
//  }

//  std::string timeString = stopWatch.timeElapsedString();
//  std::cout << "Time: " << timeString << "\n";
}

void ConsoleGame::handlePrint() const
{
  std::map<PieceType, std::string> pieceMap;
  pieceMap[NoPiece] = std::string("   ");
  pieceMap[WhitePawn] = std::string(" P ");
  pieceMap[WhiteRook] = std::string(" R ");
  pieceMap[WhiteKnight] = std::string(" N ");
  pieceMap[WhiteBishop] = std::string(" B ");
  pieceMap[WhiteQueen] = std::string(" Q ");
  pieceMap[WhiteKing] = std::string("[K]");
  pieceMap[BlackPawn] = std::string(" p ");
  pieceMap[BlackRook] = std::string(" r ");
  pieceMap[BlackKnight] = std::string(" n ");
  pieceMap[BlackBishop] = std::string(" b ");
  pieceMap[BlackQueen] = std::string(" q ");
  pieceMap[BlackKing] = std::string("[k]");

  std::cout << "\n";
  for (int row = 7; row >= 0; row--) {
    std::cout << " +---+---+---+---+---+---+---+---+\n";
    std::cout << row + 1 << "|";

    for (int col = 0; col < 8; col++) {
      PieceType pieceType = mBoard->pieceType(row, col);
      std::cout << pieceMap[pieceType];
      std::cout << "|";

      if (col == 7) {
        if (row == 7)
          std::cout << " To Move: " << ( mBoard->sideToMove() == White ? "White" : "Black");

        if (row == 6) {
          std::cout << " Castling: ";
          if (mBoard->canWhiteCastleKingSide())
            std::cout << "K";
          if (mBoard->canWhiteCastleQueenSide())
            std::cout << "Q";
          if (mBoard->canBlackCastleKingSide())
            std::cout << "k";
          if (mBoard->canBlackCastleQueenSide())
            std::cout << "q";
        }
      }
    }
    std::cout << "\n";
  }

  std::cout << " +---+---+---+---+---+---+---+---+\n";
  std::cout << "   A   B   C   D   E   F   G   H  \n";
  std::cout << "\n";

  //mBoard->printColors();
}

void ConsoleGame::handleSetBoard(std::istringstream & iss)
{
//  std::string fenString;
//  std::getline(iss, fenString);
//  if (fenString.empty()) {
//    std::cout << "A fen string must be provided\n";
//    return;
//  }

//  setBoardPosition(fenString);
}

void ConsoleGame::handleUndo()
{
  if (mTotalMoves == 0)
    return;

  sMove move = mCompletedMoves[mTotalMoves-1];
  mBoard->unmakeMove(move);
  --mTotalMoves;

  //if (mCompletedMoves->totalMoves() == 0) {
//    std::cout << "Nothing to undo!\n";
//    return;
//  }

//  undoLastMove();
//  mGameOver = isGameOver();
}

bool ConsoleGame::isGameOver()
{
  bool gameOver = false;

//  if (isBlackMated()) {
//    std::cout << "Checkmate. White wins\n";
//    gameOver = true;
//  }

//  if (isWhiteMated()) {
//    std::cout << "Checkmate. Black wins\n";
//    gameOver = true;
//  }

//  if (isStalemate()) {
//    std::cout << "Stalemate\n";
//    gameOver = true;
//  }

  return gameOver;
}

int ConsoleGame::run()
{
  std::string cmdString("5");
  std::istringstream iss(cmdString);
  handlePerft(iss);
  return 0;

  while (true) {
    std::cout << "\nsc: ";

    std::string inputString;
    std::getline(std::cin, inputString);

    std::istringstream iss(inputString);
    std::string commandString;
    iss >> commandString;
    if (commandString.empty())
      continue;

    if (commandString == "quit")
      break;
    else if (commandString == "divide")
      handleDivide(iss);
    else if (commandString == "help")
      handleHelp();
    else if (commandString == "print")
      handlePrint();
    else if (commandString == "disp")
      handlePrint();
    else if (commandString == "fen")
      handleFen();
    else if (commandString == "perft")
      handlePerft(iss);
//    else if (commandString == "table")
//      handleTable(iss);
    else if (commandString == "move")
      handleMove(iss);
//    else if (commandString == "setboard")
//      handleSetBoard(iss);
    else if (commandString == "new")
      handleNewGame();
    else if (commandString == "show")
      handleShow();
    else if (commandString == "undo")
      handleUndo();
    else {
      std::cout << "Unrecognized command " << commandString << "\n";
      std::cout << "Type help to show command list\n";
    }
  }
  return 0;
}
