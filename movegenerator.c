#include "stdio.h"
#include "chessEngine.h"

// from, to, captured, and promotion
#define MOVE(f, t, cap, prom, fl) (f | (t<<7) | (cap<<14) | (prom<<20) | fl);
#define BPOSOFFBOARD(bPos) (FileOnBoard[bPos]==OFFBOARD)

// sliding pieces
int slidePieceLoopList[8] = {wB, wR, wQ, 0, bB, bR, bQ, 0};
int slideLoopIndex[2] = {0, 4};

// non-sliding pieces
int nonSlidePieceLoopList[6] = {wN, wK, 0, bN, bK, 0};
int nonSlideLoopIndex[2] = {0, 3};

// number of directions for each piece
int numDir[13] = {0, 0, 8, 4, 4, 8, 8, 0, 4, 4, 8, 8};

// list of board positions each piece can move to
// pawns are considered as 0 all across the board
int pieceDir[13][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0}, // empty
  {0, 0, 0, 0, 0, 0, 0, 0}, // white pawn
  {-21, -19, -12, -8, 8, 12, 19, 21}, // white knight
  {-11, -9, 9, 11, 0, 0, 0, 0}, // white bishop
  {-10, -1, 1, 10, 0, 0, 0, 0}, // white rook
  {-11, -10, -9, -1, 1, 9, 10, 11}, // white queen
  {-11, -10, -9, -1, 1, 9, 10, 11}, // white king
  {0, 0, 0, 0, 0, 0, 0, 0}, // black pawn
  {-11, -9, 9, 11, 0, 0, 0, 0}, // black bishop
  {-10, -1, 1, 10, 0, 0, 0, 0}, // black rook
  {-11, -10, -9, -1, 1, 9, 10, 11}, // black queen
  {-11, -10, -9, -1, 1, 9, 10, 11}, // black king
};

// function to add in a quiet move to the list
void addQuietMove(const BoardStruct* b, int move, MoveListStruct* list) {
  list->movesList[list->count].move = move;
  list->movesList[list->count].score = 0;
  list->count++;
}

// function to add in a capture move to the list
void addCaptureMove(const BoardStruct* b, int move, MoveListStruct* list) {
  list->movesList[list->count].move = move;
  list->movesList[list->count].score = 0;
  list->count++;
}

// function to add an en passant move to the list
void addEnPasMove(const BoardStruct* b, int move, MoveListStruct* list) {
  list->movesList[list->count].move = move;
  list->movesList[list->count].score = 0;
  list->count++;
}

// function to add a quiet white pawn move to the list
void addQuietWPMove(const BoardStruct* b, const int from, const int to,
  const int cap, MoveListStruct* list ) {
  // assert to and from board positions are valid
  ASSERT(posOnBoard(from) == TRUE);
  ASSERT(posOnBoard(to) == TRUE);

  if (RankOnBoard[from] == RANK_7) {
    addQuietMove(b, MOVE(from, to, EMPTY, wN, 0), list);
    addQuietMove(b, MOVE(from, to, EMPTY, wB, 0), list);
    addQuietMove(b, MOVE(from, to, EMPTY, wR, 0), list);
    addQuietMove(b, MOVE(from, to, EMPTY, wQ, 0), list);
  }
  else {
    addQuietMove(b, MOVE(from to, cap, EMPTY, 0), list);
  }
}

// function to add a white pawn capture move to the list
void addCaptureWPMove(const BoardStruct* b, const int from, const int to,
  const int cap, MoveListStruct* list ) {
  // assert to and from board positions are valid
  // also check that captured piece is valid
  ASSERT(posOnBoard(from) == TRUE);
  ASSERT(posOnBoard(to) == TRUE);
  ASSERT(pieceValidEmpty(cap) == TRUE);

  if (RankOnBoard[from] == RANK_7) {
    addQuietMove(b, MOVE(from, to, cap, wN, 0), list);
    addQuietMove(b, MOVE(from, to, cap, wB, 0), list);
    addQuietMove(b, MOVE(from, to, cap, wR, 0), list);
    addQuietMove(b, MOVE(from, to, cap, wQ, 0), list);
  }
  else {
    addCaptureMove(b, MOVE(from to, cap, EMPTY, 0), list);
  }
}

// function to add a quiet black pawn move to the list
void addQuietBPMove(const BoardStruct* b, const int from, const int to,
  MoveListStruct* list ) {
  // assert to and from board positions are valid
  ASSERT(posOnBoard(from) == TRUE);
  ASSERT(posOnBoard(to) == TRUE);

  if (RankOnBoard[from] == RANK_2) {
    addQuietMove(b, MOVE(from, to, EMPTY, bN, 0), list);
    addQuietMove(b, MOVE(from, to, EMPTY, bB, 0), list);
    addQuietMove(b, MOVE(from, to, EMPTY, bR, 0), list);
    addQuietMove(b, MOVE(from, to, EMPTY, bQ, 0), list);
  }
  else {
    addQuietMove(b, MOVE(from to, cap, EMPTY, 0), list);
  }
}

// function to add a black pawn capture move to the list
void addCaptureBPMove(const BoardStruct* b, const int from, const int to,
  const int cap, MoveListStruct* list ) {
  // assert to and from board positions are valid
  // also check that captured piece is valid
  ASSERT(posOnBoard(from) == TRUE);
  ASSERT(posOnBoard(to) == TRUE);
  ASSERT(pieceValidEmpty(cap) == TRUE);

  if (RankOnBoard[from] == RANK_2) {
    addQuietMove(b, MOVE(from, to, cap, bN, 0), list);
    addQuietMove(b, MOVE(from, to, cap, bB, 0), list);
    addQuietMove(b, MOVE(from, to, cap, bR, 0), list);
    addQuietMove(b, MOVE(from, to, cap, bQ, 0), list);
  }
  else {
    addCaptureMove(b, MOVE(from to, cap, EMPTY, 0), list);
  }
}

//TODO: Finish
// function to generate all possible moves from a certain side on the board
int generateAllMoves(const BoardStruct* b, MoveListStruct* list) {
  return 0;
}
