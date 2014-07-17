#include "consolegame.h"
#include "perft.h"

#include <core/stopwatch.h>
#include <core/util.h>

#include <iostream>
#include <algorithm>
#include <sstream>
#include <memory>
#include <iomanip>
#include <fstream>

ConsoleGame::ConsoleGame()
{
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
  std::cout << "testmovegen..........Tests the move generator\n";
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
  if (isGameOver()) {
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
  MoveList moveList;
  uchar totalMoves = mBoard->generateMoves(moveList);
  for (uchar i = 0; i < totalMoves; i++) {
    Move move = moveList[i];
    uchar sourceRow = move.mSourceRow;
    uchar sourceCol = move.mSourceCol;
    uchar destRow = move.mDestRow;
    uchar destCol = move.mDestCol;
    if (srcRow == sourceRow && dstRow == destRow && srcCol == sourceCol && dstCol == destCol) {
      moveIndex = i;
      moveFound = true;
      break;
    }
  }

  if (!moveFound) {
    std::cout << "Sorry " << moveString << " is not a valid move\n";
    for (uint i = 0; i < totalMoves; i++)
      std::cout << getSmithNotation(moveList[i]) << "\n";
    return;
  }

  doMove(moveList[moveIndex]);
  if (isGameOver()) {
    showEndGame();
    return;
  }

  executeEngineMove();
  if (isGameOver()) {
    showEndGame();
    return;
  }
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

  std::ostringstream epString;
  int epRow = mBoard->enPassantRow();
  int epCol = mBoard->enPassantCol();
  if (epRow >= 0 && epCol >= 0) {
    switch (epCol)
    {
    case 0:
      epString << "a";
      break;
    case 1:
      epString << "b";
      break;
    case 2:
      epString << "c";
      break;
    case 3:
      epString << "d";
      break;
    case 4:
      epString << "e";
      break;
    case 5:
      epString << "f";
      break;
    case 6:
      epString << "g";
      break;
    case 7:
      epString << "h";
      break;
    }
    epString << (epRow + 1);
  }

  if (epString.str().empty())
    epString << "-";

  oss << " " << epString.str();
  oss << " " << mBoard->halfMoveClock();
  oss << " " << mBoard->fullMoveCounter();

  std::cout << oss.str() << "\n";
}

void ConsoleGame::handleNewGame()
{
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
  MoveList moveList;
  uchar totalMoves = mBoard->generateMoves(moveList);
  for (uchar i = 0; i < totalMoves; i++)
    std::cout << getSmithNotation(moveList[i]) << "\n";
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
}

void ConsoleGame::handleSetBoard(std::istringstream & iss)
{
  std::string fenString;
  std::getline(iss, fenString);
  if (fenString.empty()) {
    std::cout << "A fen string must be provided\n";
    return;
  }

  setBoardPosition(fenString);
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

void ConsoleGame::handleTestMoveGen() const
{
  std::string fileName("data/perftsuite.epd");
  std::ifstream inputStream(fileName.c_str());
  if (inputStream.fail()) {
    std::cout << "Could not locate perft test file " << fileName << "\n";
    return;
  }

  while (true) {
    std::string inputLine;
    std::getline(inputStream, inputLine);
    if (inputStream.fail())
      break;

    std::vector<std::string> tokens;
    util::split(tokens, inputLine, ';');
    if (tokens.size() < 1)
      continue;

    std::string fenString = tokens.at(0);
    if (tokens.size() > 1) {
      std::cout << "fen: " << fenString << "\n";
      mBoard->setPosition(fenString);
    }

    for (uint i = 1; i < tokens.size(); i++) {
      std::string perftDepthString = tokens.at(i);

      std::vector<std::string> perftTokens;
      util::split(perftTokens, perftDepthString, ' ');
      if (perftTokens.size() < 2)
        continue;

      std::string depthString = perftTokens.at(0);
      std::string nodesString = perftTokens.at(1);

      uint depthLevel = atoi(depthString.substr(1).c_str());
      uint numNodes = atoi(nodesString.c_str());

      Stopwatch stopWatch;
      Perft perft(mBoard.get());

      stopWatch.start();
      ulonglong totalNodes = perft.execute(depthLevel);
      std::string timeString = stopWatch.timeElapsedString();

      bool success = (totalNodes == numNodes);
      std::cout << "Perft (" << depthLevel << "): " << totalNodes << " nodes, ";
      std::cout << "Time: " << timeString << " s, [" << numNodes << "], ";
      std::cout << (success ? "OK" : "FAIL") << "\n";

      if (!success)
        return;
    }
  }
}

void ConsoleGame::handleUndo()
{
  undoLastMove();
}

int ConsoleGame::run()
{
//  std::string cmdString("4");
//  std::istringstream iss(cmdString);
//  handlePerft(iss);
//  return 0;

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
    else if (commandString == "setboard")
      handleSetBoard(iss);
    else if (commandString == "testmovegen")
      handleTestMoveGen();
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

void ConsoleGame::showEndGame()
{
  if (isBlackMated())
    std::cout << "Checkmate. White wins\n";

  if (isWhiteMated())
    std::cout << "Checkmate. Black wins\n";

  if (isStalemate())
    std::cout << "Stalemate\n";
}
