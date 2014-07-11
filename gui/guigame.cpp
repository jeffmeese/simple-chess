#include "guigame.h"

#include <QDebug>

GuiGame::GuiGame()
{

}

//GuiGame::GuiGame()
//  : mBoard(new Board)
//  , mWhiteMated(false)
//  , mBlackMated(false)
//{
//}

//void GuiGame::checkMateCondition()
//{
//  mBlackMated = mWhiteMated = false;

//  MoveList moveList;
//  uint totalMoves = mBoard->generateLegalMoves(moveList);
//  if (totalMoves == 0) {

//    qDebug() << totalMoves << mBoard->isWhiteToMove() << mBoard->isBlackChecked();
//    if (mBoard->isWhiteToMove() && mBoard->isWhiteChecked())
//      mWhiteMated = true;

//    if (!mBoard->isWhiteToMove() && mBoard->isBlackChecked())
//      mBlackMated = true;
//  }
//}

//bool GuiGame::doMove(const Move & newMove)
//{
//  mBoard->makeMove(newMove);
//  mCompletedMoves.addMove(newMove);
//  checkMateCondition();
//  emit moveCompleted(newMove);
//  return true;
//}

//bool GuiGame::executeEngineMove()
//{
//  return true;
//}

//bool GuiGame::generateMoves(uint row, uint col, MoveList & moveList)
//{
//  mBoard->generateLegalMoves(row, col, moveList);
//  return true;
//}

//PieceType GuiGame::getPieceType(uint row, uint col) const
//{
//  return mBoard->getPieceType(row, col);
//}

//bool GuiGame::isBlackMated() const
//{
//  return mBlackMated;
//}

//bool GuiGame::isWhiteMated() const
//{
//  return mWhiteMated;
//}

//bool GuiGame::isWhiteToMove() const
//{
//  return mBoard->isWhiteToMove();
//}

//bool GuiGame::undoLastMove()
//{
//  uint totalMoves = mCompletedMoves.totalMoves();
//  if (totalMoves == 0)
//    return false;

//  Move move = mCompletedMoves.moveAt(totalMoves-1);
//  mBoard->unmakeMove(move);

//  mCompletedMoves.deleteMove(totalMoves-1);
//  checkMateCondition();
//  emit moveCompleted(move);

//  return true;
//}

//#include <core/move.h>
//#include <core/movelist.h>

//Game::Game(Board * chessBoard, Engine * chessEngine)
//  : mBoard(chessBoard)
//  , mEngine(chessEngine)
//{
//}

//void Game::executeEngineMove()
//{
//  Move engineMove;
//  if (mEngine->getMove(engineMove)) {
//    mBoard->makeMove(engineMove);
//    emit engineMoveCompleted(engineMove);
//  }
//}

//bool Game::isBlackMated() const
//{
//  MoveList moveList;
//  uint totalMoves = mBoard->generateLegalMoves(moveList);
//  if (!mBoard->isWhiteToMove() && totalMoves == 0 && mBoard->isBlackChecked())
//    return true;

//  return false;
//}

//bool Game::isWhiteMated() const
//{
//  MoveList moveList;
//  uint totalMoves = mBoard->generateLegalMoves(moveList);
//  if (mBoard->isWhiteToMove() && totalMoves == 0 && mBoard->isWhiteChecked())
//    return true;

//  return false;
//}
