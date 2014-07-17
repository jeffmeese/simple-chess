#include "game.h"

#include <core/move.h>
#include <core/movelist.h>

#include <iostream>

Game::Game()
  : mWhiteMated(false)
  , mBlackMated(false)
  , mStalemate(false)
  , mBoard(new Board)
  , mEngine(new Engine)
  , mCompletedMoves(new MoveList)
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
  // Generate moves
  MoveList moveList;
  uchar totalMoves = mBoard->generateMoves(moveList);

  // Check for legal moves
  uchar legalMoves = 0;
  for (uchar i = 0; i < totalMoves; i++) {
    mBoard->makeMove(moveList[i]);
    if (!mBoard->isKingAttacked(!mBoard->sideToMove()))
      legalMoves++;
    mBoard->unmakeMove(moveList[i]);
  }

  // Check for checkmate
  mBlackMated = mWhiteMated = false;
  if (legalMoves == 0) {
    if (isWhiteToMove() && mBoard->isKingAttacked(White))
      mWhiteMated = true;
    else if (!isWhiteToMove() && mBoard->isKingAttacked(Black))
      mBlackMated = true;
  }

  if (mWhiteMated || mBlackMated)
    return;

  // Check for stalemate
  mStalemate = false;
  if (legalMoves == 0) {
    if (isWhiteToMove() && !mBoard->isKingAttacked(White))
      mStalemate = true;
    if (!isWhiteToMove() && !mBoard->isKingAttacked(Black))
      mStalemate = true;
  }
}

bool Game::doMove(const Move & newMove)
{
  mBoard->makeMove(newMove);
  mCompletedMoves->addMove(newMove);
  checkMateCondition();
  return true;
}

bool Game::executeEngineMove()
{
  Move engineMove;
  bool validMove = mEngine->getMove(mBoard.get(), engineMove);
  if (!validMove)
    return false;

  doMove(engineMove);
  return true;
}

uchar Game::generateMoves(uchar row, uchar col, MoveList & moveList)
{
  return mBoard->generateMoves(row, col, moveList);
}

bool Game::isBlackMated() const
{
  return mBlackMated;
}

bool Game::isGameOver() const
{
  return (mBlackMated || mWhiteMated || mStalemate);
}

bool Game::isStalemate() const
{
  return mStalemate;
}

bool Game::isWhiteMated() const
{
  return mWhiteMated;
}

bool Game::isWhiteToMove() const
{
  return (mBoard->sideToMove() == White);
}

bool Game::setBoardPosition(const std::string &fenString)
{
  if (!mBoard->setPosition(fenString))
    return false;

  mCompletedMoves->clear();
  return true;
}

void Game::startNewGame()
{
  mBoard.reset(new Board);
  mCompletedMoves->clear();
  mBlackMated = mWhiteMated = mStalemate = false;
}

bool Game::undoLastMove()
{
  uchar totalMoves = mCompletedMoves->size();
  if (totalMoves == 0)
    return false;

  Move move = mCompletedMoves->moveAt(totalMoves-1);
  mBoard->unmakeMove(move);
  mCompletedMoves->removeLast();
  checkMateCondition();

  return true;
}
