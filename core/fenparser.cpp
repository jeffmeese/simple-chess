#include "fenparser.h"

#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

void split(std::vector<std::string> &tokens, const std::string &text, char sep)
{
  int start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
}

// trim from start
static inline std::string& ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

// trim from end
static inline std::string& rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

// trim from both ends
static inline std::string& trim(std::string &s) {
  return ltrim(rtrim(s));
}

bool readFenString(const std::string &fenString, FenData & fenData)
{
  typedef std::map<char, PieceType> PieceMap;
  typedef std::map<char, int> EnPassantMap;

  static PieceMap pieceTypes;
  static EnPassantMap enPassantMap;
  if (pieceTypes.size() == 0 ) {
    pieceTypes['p'] = BlackPawn;
    pieceTypes['r'] = BlackRook;
    pieceTypes['n'] = BlackKnight;
    pieceTypes['b'] = BlackBishop;
    pieceTypes['q'] = BlackQueen;
    pieceTypes['k'] = BlackKing;
    pieceTypes['P'] = WhitePawn;
    pieceTypes['R'] = WhiteRook;
    pieceTypes['N'] = WhiteKnight;
    pieceTypes['B'] = WhiteBishop;
    pieceTypes['Q'] = WhiteQueen;
    pieceTypes['K'] = WhiteKing;

    enPassantMap['a'] = 0;
    enPassantMap['b'] = 1;
    enPassantMap['c'] = 2;
    enPassantMap['d'] = 3;
    enPassantMap['e'] = 4;
    enPassantMap['f'] = 5;
    enPassantMap['g'] = 6;
    enPassantMap['h'] = 7;
  }

  std::string fs(fenString);
  trim(fs);

  // Split the distinct fen tokens
  std::vector<std::string> fenTokens;
  split(fenTokens, fs, ' ');

  // There must be at least four fields
  if (fenTokens.size() < 4) {
    std::cout << "Invalid number of tokens in fen string\n";
    std::cout << "Fen String: " << fenString << "\n";
    std::cout << "Number of tokens found: " << fenTokens.size() << "\n";
    return false;
  }

  // Validate the piece string
  std::string pieceString = trim(fenTokens.at(0));
  if (pieceString.find_first_not_of("/12345678PNBRQKpnbrkq") != std::string::npos) {
    std::cout << "Error in piece specification\n";
    return false;
  }

  // Validate the side string
  std::string sideString = trim(fenTokens.at(1));
  if (sideString.find_first_not_of("wb") != std::string::npos) {
    std::cout << "Error in side specification\n";
    return false;
  }

  // Validate the castle string
  std::string castleString = trim(fenTokens.at(2));
  if (castleString.find_first_not_of("-KQkq") != std::string::npos) {
    std::cout << "Error in castle specification\n";
    return false;
  }

  // Validate the en passant string
  std::string enPassantString = trim(fenTokens.at(3));
  if (enPassantString.find_first_not_of("-abcdefgh36") != std::string::npos) {
    std::cout << "Error in en passant specification\n";
    return false;
  }

  // Validate the half move counter
  uint halfMoveClock = 0;
  if (fenTokens.size() > 4) {
    std::string halfMoveString = trim(fenTokens.at(4));
    if (halfMoveString.find_first_not_of("0123456789") != std::string::npos) {
      std::cout << "Invalid half move clock specification\n";
      return false;
    }
    halfMoveClock = atoi(halfMoveString.c_str());
  }

  // Validate the full move counter
  uint fullMoveCounter = 0;
  if (fenTokens.size() > 5) {
    std::string fullMoveString = trim(fenTokens.at(5));
    if (fullMoveString.find_first_not_of("0123456789") != std::string::npos) {
      std::cout << "Invalid full move counter specification\n";
      return false;
    }
    fullMoveCounter = atoi(fullMoveString.c_str());
  }

  // Split the piece string into rank strings
  std::vector<std::string> tokens;
  split(tokens, pieceString, '/');
  if (tokens.size() < 8) {
    std::cout << "Invalid piece string\n";
    std::cout << tokens.size() << "\n";
    return false;
  }

  for (uint row = 0; row < 8; row++) {
    for (uint col = 0; col < 8; col++) {
      fenData.mPieceType[row][col] = NoPiece;
    }
  }

  // Parse each rank string
  for (int rank = 7; rank >= 0; rank--) {
    std::string rankString = tokens.at(7-rank);
    int currentIndex = -1;
    uint file = 0;
    while (++currentIndex < rankString.length()) {
      char c = rankString.at(currentIndex);
      PieceMap::iterator itr = pieceTypes.find(c);

      // If a piece character was found
      // set the associated piece type
      if (itr != pieceTypes.end()) {
        fenData.mPieceType[rank][file] = itr->second;
        ++file;
      }
      // Skip the correct number of files for numbers
      else {
        int number = c - '0';
        file += number;
      }
    }
  }

  // Set side to move
  fenData.mWhiteToMove = true;
  if (sideString == "b")
    fenData.mWhiteToMove = false;

  // Parse the castle string
  fenData.mWhiteCastleKingSide = (castleString.find("K") != std::string::npos);
  fenData.mWhiteCastleQueenSide = (castleString.find("Q") != std::string::npos);
  fenData.mBlackCastleKingSide = (castleString.find("k") != std::string::npos);
  fenData.mBlackCastleQueenSide = (castleString.find("q") != std::string::npos);

  fenData.mEnPassantCol = 0;
  if (enPassantString.size() > 1) {
    fenData.mEnPassantCol = enPassantMap[enPassantString.at(0)];
  }

  fenData.mHalfMoveClock = halfMoveClock;
  fenData.mFullMoveCounter = fullMoveCounter;

  return true;
}
