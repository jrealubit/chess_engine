/*
 * movegenerator.c is used to house the piece movement logic
 * of the board for the chess engine. Conencts to chessEngine.h.
 */
#include "stdio.h"
#include "chessEngine.h"

// from, to, captured, and promotion
#define MOVE(f, t, cap, prom, fl) (f | (t<<7) | (cap<<14) | (prom<<20) | fl)
#define BPOSOFFBOARD(bPos) (FileOnBoard[bPos] == OFFBOARD)

// sliding pieces
int slidePieceList[8] = {wB, wR, wQ, 0, bB, bR, bQ, 0};
int slideLoopIndex[2] = {0, 4};

// non-sliding pieces
int nonSlidePieceList[6] = {wN, wK, 0, bN, bK, 0};
int nonSlideLoopIndex[2] = {0, 3};

// number of directions for each piece
int numDir[13] = {0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8};

// list of board positions each piece can move to
// pawns are considered as 0 all across the board
// because there are dedicated move functions for pawns
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
void addWPQuietMove(const BoardStruct* b, const int from, const int to,
  MoveListStruct* list ) {
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
    addQuietMove(b, MOVE(from, to, EMPTY, EMPTY, 0), list);
  }
}

// function to add a white pawn capture move to the list
void addWPCapMove(const BoardStruct* b, const int from, const int to,
  const int cap, MoveListStruct* list ) {
  // check to, from board positions, and captured are valid
  ASSERT(posOnBoard(from) == TRUE);
  ASSERT(posOnBoard(to) == TRUE);
  ASSERT(pieceValidEmpty(cap) == TRUE);

  if (RankOnBoard[from] == RANK_7) {
    addCaptureMove(b, MOVE(from, to, cap, wN, 0), list);
    addCaptureMove(b, MOVE(from, to, cap, wB, 0), list);
    addCaptureMove(b, MOVE(from, to, cap, wR, 0), list);
    addCaptureMove(b, MOVE(from, to, cap, wQ, 0), list);
  }
  else {
    addCaptureMove(b, MOVE(from, to, cap, EMPTY, 0), list);
  }
}

// function to add a quiet black pawn move to the list
void addBPQuietMove(const BoardStruct* b, const int from, const int to,
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
    addQuietMove(b, MOVE(from, to, EMPTY, EMPTY, 0), list);
  }
}

// function to add a black pawn capture move to the list
void addBPCapMove(const BoardStruct* b, const int from, const int to,
  const int cap, MoveListStruct* list ) {
  // assert to and from board positions are valid
  ASSERT(posOnBoard(from) == TRUE);
  ASSERT(posOnBoard(to) == TRUE);
  ASSERT(pieceValidEmpty(cap) == TRUE);

  if (RankOnBoard[from] == RANK_2) {
    addCaptureMove(b, MOVE(from, to, cap, bN, 0), list);
    addCaptureMove(b, MOVE(from, to, cap, bB, 0), list);
    addCaptureMove(b, MOVE(from, to, cap, bR, 0), list);
    addCaptureMove(b, MOVE(from, to, cap, bQ, 0), list);
  }
  else {
    addCaptureMove(b, MOVE(from, to, cap, EMPTY, 0), list);
  }
}

