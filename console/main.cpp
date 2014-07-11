#include "application.h"

#include <core/board0x88.h>
#include <iostream>

int main(int argc, char ** argv)
{
  //Board0x88 board;
  //MoveList moveList;
  //board.generateLegalMoves(moveList);
  //for (uint i = 0; i < moveList.totalMoves(); i++) {
  //  std::cout << moveList[i].getSmithNotation() << "\n";
  //}
  //return 0;

  Application application(argc, argv);
  return application.execute();
}
