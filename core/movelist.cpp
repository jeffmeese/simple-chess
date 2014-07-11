#include "movelist.h"

MoveList::MoveList()
{
  init(256);
}

MoveList::MoveList(uint totalCapacity)
{
  init(totalCapacity);
}

MoveList::~MoveList()
{
  delete mMoves;
}

void MoveList::addMove(const Move & newMove)
{
  mMoves[mTotalMoves++] = newMove;
}

void MoveList::addMoveList(const MoveList & moveList)
{
  for (uint i = 0; i < moveList.totalMoves(); i++)
    addMove(moveList[i]);
}

void MoveList::clearMoves()
{
  mTotalMoves = 0;
}

bool MoveList::deleteMove(const Move & moveToDelete)
{
  for (uint index = 0; index < mTotalMoves; index++) {
    if (mMoves[index] == moveToDelete)
      return deleteMove(index);
  }
  return false;
}

bool MoveList::deleteMove(uint index)
{
  if (index >= mTotalMoves)
    return false;

  for (uint i = index; i < mTotalMoves-1; i++) {
    mMoves[index] = mMoves[index+1];
  }

  mTotalMoves--;
  return true;
}

void MoveList::init(uint totalCapacity)
{
  mTotalMoves = 0;
  mTotalCapacity = totalCapacity;
  mMoves = new Move[totalCapacity];
}

Move & MoveList::moveAt(uint index)
{
  return mMoves[index];
}

const Move & MoveList::moveAt(uint index) const
{
  return mMoves[index];
}

Move & MoveList::operator[](uint index)
{
  return mMoves[index];
}

const Move & MoveList::operator[](uint index) const
{
  return mMoves[index];
}

uint MoveList::totalCapacity() const
{
  return mTotalCapacity;
}

uint MoveList::totalMoves() const
{
  return mTotalMoves;
}

