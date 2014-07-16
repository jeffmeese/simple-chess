#include "game.h"

#include <core/move.h>
#include <core/movelist.h>

#include <iostream>

Game::Game()
  : mBoard(new Board)
  , mEngine(new Engine)
  , mWhiteMated(false)
  , mBlackMated(false)
  , mTotalCompletedMoves(0)
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
  mBlackMated = mWhiteMated = false;

  sMove moveList[256];
  uchar totalMoves = mBoard->generateMoves(moveList);

  if (isWhiteToMove()) {
    mWhiteMated = true;
    for (uchar i = 0; i < totalMoves; i++) {
      mBoard->makeMove(moveList[i]);
      if (!mBoard->isKingAttacked(White)) {
        mWhiteMated = false;
        mBoard->unmakeMove(moveList[i]);
        break;
      }
      mBoard->unmakeMove(moveList[i]);
    }
  }
  else {
    mBlackMated = true;
    for (uchar i = 0; i < totalMoves; i++) {
      mBoard->makeMove(moveList[i]);
      if (!mBoard->isKingAttacked(Black)) {
        mBlackMated = false;
        mBoard->unmakeMove(moveList[i]);
        break;
      }
      mBoard->unmakeMove(moveList[i]);
    }
  }

//  if (totalMoves == 0) {
//    if (isWhiteToMove() && mBoard->isKingAttacked(White))
//      mWhiteMated = true;

//    if (!isWhiteToMove() && mBoard->isKingAttacked(Black))
//      mBlackMated = true;
//  }

  mGameOver = (mBlackMated || mWhiteMated);
}

bool Game::doMove(const sMove &newMove)
{
  mBoard->makeMove(newMove);
  mCompletedMoves[mTotalCompletedMoves] = newMove;
  mTotalCompletedMoves++;

  checkMateCondition();
}

bool Game::executeEngineMove()
{
  sMove engineMove;
  bool validMove = mEngine->getMove(mBoard.get(), engineMove);
  if (!validMove)
    return false;

  doMove(engineMove);
  return true;
}

uchar Game::generateMoves(uchar row, uchar col, sMove * moveList)
{
  return mBoard->generateMoves(row, col, moveList);
}

bool Game::isBlackMated() const
{
  return mBlackMated;
}

bool Game::isStalemate() const
{
  return false;
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
  return mBoard->setPosition(fenString);
}

void Game::startNewGame()
{
  mBoard.reset(new Board);
  mTotalCompletedMoves = 0;
}

bool Game::undoLastMove()
{
  if (mTotalCompletedMoves == 0)
    return false;

  sMove move = mCompletedMoves[mTotalCompletedMoves-1];
  mBoard->unmakeMove(move);
  mTotalCompletedMoves--;

  checkMateCondition();
  return true;
}
