#include "game.h"

#include <core/move.h>
#include <core/movelist.h>

#include <iostream>

Game::Game()
  : mBoard(new Board)
//  , mEngine(new Engine)
//  , mCompletedMoves(new MoveList)
  , mWhiteMated(false)
  , mBlackMated(false)
{
}

Game::~Game()
{
}

const Board * Game::board() const
{
  return mBoard.get();
}

void Game::checkMateCondition()
{
  //mBlackMated = mWhiteMated = false;

//  MoveList moveList;
//  uint totalMoves = mBoard->generateLegalMoves(moveList);
//  if (totalMoves == 0) {

//    if (mBoard->isWhiteToMove() && mBoard->isWhiteChecked())
//      mWhiteMated = true;

//    if (!mBoard->isWhiteToMove() && mBoard->isBlackChecked())
//      mBlackMated = true;
//  }
}

bool Game::doMove(const Move & newMove)
{
  //mBoard->makeMove(newMove);
  //mCompletedMoves->addMove(newMove);
  //checkMateCondition();

  return true;
}

bool Game::executeEngineMove()
{
//  Move engineMove;

//  bool validMove = mEngine->getMove(mBoard.get(), engineMove);
//  if (!validMove)
//    return false;

//  doMove(engineMove);
  return true;
}

bool Game::generateMoves(uint row, uint col, MoveList & moveList)
{
  //mBoard->generateLegalMoves(row, col, moveList);
  return true;
}

PieceType Game::getPieceType(uint row, uint col) const
{
  return NoPiece;
  //return mBoard->getPieceType(row, col);
}

bool Game::isBlackMated() const
{
  return false;
  //return mBlackMated;
}

bool Game::isStalemate() const
{
  return false;
}

bool Game::isWhiteMated() const
{
  return false;
  //return mWhiteMated;
}

bool Game::isWhiteToMove() const
{
  return true;
  //return mBoard->isWhiteToMove();
}

void Game::setBoardPosition(const std::string &fenString)
{
  //mBoard->setPosition(fenString);
}

void Game::startNewGame()
{
  mBoard.reset(new Board);
  //mCompletedMoves->clearMoves();
}

bool Game::undoLastMove()
{
//  uint totalMoves = mCompletedMoves->totalMoves();
//  if (totalMoves == 0)
//    return false;

//  Move move = mCompletedMoves->moveAt(totalMoves-1);
//  mBoard->unmakeMove(move);

//  mCompletedMoves->deleteMove(totalMoves-1);
//  checkMateCondition();

  return true;
}
