#include "chessEngine.h"

// function to check of square position is on the board
int posOnBoard(const int bPos) {
  if (FileOnBoard[bPos] == OFFBOARD) {
    return FALSE;
  }
  else {
    return TRUE;
  }
}

// function to check is side is valid
int sideValid(const int side) {
  if (side == WHITE || side == BLACK) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

// function to check if file and rank are valid
int fileAndRankValid(const int fileRank) {
  if (fileRank >= 0 && fileRank <= 7) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

// function to check if piece is valid
int pieceValid(const int piece) {
  if (piece >= wP && piece <= bK) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

// function to check if piece is valid, empty inclusive
int pieceValidEmpty(const int piece) {
  if (piece >= EMPTY && piece <= bK) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
