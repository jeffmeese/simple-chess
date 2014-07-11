#ifndef MOVELIST_H
#define MOVELIST_H

#include "types.h"
#include "move.h"

class MoveList
{
public:
  MoveList();
  MoveList(uint totalCapacity);
  ~MoveList();

public:
  uint totalCapacity() const;
  uint totalMoves() const;

public:
  void addMove(const Move & newMove);
  void addMoveList(const MoveList & moveList);
  void clearMoves();
  bool deleteMove(const Move & moveToDelete);
  bool deleteMove(uint index);
  Move & moveAt(uint index);
  const Move & moveAt(uint index) const;
  Move & operator[](uint index);
  const Move & operator[](uint index) const;

private:
  MoveList(const MoveList&);
  MoveList& operator=(const MoveList&);
  void init(uint totalCapacity);

private:
  Move * mMoves;
  uint mTotalCapacity;
  uint mTotalMoves;
};

#endif // MOVELIST_H
