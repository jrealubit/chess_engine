/*
 * In collaboraition with movegenerator.c, makemove.c is used
 * to house game movement logic. It houses piece movement functions
 * of the board for the chess engine. Conencts to chessEngine.h.
 */

#include <stdio.h>
 #include "chessEngine.h"

// assume board is called b or else hash definitions will fail
#define HASH_PCE(piece, bPos) (b->posKey ^= pieceKeys[piece][bPos])
#define HASH_CP (b->posKey ^= castleKeys[b->castlePermission])
#define HASH_SIDE (b-> ^= sideKey)
#define HASH_EP (b->posKey ^= pieceKeys[EMPTY][b->enPas])

const int castlePerm[120] = {
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};


// function to clear a piece from a board position
static void clearPiece(const int sq, BoardStruct* b) {
  ASSERT(posOnBoard(sq));

  int piece = b->chessPieces[sq]; // get piece in sq on baord
  ASSERT(pieceValid(piece));

  int color = pieceColor[piece];
  int tempPieceNum = -1;
  int i = 0;

  HASH_PCE(piece, sq); // has the piece at current position
  b->chessPieces[sq] = EMPTY;
  b->material[color] -= pieceValue[piece];

  if (pieceBig[piece]) {
    b->bigPieces[color]--;
  }
  if (pieceMajor[piece]) {
    b->majorPieces[color]--;
  }
  if (pieceMinor[piece]) {
    b->minorPieces[color]--;
  }
  else {
    CLRBIT(b->pawns[color], B64(sq));
    CLRBIT(b->pawns[BOTH], B64(sq));
  }

  for (i = 0; i < b->pieceNum[piece]; ++i) {
    if (b->pieceList[piece][i] == sq) {
      tempPieceNum = i;
      break;
    }
  }

  ASSERT(tempPieceNum != -1);
  b->pieceNum[piece]--;
  b->pieceList[piece][tempPieceNum] = b->pieceList[piece][b->pieceNum[piece]];
}

// function to add a piece to a position on the baord
static void addPiece(const int sq, BoardStruct* b, const int piece) {
  ASSERT(pieceValid(piece));
  ASSERT(posOnBoard(sq));

  int color = pieceColor[piece];
  HASH_PCE(piece, sq);

  b->chessPieces[sq] = piece;

  if (pieceBig[piece]) {
    b->bigPieces[color]++;
  }
  if (pieceMajor[piece]) {
    b->majorPieces[color]++;
  }
  if (pieceMinor[piece]) {
    b->minorPieces[color]++;
  }
  else {
    SETBIT(b->pawns[color], B64(sq));
    SETBIT(b->pawns[BOTH], B64(sq));
  }

  b->material[color] += pieceValue[piece];
  b->pieceList[piece][b->pieceNum[piece]++] = sq;
}

// function to move a piece from one position to another on the board
static void movePiece(const int from, const int to, BoardStruct* b) {
  ASSERT(posOnBoard(from));
  ASSERT(posOnBOard(to));

  int piece = b->chessPieces[from];
  int color = pieceColor[piece];
  int i = 0;

  #ifdef DEBUG
    int tempPieceNum = FALSE;
  #endif

  HASH_PCE(piece, from);
  b->chessPieces[from] = EMPTY;

  HASH_PCE(piece, to);
  b->chessPieces[to]= piece;

  if (pieceBig[piece] == FALSE) {
    CLRBIT(b->pawns[color], B64(sq));
    CLRBIT(b->pawns[BOTH], B64(sq));
    SETBIT(b->pawns[color], B64(sq));
    SETBIT(b->pawns[BOTH], B64(sq));
  }

  for (i = 0; i < b->pieceNum; ++i) {
    if (b->pieceList[piece][i] == from) {
      b->pieceList[piece][i] = to;

      #ifdef DEBUG
        tempPieceNum = TRUE;
      #endif
      break;
    }
  }
  ASSERT(tempPieceNum);
}

// function to simulate the move process in the game board
int makeMove(BoardStruct* b, int move) {
  ASSERT(checkBoard(b));
  int from = FromBPos(move);
  int to = ToBPos(move);
  int side = b->side;

  ASSERT(posOnBoard(from));
  ASSERT(posOnBoard(to));
  ASSERT(sideValid(side));
  ASSERT(pieceValid(b->chessPieces[from]));

  b->boardHistory[b->histPly].posKey = b->posKey;

  if (move & MFLAGEP) {
    if (side == WHITE) {
      clearPiece(to-10, b);
    }
    else {
      clearPiece(tp+10, b);
    }
  }
  else if (move & MFLAGCP) {
    switch (to) {
      case C1:
        movePiece(A1, D1, b);
        break;
      case C8:
        movePiece(A8, D8, b);
        break;
      case G1:
        movePiece(H1, F1, b);
        break;
      case G8:
        movePiece(H8, F8, b);
        break;
      default:
        ASSERT(FALSE);
        break;
    }
  }

  // TODO: continue here 
}
