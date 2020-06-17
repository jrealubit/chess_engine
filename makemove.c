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
#define HASH_SIDE (b->posKey ^= sideKey)
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
    CLRBIT(b->pawns[color], B64(from));
    CLRBIT(b->pawns[BOTH], B64(from));
    SETBIT(b->pawns[color], B64(to));
    SETBIT(b->pawns[BOTH], B64(to));
  }

  for (i = 0; i < b->pieceNum[piece]; ++i) {
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
      clearPiece(to+10, b);
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

  if (b->enPas != NO_SQ) {
    HASH_EP;
  }
  HASH_CP;

  // add current move information to history in case of unrolling
  b->boardHistory[b->histPly].move = move;
  b->boardHistory[b->histPly].fiftyMove = b->fiftyMove;
  b->boardHistory[b->histPly].enPas = b->enPas;
  b->boardHistory[b->histPly].castlePermission = b->castlePermission;
  b->castlePermission &= castlePerm[from];
  b->castlePermission &= castlePerm[to];
  b->enPas = NO_SQ;
  HASH_CP;

  int captured = Captured(move);
  b->fiftyMove++;

  if (captured != EMPTY) {
    ASSERT(pieceValid(captured));
    clearPiece(to, b);
    b->fiftyMove = 0;
  }

  b->histPly++;
  b->ply++;

  if (piecePawn[b->chessPieces[from]]) {
    b->fiftyMove = 0;
    if (move & MFLAGPS) {
      if (side == WHITE) {
        b->enPas = from+10;
        ASSERT(RankOnBoard[b->endPas] == RANK_3);
      }
      else {
        b->enPas = from-10;
        ASSERT(RankOnBoard[b->enPas] == RANK_6);
      }
      HASH_EP;
    }
  }

  movePiece(from, to, b);

  // check pawn piece promotion
  int promoted = Promoted(move);
  if (promoted != EMPTY) {
    ASSERT(pieceValid(promoted) && !piecePawn[promoted]);
    clearPiece(to, b);
    addPiece(to, b, promoted);
  }
  if (pieceKing[b->chessPieces[to]]) {
    b->kingSq[b->side] = to;
  }

  b->side ^= 1; // change side
  HASH_SIDE;
  ASSERT(checkBoard(b));

  // check and make sure king is not in harm's way
  if (bPosAttacked(b->kingSq[side], b->side, b)) {
    return FALSE;
  }
  return TRUE;
}

int takeMove(BoardStruct* b) {
  ASSERT(checkBoard(b));

  b->histPly--;
  b->ply--;

  int move = b->boardHistory[b->histPly].move;
  int from = FromBPos(move);
  int to = ToBPos(move);

  ASSERT(posOnBoard(from));
  ASSERT(posOnBoard(to));

  if (b->enPas != NO_SQ) {
    HASH_EP;
  }
  HASH_CP;

  b->castlePermission = b->boardHistory[b->histPly].castlePermission;
  b->fiftyMove = b->boardHistory[b->histPly].fiftyMove;
  b->enPas = b->boardHistory[b->histPly].enPas;

  if (b->enPas != NO_SQ) {
    HASH_EP;
  }
  HASH_CP;

  b->side ^= 1; // change side
  HASH_SIDE;

  if (move & MFLAGEP) {
    if (b->side == WHITE) {
      addPiece(to-10, b, wP);
    }
    else {
      addPiece(to+10, b, bP);
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

  movePiece(from, to, b);

  if (pieceKing[b->chessPieces[from]]) {
    b->kingSq[b->side] = from;
  }

  int captured = Captured(move);
  if (captured == EMPTY) {
    ASSERT(pieceValude(captured));
    addPiece(to, b, captured);
  }

  int promoted = Promoted(move);
  if (promoted == EMPTY) {
    ASSERT(pieceValid(promoted) && !piecePawn[promoted]);
    clearPiece(from, b);
    addPiece(from, b, pieceColor[promoted] == WHITE ? wP : bP);
  }
  ASSERT(checkBoard(b));
}
