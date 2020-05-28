/*
 * board.c is used for board representation type functionality
 * for the chess engine. Conencts to chessEngine.h.
 */

#include "stdio.h"
#include "chessEngine.h"

void resetBoard(BoardStruct* b) {
  int i = 0;

  // set the values to offboard for the 120-bit board structure
  for (i = 0; i < BOARD_NUM; ++i) {
    b->chessPieces[i] = OFFBOARD;
  }

  // set the values to EMPTY for the 64-bit board structure
  for (i = 0; i < 64; ++i) {
    b->chessPieces[B120(i)] = EMPTY;
  }

  // set all piece types to 0
  for (i = 0; i < 3; ++i) {
    b->pawns[i] = 0ULL;
    b->bigPieces[i] = 0;
    b->majorPieces[i] = 0;
    b->minorPieces[i] = 0;
  }

  // clear piece numbers on the board
  for (i = 0; i < 13; ++i) {
    b->pieceNum[i] = 0;
  }

  // set king position to no square
  // reset side, en passat, and fifty mvoe variables
  // and play, history play, and castling permissions
  b->kingSq[WHITE] = b->kingSq[BLACK] = NO_SQ;
  b->side = BOTH;
  b->enPas = NO_SQ;
  b->fiftyMove = 0;
  b->ply = b->histPly = 0;
  b->castlePermission = 0;
  b->posKey = 0ULL;
}
