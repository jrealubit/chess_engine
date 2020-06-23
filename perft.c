/*
* perft.c is the performance testing file that houses the testing functions
* for the chess engine. Conencts to chessEngine.h.
*/

#include "stdio.h"
#include "chessEngine.h"

long leafNodes;

void perft(int depth, BoardStruct *b) {
  ASSERT(checkBoard(b));

  if (depth == 0) {
    leafNodes++;
    return;
  }

  MoveListStruct list[1];
  generateAllMoves(b, list);

  int i = 0;
  for (i = 0; i < list->count; ++i) {
    if (!makeMove(b,list->moves[i].move)) continue;
    perft(depth-1, b);
    takeMove(b);
   }
}

void perftTest(int depth, BoardStruct *b) {
  ASSERT(checkBoard(b));

  printBoard(b);
  printf("\nStarting Test To Depth:%d\n", depth);
  leafNodes = 0;

  MoveListStruct list[1];
  generateAllMoves(b, list);

  int move;
  int i = 0;
  printf("Move count value in the board is %d\n", list->count);
  for (i = 0; i < list->count; ++i) {
    move = list->moves[i].move;
    if (!makeMove(b, move)) continue;
    long cumnodes = leafNodes;
    perft(depth-1, b);
    takeMove(b);
    long oldnodes = leafNodes - cumnodes;
    printf("move %d : %s : %ld\n", i+1, printMove(move), oldnodes);
  }
  printf("\nTest Complete : %ld nodes visited\n", leafNodes);
}
