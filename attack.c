/*
 * attack.c houses the piece attack logic for the
 * board for the chess engine. Conencts to chessEngine.h.
 */

#include "chessEngine.h"

// directions from the piece square where certain
// pieces may potentially pose a threat
const int kingDir[8] = {-11, -10, -9, -1, 1, 9, 10, 11};
const int knightDir[8] = {-21, -19, -12, -8, 8, 12, 19, 21};
const int rookDir[4] = {-10, -1, 1, 10};
const int bishopDir[4] = {-11, -9, 9, 11};

// function to check if board position is
// in potential threat
int bPosAttacked(const int pos, const int side, const BoardStruct* b) {
  int i, piece, temp_pos, dir;

  // check pawns
  if (side == WHITE) {
    if (b->chessPieces[pos-11] == wP || b->chessPieces[pos-11] == wP) {
      return TRUE;
    }
  }
  else {
    if (b->chessPieces[pos+11] == bP || b->chessPieces[pos+11] == bP) {
      return TRUE;
    }
  }

  // check kings
  for (i = 0; i < 8; ++i) {
    piece = b->chessPieces[pos+kingDir[i]];
    if (IsKing(piece) && pieceColor[piece] == side) {
      return TRUE;
    }
  }

  // check knights
  for (i = 0; i < 8; ++i) {
    piece = b->chessPieces[pos+knightDir[i]]; // get piece at position
    if (IsKnight(piece) && pieceColor[piece] == side) {
      return TRUE;
    }
  }

  // check rooks and queens
  for (i = 0; i < 4; ++i) {
    dir = rookDir[i];
    temp_pos = pos+dir;
    piece = b->chessPieces[temp_pos];

    while (piece != OFFBOARD) {
      if (piece != EMPTY) {
        if (IsRookQueen(piece) && pieceColor[piece] == side) {
          return TRUE;
        }
        break;
      }
      temp_pos += dir;
      piece = b->chessPieces[temp_pos];
    }
  }

  // check bishops and queens
  for (i = 0; i < 4; ++i) {
    dir = bishopDir[i];
    temp_pos = pos+dir;
    piece = b->chessPieces[temp_pos];

    while (piece != OFFBOARD) {
      if (piece != EMPTY) {
        if (IsBishopQueen(piece) && pieceColor[piece] == side) {
          return TRUE;
        }
        break;
      }
      temp_pos += dir;
      piece = b->chessPieces[temp_pos];
    }
  }

  return FALSE; // no attacks
}
