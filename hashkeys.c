/*
* hashkeys.c is used to generate the hashed position keys
* for each position on the board. Connects to chessEngine.h.
*/

#include "stdio.h"
#include "chessEngine.h"

U64 generatePositionKey(const BoardStruct* b) {
  int i = 0;
  int piece = EMPTY;
  U64 finalKey = 0;

  for (i = 0; i < BOARD_NUM; ++i) {
    piece = b->chessPieces[i];

    // check to see if it an actual piece in the board
    // that is at least a white pawn all the way to a black king
    if (piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) {
      ASSERT(piece >= wP && piece <= bK);
      finalKey ^= pieceKeys[piece][i];
    }
  }

  if (b->side == WHITE) {
    finalKey ^= sideKey;
  }

  // check to see if the en passant is set
  if (b->enPas != NO_SQ) {
    ASSERT(b->enPas >= 0 && b->enPas < BOARD_NUM);
    finalKey ^= pieceKeys[EMPTY][b->enPas];
  }

  // hash in our castling permissions
  ASSERT(b->castlePermission >= 0 && b->castlePermission <= 15);
  finalKey ^= castleKeys[b->castlePermission];
  return finalKey;
}
