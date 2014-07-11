#ifndef STALEMATECONDITION_H
#define STALEMATECONDITION_H

enum StalemateCondition
{
  NoStalemate = 0,
  NoLegalMoves,
  InsufficientMaterial,
  FiftyMoveRule,
  ThreeFoldRepition
};

#endif // STALEMATECONDITION_H