// function to generate all possible moves from a certain side on the board
void generateAllMoves(const BoardStruct* b, MoveListStruct* list) {
  ASSERT(checkBoard(b)); // board state must be valid

  int i = 0;
  int piece = EMPTY;
  int side = b->side;
  int bPos = 0;
  int tempBPos = 0; // temporary board position
  int pieceNum = 0;
  int pieceIndex = 0;
  int dir = 0;

  list->count = 0;

  printf("\n\nSide: %s\n", (side == WHITE) ? "WHITE" : "BLACK");

  if (side == WHITE) { // whtie side
    for (pieceNum = 0; pieceNum < b->pieceNum[wP]; ++pieceNum) {
      bPos = b->pieceList[wP][pieceNum];
      ASSERT(posOnBoard(bPos) == TRUE);

      if (b->chessPieces[bPos+10] == EMPTY) { // check front of pawn
        addWPQuietMove(b, bPos, bPos+10, list);
        if (RankOnBoard[bPos] == RANK_2 &&
        b->chessPieces[bPos+20] == EMPTY) {
          addQuietMove(b, MOVE(bPos, bPos+20, EMPTY, EMPTY, MFLAGPS), list);
        }
      }

      // check potential capture moves
      if (BPOSOFFBOARD(bPos+9) == FALSE &&
      pieceColor[b->chessPieces[bPos+9]] == BLACK) {
        addWPCapMove(b, bPos, bPos+9, b->chessPieces[bPos+9], list);
      }
      if (BPOSOFFBOARD(bPos+11) == FALSE &&
      pieceColor[b->chessPieces[bPos+11]] == BLACK) {
        addWPCapMove(b, bPos, bPos+11, b->chessPieces[bPos+11], list);
      }

      // check en passant
      if (bPos+9 == b->enPas) {
        addCaptureMove(b, MOVE(bPos, bPos+9, EMPTY, EMPTY, MFLAGEP), list);
      }
      if (bPos+11 == b->enPas) {
        addCaptureMove(b, MOVE(bPos, bPos+11, EMPTY, EMPTY, MFLAGEP), list);
      }
    }

    // castling permissions
    if (b->castlePermission & WKC) {
      if (b->chessPieces[F1] == EMPTY && b->chessPieces[G1] == EMPTY) {
        if (!bPosAttacked(E1, BLACK, b) && !bPosAttacked(F1, BLACK, b)) {
          addQuietMove(b, MOVE(E1, G1, EMPTY, EMPTY, MFLAGCP), list);
        }
      }
    }
    if (b->castlePermission & WQC) {
      if (b->chessPieces[D1] == EMPTY && b->chessPieces[C1] == EMPTY &&
      b->chessPieces[B1] == EMPTY) {
        if (!bPosAttacked(E1, BLACK, b) && !bPosAttacked(D1, BLACK, b)) {
          addQuietMove(b, MOVE(E1, C1, EMPTY, EMPTY, MFLAGCP), list);
        }
      }
    }
  }
  else { // black side
    for (pieceNum = 0; pieceNum < b->pieceNum[bP]; ++pieceNum) {
      bPos = b->pieceList[bP][pieceNum];
      ASSERT(posOnBoard(bPos) == TRUE);

      if (b->chessPieces[bPos-10] == EMPTY) { // check front of pawn
        addBPQuietMove(b, bPos, bPos-10, list);
        if (RankOnBoard[bPos] == RANK_7 &&
        b->chessPieces[bPos-20] == EMPTY) {
          addQuietMove(b, MOVE(bPos, bPos-20, EMPTY, EMPTY, MFLAGPS), list);
        }
      }

      // check potential capture moves
      if (BPOSOFFBOARD(bPos-9) == FALSE &&
      pieceColor[b->chessPieces[bPos-9]] == BLACK) {
        addWPCapMove(b, bPos, bPos-9, b->chessPieces[bPos-9], list);
      }
      if (BPOSOFFBOARD(bPos-11) == FALSE &&
      pieceColor[b->chessPieces[bPos-11]] == BLACK) {
        addWPCapMove(b, bPos, bPos-11, b->chessPieces[bPos-11], list);
      }

      // check en passant
      if (bPos-9 == b->enPas) {
        addCaptureMove(b, MOVE(bPos, bPos-9, EMPTY, EMPTY, MFLAGEP), list);
      }
      if (bPos-11 == b->enPas) {
        addCaptureMove(b, MOVE(bPos, bPos-11, EMPTY, EMPTY, MFLAGEP), list);
      }
    }

    // castling permissions
    if (b->castlePermission & BKC) {
      if (b->chessPieces[F8] == EMPTY && b->chessPieces[G8] == EMPTY) {
        if (!bPosAttacked(E8, WHITE, b) && !bPosAttacked(F8, WHITE, b)) {
          addQuietMove(b, MOVE(E8, G8, EMPTY, EMPTY, MFLAGCP), list);
        }
      }
    }
    if (b->castlePermission & BQC) {
      if (b->chessPieces[D8] == EMPTY && b->chessPieces[C8] == EMPTY &&
      b->chessPieces[B8] == EMPTY) {
        if (!bPosAttacked(E8, WHITE, b) && !bPosAttacked(D8, WHITE, b)) {
          addQuietMove(b, MOVE(E8, C8, EMPTY, EMPTY, MFLAGCP), list);
        }
      }
    }
  }

  // loop for sliding pieces
  pieceIndex = slideLoopIndex[side];
  piece = slidePieceList[pieceIndex++];

  while (piece != 0) {
    ASSERT(pieceValid(piece) == TRUE); // make sure piece is valid

    for (pieceNum = 0; pieceNum < b->pieceNum[piece]; ++pieceNum) {
      bPos = b->pieceList[piece][pieceNum];
      ASSERT(posOnBoard(bPos));

      for(i = 0; i < numDir[piece]; ++i) {
        dir = pieceDir[piece][i];
        tempBPos = bPos+dir;

        while (BPOSOFFBOARD(tempBPos) == FALSE) {
          if (b->chessPieces[tempBPos] != EMPTY) {
            if (pieceColor[b->chessPieces[tempBPos]] == side^1) {
              addCaptureMove(b, MOVE(bPos, tempBPos,
                b->chessPieces[tempBPos], EMPTY, 0), list);
            }
            break;
          }
          addQuietMove(b, MOVE(bPos, tempBPos, EMPTY, EMPTY, 0), list);
          tempBPos += dir;
        }
      }
    }
    piece = slidePieceList[pieceIndex++];
  }

  // loop for non-sliding pieces
  pieceIndex = nonSlideLoopIndex[side];
  piece = nonSlidePieceList[pieceIndex++];

  while (piece != 0) {
    ASSERT(pieceValid(piece) == TRUE); // make sure piece is valid

    for (pieceNum = 0; pieceNum < b->pieceNum[piece]; ++pieceNum) {
      bPos = b->pieceList[piece][pieceNum];
      ASSERT(posOnBoard(bPos));

      for(i = 0; i < numDir[piece]; ++i) {
        dir = pieceDir[piece][i];
        tempBPos = bPos+dir;

        if (BPOSOFFBOARD(tempBPos)) { // skip
          continue;
        }

        if (b->chessPieces[tempBPos] != EMPTY) {
          if (pieceColor[b->chessPieces[tempBPos]] == side^1) {
            addCaptureMove(b, MOVE(bPos, tempBPos,
              b->chessPieces[tempBPos], EMPTY, 0), list);
          }
          continue;
        }
        addQuietMove(b, MOVE(bPos, tempBPos, EMPTY, EMPTY, 0), list);
      }
    }
    piece = nonSlidePieceList[pieceIndex++];
  }
}
